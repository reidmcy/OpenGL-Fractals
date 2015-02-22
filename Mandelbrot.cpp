//
//  Mandelbrot.cpp
//  Fractal
//
//  Created by Reid on 2015-02-21.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include "Mandelbrot.h"

template <class T>
Mpoint<T>::Mpoint(T r, T i) : Point<T>(0, 0){
    this->c = com<T>(r, i);
}

template <class T>
void Mpoint<T>::setC(T r, T i) {
    this->c = {r, i};
}

template <class T>
Mpoint<T>::~Mpoint<T>() {}

template <class T>
com<T> Mpoint<T>::f(com<T> x) {
    return x * x + this->c;
}

template <class T>
Mfield<T>::Mfield(int sx, int sy, bool square) : Field<T>(sx, sy, square) {
    T centx = sx / (T) 2;
    T centy = sy / (T) 2;
    if (square) {
        int dim = (sx < sy) ? sx : sy;
        T cent = dim / (T) 2;
        for (int y = 0; y < this->dimy; y++) {
            this->pfield[y] = new Mpoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                if (((x - centx) *(x - centx) + (y - centy) * (y - centy)) * 4 > dim * dim) {
                    this->pfield[y][x].makeExterior();
                    this->pfield[y][x].set(0,0);
                } else {
                this->pfield[y][x].setSquare((x - centx) / centx, (y - centy) / centy, 2 * (x - centx) / cent, 2 * (y - centy) / cent);
                    static_cast<Mpoint<T>*>(this->pfield[y])[x].setC(2 * (x - centx) / cent, 2 * (y - centy) / cent);
                }
            }
        }
    } else{
        for (int y = 0; y < this->dimy; y++) {
            this->pfield[y] = new Mpoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                this->pfield[y][x].set(2 * (x - centx) / centx, 2 * (y - centy) / centy);
                static_cast<Mpoint<T>*>(this->pfield[y])[x].setC(2 * (x - centx) / centx, 2 * (y - centy) / centy);
            }
        }
    }
    
}

template <class T>
Mfield<T>::~Mfield() {
    for (int y = 0; y < this->dimy; y++) {
        delete [] this->pfield[y];
    }
}

template class Mpoint<double>;
template class Mfield<double>;