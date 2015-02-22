//
//  Julia.h
//  Fractal
//
//  Created by Reid on 2015-02-16.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Julia__
#define __Fractal__Julia__

#include "Points.h"
#include <thread>

template <class T>
class Jpoint : public Point<T>{
public:
    //Base constructor does not set loc or value
    Jpoint() {}
    //Sets all values
    Jpoint(T r, T i, T cr, T ci);
    //Destructor does nothing as no dymamic memory used
    ~Jpoint();
    //Julia sets iterator function x_n+1 = (x_n)^2 + c
    com<T> f(com<T> x);
    //Sets the c value
    void setC(T cr, T ci);

};

template <class T>
class Jfield : public Field<T>{
    com<T> c;
public:
    //Constructs field of Jpoints with c values set
    Jfield(int sx, int sy, T cr, T ci, bool square = false);
    //Destructor frees Jpoint array
    ~Jfield();
    //Restatrs field with give c value
    void restart(T cr, T ci);
    //Prints out c value
    void printC();
};

#endif /* defined(__Fractal__Julia__) */