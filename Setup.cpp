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
#include <random>
#include <iostream>

#include "Setup.h"

/*Have to be global*/
namespace setup {
    //The field in the main window
    drawObject<double>* MainField;
    //Number of iterations in a step
    int step = 1;
    //Number of iterations that have occured since start or last restart
    int iters = 0;
    //Toggle for axis drawing
    bool axistoggle = false;
    
    //Global contols for animation
    bool animate = false;
    int intialIterations = 100;
    int intialTreeIterations = 10; //Analytically derived
    std::chrono::milliseconds waitTime(500);
    std::chrono::milliseconds waitSubtractor(50);
    int intialIterationsMulti = 2;
    
    //Global variables for the GLUT window
    int dimx = 500;
    int dimy = 500;
    bool squarecheck = false;
    
    //Global testing controls
    bool testing = false;
    int testingIter = 2000;
}
    
template <class T>
void readArgs(std::string w, int* argc, char* argv[]) {
    if (*argc == 1) {
        //Default produces Mandlebrot set
        setupMandl<T>(w, argc, argv, true);
    } else {
        bool mandle = false;
        bool windowed = false;
        bool square = true;
        bool serp = false;
        bool biTree = false;
        bool testing = false;
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
                if (serp) {
                    std::cout << "Invalid arguments" << '\n';
                    exit(EXIT_SUCCESS);
                }
            } else if ((!strcmp(argv[i], "-t") || !strcmp(argv[i], "-T"))) {
                testing = true;
            } else if ((!strcmp(argv[i], "-r") || !strcmp(argv[i], "-R"))) {
                square = false;
            } else if ((!strcmp(argv[i], "-b") || !strcmp(argv[i], "-B"))) {
                biTree = true;
            }else if ((!strcmp(argv[i], "-s") || !strcmp(argv[i], "-S"))) {
                serp = true;
                if (mandle) {
                    std::cout << "Invalid arguments" << '\n';
                    exit(EXIT_SUCCESS);
                }
            } else if ((!strcmp(argv[i], "-w") || !strcmp(argv[i], "-W"))) {
                //checks for -w and number of numbers give after it
                windowed = true;
                T tmpX = dimx;
                T tmpY = dimy;
                if (i < *argc - 2) {
                    try {
                        tmpX = std::stoi(argv[1 + i]);
                        tmpY = std::stoi(argv[2 + i]);
                        i += 2;
                    } catch (std::invalid_argument) {}
                }
                if (i < *argc - 1) {
                    try {
                        tmpX = tmpY = std::stoi(argv[1 + i]);
                        i++;
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
                setupMandl<T>(dimx, dimy, w, argc, argv, square, testing);
            } else {
                setupMandl<T>(w, argc, argv, square, testing);
            }
        } else if (serp) {
            if (windowed) {
                setupSerp<T>(dimx, dimy, w, argc, argv, square, testing);
            } else {
                setupSerp<T>(w, argc, argv, square, testing);
            }
        }  else if (biTree) {
            std::uniform_real_distribution<T> rt(0, 360);
            if (windowed) {
                setupTree<T, 2>(dimx, dimy, rt(gen), 0.5, w, argc, argv, square, testing);
            } else {
                setupTree<T, 2>(rt(gen), 0.5, w, argc, argv, square, testing);
            }
        }else {
            if (windowed) {
                setupJulia(dimx, dimy, jr, ji, w, argc, argv, square, testing);
            } else {
                setupJulia(jr, ji, w, argc, argv, square, testing);
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
    setup::dimx = glutGet(GLUT_SCREEN_WIDTH);
    setup::dimy = glutGet(GLUT_SCREEN_HEIGHT);
    setup::testing = t;
    if (t) {
        setup::waitTime *= 0;
        setup::step = 10;
        setup::testingIter -= setup::step * setup::intialIterations;
    }
}

void setupWindowed(int x, int y, std::string w, int* argc, char* argv[], bool t) {
    //Sets up GLUT in a window of size x by y
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(x, y);
    glutCreateWindow(w.c_str());
    setup::dimx = x;
    setup::dimy = y;
    setup::testing = t;
    if (t) {
        setup::waitTime *= 0;
        setup::step = 10;
        setup::testingIter -= setup::step * setup::intialIterations;
    }
}

template <class T>
void setupJulia(int x, int y, T r, T i, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Jfield<T>(x, y, r, i, square);
}

template <class T>
void setupJulia(T r, T i, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Jfield<T>(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), r, i, square);
}

template <class T>
void setToJulia() {
    delete setup::MainField;
    setup::MainField = new Jfield<double>(setup::dimx, setup::dimy, -1, 0, setup::squarecheck);
    setup::MainField->run(setup::step * setup::intialIterations);
    setup::iters = setup::step * setup::intialIterations;
    display();
}

template <class T>
void setupMandl(int x, int y, std::string w, int* argc, char* argv[], bool square,  bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Mfield<T>(x, y, square);
}

template <class T>
void setupMandl(std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Mfield<T>(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), square);
}

template <class T>
void setToMandl() {
    delete setup::MainField;
    setup::MainField = new Mfield<double>(setup::dimx, setup::dimy, setup::squarecheck);
    setup::MainField->run(setup::step * setup::intialIterations);
    setup::iters = setup::step * setup::intialIterations;
    display();
}

template <class T>
void setupSerp(int x, int y, std::string w, int* argc, char* argv[], bool square,  bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Sfield<T>(x, y, square);
}

template <class T>
void setupSerp(std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Sfield<T>(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT), square);
}

