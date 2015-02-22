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
#include <math.h>

#include "Points.h"

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

template <class T>
Point<T>::~Point() {}

template <class T>
void Point<T>::converge(int n) {
    if (diverged || exterior) {
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
}

template <class T>
void Point<T>::setSquare(float x, float y, T r, T i) {
    value = {r, i};
    loc[0] = x;
    loc[1] = y;
}

template <class T>
void Point<T>::reset(T r, T i) {
    value = {r, i};
    loc[0] = r / 2;
    loc[1] = i / 2;
    iterations = 1;
    divergedAt = 0;
    diverged = false;
}

template <class T>
void Point<T>::resetSquare(float x, float y, T r, T i) {
    value = {r, i};
    loc[0] = x;
    loc[1] = y;
    iterations = 1;
    divergedAt = 0;
    diverged = false;
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
void Point<T>::draw(float n) {
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
    for (int x = 0; x < dimx; x++) {
        pfield[y][x].converge(n);
    }
}

template <class T>
Field<T>::Field(int sx, int sy, bool square) {
    pfield = new Point<T>* [sy];
    pthreads = new std::thread[sy];
    dimx = sx;
    dimy = sy;
    isSquare = square;
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
    for (int y = 0; y < dimy; y++) {
        pthreads[y] = std::thread(&Field::runRow, this, y, n);
    }
    for (int y = 0; y < dimy; y++) {
        pthreads[y].join();
    }
}

template <class T>
void Field<T>::draw(int n) {
    glBegin(GL_POINTS);
    for (int y = 0; y < dimy; y++) {
        for (int x = 0; x < dimx; x++) {
            pfield[y][x].draw(n);
        }
    }
    glEnd();
}

template class Field<double>;
template class Point<double>;