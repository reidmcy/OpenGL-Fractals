//
//  Mandelbrot.h
//  Fractal
//
//  Created by Reid on 2015-02-21.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Mandelbrot__
#define __Fractal__Mandelbrot__

#include "Points.h"

template <class T>
class Mpoint : public Point<T>{
public:
    //Base constructor does not set loc or value
    Mpoint() {}
    //Sets all values
    Mpoint(T r, T i);
    //Destructor does nothing as no dymamic memory used
    ~Mpoint();
    //Mandlebrot set iterator function
    com<T> f(com<T> x);
    //Sets the c value
    void setC(T r, T i);
    
};

template <class T>
class Mfield : public Field<T>{
public:
    //Constructs field of Mpoints with intial values set
    Mfield(int sx, int sy, bool square = false);
    //Destructor frees Mpoint array
    ~Mfield();
};

#endif /* defined(__Fractal__Mandelbrot__) */