template <class T>
void setToSerp() {
    delete setup::MainField;
    setup::MainField = new Sfield<double>(setup::dimx, setup::dimy, setup::squarecheck);
    setup::MainField->run(setup::step * setup::intialIterations);
    setup::iters = setup::step * setup::intialIterations;
    display();
}

template <class T, int size>
void setupTree(int x, int y, T theta, T r, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupWindowed(x, y, w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Node<T, size>(theta, r, 0, -1, 1, 0, 0);
}

template <class T, int size>
void setupTree(T theta, T r, std::string w, int* argc, char* argv[], bool square, bool t) {
    setupFullscreen(w, argc, argv, t);
    setup::squarecheck = square;
    setup::MainField = new Node<T, size>(theta, r, 0, -1, 1, 0, 0);
    setup::animate = false;
}

template <class T, int size>
void setToTree() {
    delete setup::MainField;
    setup::MainField = new Node<double, size>(120, .5, 0, -1, 1, 0, 0);
    setup::MainField->run(setup::step * setup::intialTreeIterations);
    setup::iters = setup::step * setup::intialTreeIterations;
    display();
}

template <class T>
void mainloop(int* argc, char* argv[]) {
    //Runs the field a few times
    if (dynamic_cast<Node<T, 2>*>(setup::MainField)) {
        setup::MainField->run(setup::step * setup::intialTreeIterations);
        setup::iters = setup::step * setup::intialTreeIterations;
    } else {
        setup::MainField->run(setup::step * setup::intialIterations);
        setup::iters = setup::step * setup::intialIterations;
    }
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
    Jfield<double>* tmpJfield = dynamic_cast<Jfield<double>*>(setup::MainField);
    NodeInterface<double>* tmpNode = dynamic_cast<NodeInterface<double>*>(setup::MainField);
    if (tmpJfield) {
        std::random_device rdVals;
        std::default_random_engine cGen(rdVals());
        std::normal_distribution<double> cV(0, normalDeviation);
        tmpJfield->restart(cV(cGen), cV(cGen));
        tmpJfield->run(setup::step * setup::intialIterations);
        setup::iters = setup::step * setup::intialIterations;
        setup::waitTime = std::chrono::milliseconds(1000);
    } else if (tmpNode) {
        std::random_device rdVals;
        std::default_random_engine cGen(rdVals());
        std::uniform_real_distribution<double> cO(0, 360);
        tmpNode->restart(cO(cGen), 0.5);
        tmpNode->run(setup::step * setup::intialTreeIterations);
        setup::iters = setup::step * setup::intialTreeIterations;
        setup::waitTime = std::chrono::milliseconds(1000);
    }
    display();
}

void exitMain() {
    //exits and frees memory
    delete setup::MainField;
    exit(EXIT_SUCCESS);
}

void display() {
    //Draws the field
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.15, 0.05, .1, 0);
    setup::MainField->draw();
    if (setup::axistoggle) {
        setup::MainField->drawAxis();
    }
    glutSwapBuffers();
    if (setup::testing && 0 >= setup::testingIter) {
        exit(EXIT_SUCCESS);
    }
}

void idle() {
    //Checks for animation
    if (setup::animate || setup::testing) {
        setup::iters += setup::step;
        setup::MainField->run(setup::step);
        display();
        //This is to slow down the animation
        std::this_thread::sleep_for(setup::waitTime);
        //Animation speeds up as it progresses
        setup::waitTime -= setup::waitSubtractor;
        if (setup::testing) {
            setup::testingIter -= setup::step;
        }
    }
    display();
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
            Jfield<double>* tmpField = dynamic_cast<Jfield<double>*>(setup::MainField);
            if (tmpField) {
                tmpField->printC();
            }
            break;
        }
        case 'a':
            setup::axistoggle = !setup::axistoggle;
            break;
        case '1':
            setToMandl<double>();
            break;
        case '2':
            setToJulia<double>();
            break;
        case '3':
            setToSerp<double>();
            break;
        case '4':
            setToTree<double, 2>();
            break;
        case '5':
            setToTree<double, 3>();
            break;
        case '6':
            setToTree<double, 4>();
            break;
        case ' ':
            setup::animate = !setup::animate;
            break;
        case '+':
            setup::intialIterations *= setup::intialIterationsMulti;
            break;
        case '-':
            setup::intialIterations /= setup::intialIterationsMulti;
            if (setup::intialIterations == 0) {
                setup::intialIterations = 1;
            }
            break;
        case '=':
            setup::intialIterations *= setup::intialIterationsMulti;
            break;
        case '_':
            setup::intialIterations /= setup::intialIterationsMulti;
            if (setup::intialIterations == 0) {
                setup::intialIterations = 1;
            }
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

template void setupJulia(double r, double i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupJulia<double>(int x, int y, double r, double i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setToJulia<double>();

template void setupMandl<double>(std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupMandl<double>(int x, int y, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setToMandl<double>();

template void setupSerp<double>(std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupSerp<double>(int x, int y, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setToSerp<double>();

template void setupTree<double, 2>(int x, int y, double theta, double r, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setupTree<double, 2>(double theta, double r, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
template void setToTree<double, 2>();
template void setToTree<double, 3>();
template void setToTree<double, 4>();

template void mainloop<double>(int* argc, char* argv[]);