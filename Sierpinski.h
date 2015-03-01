//
//  Sierpinski.h
//  Fractal
//
//  Created by Reid on 2015-02-22.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Sierpinski__
#define __Fractal__Sierpinski__

#include "Points.h"

template <class T>
class Spoint : public Point<T>{
public:
    //Base constructor does not set loc or value
    Spoint() {}
    //Sets all values
    Spoint(T x, T y, T r, T i) {}
    //Destructor does nothing as no dymamic memory used
    ~Spoint() {}
    //Mandlebrot set iterator function
    com<T> f(com<T> x);
    
};

template <class T>
class Sfield : public Field<T>{
public:
    //Constructs field of Mpoints with intial values set
    Sfield(int sx, int sy, bool square = false);
    //Destructor frees Mpoint array
    ~Sfield();
};

#endif /* defined(__Fractal__Sierpinski__) */
