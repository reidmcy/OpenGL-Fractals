//
//  Tree.cpp
//  Fractal
//
//  Created by Reid on 2015-02-24.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>

#include "Tree.h"

template <class T, int size>
Node<T, size>::Node(T thetai, T ri, T xi, T yi, T li, T phii, int d) {
    theta = thetai;
    r = ri;
    startx = xi;
    starty = yi;
    length = li;
    phi = phii;
    dist = d;
}

template <class T, int size>
void Node<T, size>::run(int n) {
    if (n <= 0) {
        return;
    } else if (subNodes[0]) {
        for (int i = 0; i < size; i++) {
            subNodes[i]->run(n);
        }
    } else {
        T nx = startx + length * sin(phi * M_PI / 180);
        T ny = starty + length * cos(phi * M_PI / 180);
        for (int i = 0; i < size; i++) {
            subNodes[i] = new Node(theta, r, nx, ny, length * r, phi + theta - 2 * i * theta / (size - 1), dist + 1);
            subNodes[i]->run(n - 1);
        }
    }
}

template <class T, int size>
Node<T, size>::~Node() {
    for (int i = 0; i < size; i++) {
        if (subNodes[i]) {
            delete subNodes[i];
        }
    }
}

template <class T, int size>
void Node<T, size>::draw() {
    float colour[3];
    rainbowGen(1 - dist / (dist + 2.f), colour);
    glColor3fv(colour);
    glBegin(GL_LINES);
    glVertex2f(startx, starty);
    glVertex2f(startx + length * sin(phi * M_PI / 180), starty + length * cos(phi * M_PI / 180));
    glEnd();
    for (int i = 0; i < size; i++) {
        if (subNodes[i]) {
            subNodes[i]-> draw();
        }
    }
}

template <class T, int size>
void Node<T, size>::restart(T thetar, T rr) {
    for (int i = 0; i < size; i++) {
        if (subNodes[i]) {
            delete subNodes[i];
            subNodes[i] = nullptr;
        }
    }
    theta = thetar;
    r = rr;
}

template class Node<double, 2>;
template class Node<double, 3>;
template class Node<double, 4>;
