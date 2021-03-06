//
//  Points.cpp
//  Fractal
//
//  Created by Reid on 2015-02-20.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>

#include "Points.h"

int threadCount = 8;

float rainbowHelper(float x, float b) {
    //Analyticly derived rainbow helper function
    if ((x + b) < - 0.4) {
        return 0;
    } else if ((x + b) < - 0.1) {
        return (x + b) * 3.333333 + 1.333333;
    } else if ((x + b) < 0.1 ) {
        return 1;
    } else if ((x + b) < 0.4) {
        return - (x + b) * 3.333333 + 1.333333;
    } else {
        return 0;
    }
}

void rainbowGen(float v, float output[3]) {
    //Makes a pseudo rainbow
    output[0] = rainbowHelper(v, -.3);
    output[1] = rainbowHelper(v, -.5);
    output[2] = rainbowHelper(v, -.9);
}


template<class T>
Point<T>::Point(T r, T i) {
    value = {r, i};
    loc[0] = r / 2;
    loc[1] = i / 2;
}

template<class T>
Point<T>::Point(T x, T y, T r, T i) {
    value = {r, i};
    loc[0] = x / 2;
    loc[1] = y / 2;
}

template <class T>
Point<T>::~Point() {}

template <class T>
void Point<T>::convergeWithCheck(int n) {
    if (diverged || exterior || repeating) {
        iterations += n;
    } else {
        int oldIterations = iterations;
        while (iterations < n + oldIterations) {
            if (std::abs(value) > 2) {
                diverged = true;
                divergedAt = iterations;
                iterations = oldIterations + n;
                break;
            } else {
                iterations++;
                com<T> tmpVal = f(value);
                if (tmpVal == secondlastValue) {
                    repeating = true;
                    break;
                } else {
                    secondlastValue = lastValue;
                    lastValue = value;
                    value = tmpVal;
                }
            }
        }
    }
    
}

template <class T>
void Point<T>::convergeNoCheck(int n) {
    if (diverged || exterior || repeating) {
        iterations += n;
    } else {
        int oldIterations = iterations;
        while (iterations < n + oldIterations) {
            if (std::abs(value) > 2) {
                diverged = true;
                divergedAt = iterations;
                iterations = oldIterations + n;
                break;
            } else {
                iterations++;
                value = f(value);
            }
        }
    }
    
}

template <class T>
void Point<T>::set(T r, T i) {
    value = {r, i};
    loc[0] = r / 2;
    loc[1] = i / 2;
    repeating = false;
    com<T> lastValue = {NAN,NAN};
    com<T> secondlastValue = {NAN, NAN};
}

template <class T>
void Point<T>::set(float x, float y, T r, T i) {
    value = {r, i};
    loc[0] = x;
    loc[1] = y;
    repeating = false;
    com<T> lastValue = {NAN,NAN};
    com<T> secondlastValue = {NAN, NAN};
}

template <class T>
void Point<T>::reset(T r, T i) {
    value = {r, i};
    loc[0] = r / 2;
    loc[1] = i / 2;
    iterations = 1;
    divergedAt = 0;
    diverged = false;
    repeating = false;
    com<T> lastValue = {NAN,NAN};
    com<T> secondlastValue = {NAN, NAN};
}

template <class T>
void Point<T>::reset(float x, float y, T r, T i) {
    value = {r, i};
    loc[0] = x;
    loc[1] = y;
    iterations = 1;
    divergedAt = 0;
    diverged = false;
    repeating = false;
    com<T> lastValue = {NAN,NAN};
    com<T> secondlastValue = {NAN, NAN};
}

template <class T>
void Point<T>::makeExterior() {
    exterior = true;
}

template <class T>
void Point<T>::print(int n) {
    std::cout<< (divergedAt >= n ? '*' : '.');
}

template <class T>
void Point<T>::draw() {
    if (!exterior) {
        float colour[3];
        float x = log(divergedAt) / log(iterations);
        rainbowGen(x, colour);
        glColor3fv(colour);
        glVertex2fv(loc);
    }
}

template <class T>
void Field<T>::runRow(int y, int n) {
    if (repchecking) {
        for (int x = 0; x < dimx; x++) {
            pfield[y][x].convergeWithCheck(n);
        }
    } else {
        for (int x = 0; x < dimx; x++) {
            pfield[y][x].convergeNoCheck(n);
        }
    }
    
}

template <class T>
void Field<T>::runThread(int yi, int yf, int n) {
    yf = (yf < dimy) ? yf : dimy;
    for (int y = yi; y < yf; y++) {
        runRow(y, n);
    }
}

template <class T>
Field<T>::Field(int sx, int sy, bool square, bool checking) {
    pfield = new Point<T>* [sy];
    pthreads = new std::thread[threadCount];
    dimx = sx;
    dimy = sy;
    isSquare = square;
    repchecking = checking;
}

template <class T>
Field<T>::~Field() {
    delete [] pfield;
    delete [] pthreads;
}

template <class T>
void Field<T>::print(int n) {
    for (int y = 0; y < dimy; y++) {
        for (int x = 0; x < dimx; x++) {
            pfield[y][x].print(n);
        }
        std::cout << std::endl;
    }
}

template <class T>
void Field<T>::run(int n) {
    for (int i = 0; i < threadCount; i++) {
        pthreads[i] = std::thread(&Field::runThread, this, i * dimy / threadCount, (i + 1) * dimy / threadCount, n);
    }
    for (int i = 0; i < threadCount; i++) {
        pthreads[i].join();
    }
}

template <class T>
void Field<T>::draw() {
    glBegin(GL_POINTS);
    for (int y = 0; y < dimy; y++) {
        for (int x = 0; x < dimx; x++) {
            pfield[y][x].draw();
        }
    }
    glEnd();
}

template <class T>
void Field<T>::drawAxis() {
    if (isSquare) {
        int minDim = (dimx > dimy) ? dimy : dimx;
        glColor3f(.4, .4, .4);
        glRectf((float) minDim / dimx, 1.f / dimy, - (float) minDim / dimx , - 1.f /  dimy);
        glRectf(1 / (float) dimx, (float) minDim / dimy, - 1 / (float) dimy, - (float) minDim / dimy);
    } else {
        glColor3f(.5, .5, .5);
        glRectf(1, 1 / (float) dimy, -1, - 1 / (float) dimy);
        glRectf(1 / (float) dimx, 1, - 1 / (float) dimy, -1);
    }
}

template class drawObject<double>;
template class Field<double>;
template class Point<double>;