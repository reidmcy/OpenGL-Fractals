//
//  Points.h
//  Fractal
//
//  Created by Reid on 2015-02-20.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Points__
#define __Fractal__Points__

#include <complex>
#include <thread>

template <class T>
using com = std::complex<T>;

void rainbowGen(float value, float output[3]);

template <class T>
class Point {
protected:
    float loc[2];
    com<T> c;
    int iterations = 1;
    int divergedAt = 0;
    bool diverged = false;
    bool exterior = false;
    com<T> value;
public:
    //Does not set loc or value
    Point() {};
    //Sets value and loc equally
    Point(T r, T i);
    //Destrctor interface
    virtual ~Point() =0;
    //Interface for iterator function
    virtual com<T> f(com<T> x) =0;
    //Iterates the point n times
    void converge(int n);
    //Sets loc and value based on same value
    void set(T r, T i);
    //Sets loc and value separately
    void setSquare(float x, float y, T r, T i);
    //Resets Point back to like it had been just created and set()
    void reset(T r, T i);
    //Resets Point back to like it had been just created and setSquare()
    void resetSquare(float x, float y, T r, T i);
    //Makes point exterior, does not draw
    void makeExterior();
    //Prints '*' if not diverged, needs work
    void print(int n);
    //Draws a pixel at loc of colour determined by if it has diverged
    void draw(float n);
};

template <class T>
class Field {
protected:
    Point<T>** pfield;
    std::thread* pthreads;
    int dimx, dimy;
    T centx, centy;
    void runRow(int y, int n);
    bool isSquare = false;
public:
    //sets up field of sx by sy and makes it square if true
    Field(int sx, int sy, bool square = false);
    //Destructor function
    virtual ~Field() =0;
    //Prints the whole field as * and . , needs work
    void print(int n);
    //Iterates each point n times
    void run(int n);
    //Draws each point at their pixel
    void draw(int n);
};

#endif /* defined(__Fractal__Points__) */
