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

//Interface to generalize objects that will be displayed
template <class T>
class drawObject {
public:
    //Iterates the object n times
    virtual void run(int n) =0;
    //Draws the object
    virtual void draw() =0;
    //Draws the axis
    virtual void drawAxis() =0;
    //Destructor interface
    virtual ~drawObject() {}
};

template <class T>
class Point{
protected:
    float loc[2];
    com<T> c;
    int iterations = 1;
    int divergedAt = 0;
    bool diverged = false;
    bool exterior = false;
    bool repeating = false;
    com<T> value;
    com<T> lastValue = {NAN,NAN};
    com<T> secondlastValue = {NAN, NAN};
    //com<T> lastValue = {NAN,NAN};
public:
    //Does not set loc or value
    Point() {};
    //Sets value and loc equally
    Point(T r, T i);
    //Sets value and loc seperately
    Point(T x, T y, T r, T i);
    //Destrctor interface
    virtual ~Point() =0;
    //Interface for iterator function
    virtual com<T> f(com<T> x) =0;
    //Iterates the point n times, checking for repeats
    void convergeWithCheck(int n);
    //Iterates the point n times
    void convergeNoCheck(int n);
    //Sets loc and value based on same value
    void set(T r, T i);
    //Sets loc and value separately
    void set(float x, float y, T r, T i);
    //Resets Point back to like it had been just created and set()
    void reset(T r, T i);
    //Resets Point back to like it had been just created and set()
    void reset(float x, float y, T r, T i);
    //Makes point exterior, does not draw
    void makeExterior();
    //Prints '*' if not diverged, needs work
    void print(int n);
    //Draws a pixel at loc of colour determined by if it has diverged
    void draw();
    
};

template <class T>
class Field : public drawObject<T>{
protected:
    Point<T>** pfield;
    std::thread* pthreads;
    int dimx, dimy;
    T centx, centy;
    void runRow(int y, int n);
    void runThread(int yi, int yf, int n);
    bool isSquare = false;
    bool repchecking = true;
public:
    //sets up field of sx by sy and makes it square if true
    Field(int sx, int sy, bool square = false, bool checking = true);
    //Destructor function
    virtual ~Field() =0;
    //Prints the whole field as * and . , needs work
    void print(int n);
    //Iterates each point n times
    void run(int n);
    //Draws each point at their pixel
    void draw();
    //Draw axis with (0,0) at center
    void drawAxis();
};

#endif /* defined(__Fractal__Points__) */
