//
//  Setup.cpp
//  Fractal
//
//  Created by Reid on 2015-02-17.
//  Copyright (c) 2015 Reid. All rights reserved.
//


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include "Setup.h"


/*Have to be global*/
//The field in the main window
Field<double>* MainField;
//Number of iterations in a step
int step = 1;
//Number of iterations that have occured since start or last restart
int iters = 0;

//Global contols for animation
bool animate = false;
std::chrono::milliseconds waitTime(500);
std::chrono::milliseconds waitSubtractor(100);

//Global testing controls
bool testing = false;
int testingIter = 100;

template <class T>
void readArgs(std::string w, int* argc, char* argv[]) {
    if (*argc == 1) {
        //Default no args produces random Julia set
        std::random_device rd;
        std::default_random_engine gen(rd());
        std::normal_distribution<double> r(0, normalDeviation);
        setupJulia<T>(r(gen), r(gen), w, argc, argv, true);
    } else {
        bool mandle = false;
        bool windowed = false;
        bool square = true;
        int dimx = 500;
        int dimy = 500;
        std::random_device rd;
        std::default_random_engine gen(rd());
        std::normal_distribution<T> r(0, normalDeviation);
        T jr = r(gen);
        T ji = r(gen);
        for (int i = 1; i < *argc; i++) {
            if ((!strcmp(argv[i], "-m") || !strcmp(argv[i], "-m"))) {
                mandle = true;
            } else if ((!strcmp(argv[i], "-r") || !strcmp(argv[i], "-R"))) {
                square = false;
            } else if ((!strcmp(argv[i], "-w") || !strcmp(argv[i], "-W"))) {
                //checks for -w and number of numbers give after it
                windowed = true;
                T tmpX = dimx;
                T tmpY = dimy;
                if (i < *argc - 2) {
                    try {
                        tmpX = std::stoi(argv[1 + i]);
                        tmpY = std::stoi(argv[2 + i]);
                    } catch (std::invalid_argument) {}
                }
                if (i < *argc - 1) {
                    try {
                        tmpX = tmpY = std::stoi(argv[1 + i]);
                    } catch (std::invalid_argument) {}
                }
                dimx = tmpX;
                dimy = tmpY;
            } else if (!mandle && i < *argc - 1) {
                //sets the Julia sets c
                try {
                    jr = std::stod(argv[i]);
                    ji = std::stod(argv[1 + i]);
                    i++;
                } catch (std::invalid_argument) {
                    std::cout << "Invalid arguments" << '\n';
                    exit(EXIT_SUCCESS);
                }
            } else {
                std::cout << "Invalid arguments" << '\n';
                exit(EXIT_SUCCESS);
            }
        }
        //setups the field based on flags
        if (mandle) {
            if (windowed) {
                setupMandl<T>(dimx, dimy, w, argc, argv, square);
            } else {
                setupMandl<T>(w, argc, argv, square);
            }
        } else {
            if (windowed) {
                setupJulia(dimx, dimy, jr, ji, w, argc, argv, square);
            } else {
                setupJulia(jr, ji, w, argc, argv, square);
            }
        }
    }
}

void setupFullscreen(std::string w, int* argc, char* argv[], bool t) {
    //sets up GLUT in fullscreen
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutFullScreen();
    glutCreateWindow(w.c_str());
    glutFullScreen();
    testing = t;
    if (t) {
        waitTime *= 0;
        step = 10;
    }
}

void setupWindowed(int x, int y, std::string w, int* argc, char* argv[], bool t) {
    //Sets up GLUT in a window of size x by y
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(x, y);
    glutCreateWindow(w.c_str());
    testing = t;
    if (t) {
        waitTime *= 0;
        step = 10;
    }
}

template <class T>
void setupMandl(int x, int y, std::string w, int* argc, char* argv[], bool square,  bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    MainField = new Mfield<T>(x, y, square);
}

template <class T>
void setupMandl(std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    MainField = new Mfield<T>(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), square);
}

template <class T>
void setupJulia(int x, int y, T r, T i, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    MainField = new Jfield<T>(x, y, r, i, square);
}

template <class T>
void setupJulia(T r, T i, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    MainField = new Jfield<T>(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), r, i, square);
}

template <class T>
void mainloop(int* argc, char* argv[]) {
    //Runs the field a few times
    MainField->run(step * 10);
    iters = step * 10;
    //makes cursor invisible
    glutSetCursor(GLUT_CURSOR_NONE);
    //setup for all the GLUT functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyb);
    glutSpecialFunc(skeyb);
    glutIdleFunc(idle);
    glutMainLoop();
}

void restartMain() {
    //check if field can be restarted if so does
    Jfield<double>* tmpField = dynamic_cast<Jfield<double>*>(MainField);
    if (tmpField) {
        std::random_device rdVals;
        std::default_random_engine cGen(rdVals());
        std::normal_distribution<double> cV(0, normalDeviation);
        tmpField->restart(cV(cGen), cV(cGen));
        tmpField->run(step * 10);
        iters = step * 10;
        waitTime = std::chrono::milliseconds(1000);
    }
    display();
}

void exitMain() {
    //exits and frees memory
    delete MainField;
    exit(EXIT_SUCCESS);
}

void display() {
    //Draws the field
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 0);
    MainField->draw(iters);
    glutSwapBuffers();
    if (testing) {
        testingIter--;
        if (testingIter <= 0) {
            exit(EXIT_SUCCESS);
        }
    }
}

void idle() {
    //Checks for animation
    if (animate || testing) {
        iters += step;
        MainField->run(step);
        display();
        //This is to slow down the animation
        std::this_thread::sleep_for(waitTime);
        //Animation speeds up as it progresses
        waitTime -= waitSubtractor;
    }
}

void keyb(unsigned char k, int x, int y) {
    switch (k) {
        case 27:
            exitMain();
            break;
        case 'q':
            exitMain();
            break;
        case 'r':
            restartMain();
            break;
        case 'p': {
            Jfield<double>* tmpField = dynamic_cast<Jfield<double>*>(MainField);
            if (tmpField) {
                tmpField->printC();
            }
            break;
        }
        case ' ':
            animate = !animate;
            break;
        case '+':
            waitTime -= waitSubtractor;
            break;
        case '-':
            waitTime += waitSubtractor;
            break;
        case '=':
            waitTime -= waitSubtractor;
            break;
        case '_':
            waitTime += waitSubtractor;
            break;
        default:
            break;
    }
}

void skeyb(int k, int x, int y) {
    switch (k) {
        default:
            break;
    }
}

template void readArgs<double>(std::string w, int* argc, char* argv[]);

template void setupMandl<double>(std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupMandl<double>(int x, int y, std::string w, int* argc, char* argv[], bool square = false, bool t = false);

template void setupJulia(double r, double i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupJulia<double>(int x, int y, double r, double i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);

template void mainloop<double>(int* argc, char* argv[]);