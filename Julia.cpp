//
//  Julia.cpp
//  Fractal
//
//  Created by Reid on 2015-02-16.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include <iostream>
#include "Julia.h"

template <class T>
com<T> Jpoint<T>::f(com<T> x) {
    return x * x + this->c;
}

template <class T>
Jpoint<T>::Jpoint(T r, T i, T cr, T ci) : Point<T>(r, i){
    this->c = com<T>(cr, ci);
}

template <class T>
Jpoint<T>::~Jpoint<T>() {}

template <class T>
void Jpoint<T>::setC(T cr, T ci) {
    this->c = com<T>(cr, ci);
}

template <class T>
Jfield<T>::Jfield(int sx, int sy, T cr, T ci, bool square) : Field<T>(sx, sy, square) {
    c = com<T>(cr, ci);
    T centx = sx / (T) 2;
    T centy = sy / (T) 2;
    if (square) {
        int dim = (sx < sy) ? sx : sy;
        T cent = dim / (T) 2;
        for (int y = 0; y <this->dimy; y++) {
            this->pfield[y] = new Jpoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                if (((x - centx) *(x - centx) + (y - centy) * (y - centy)) * 4 > dim * dim) {
                    this->pfield[y][x].makeExterior();
                    this->pfield[y][x].set(0,0);
                    static_cast<Jpoint<T>*>(this->pfield[y])[x].setC(0, 0);
                } else {
                    this->pfield[y][x].set((x - centx) / centx, (y - centy) / centy, 2 * (x - centx) / cent, 2 * (y - centy) / cent);
                    static_cast<Jpoint<T>*>(this->pfield[y])[x].setC(cr, ci);
                }
            }
        }

    } else {
        for (int y = 0; y < this->dimy; y++) {
            this->pfield[y] = new Jpoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                this->pfield[y][x].set(2 * (x - centx) / centx, 2 * (y - centy) / centy);
                static_cast<Jpoint<T>*>(this->pfield[y])[x].setC(cr, ci);
            }
        }
    }
    
}

template <class T>
Jfield<T>::~Jfield() {
    for (int y = 0; y < this->dimy; y++) {
        delete [] this->pfield[y];
    }
}

template <class T>
void Jfield<T>::restart(T cr, T ci) {
    T centx = this->dimx / 2;
    T centy = this->dimy / 2;
    c = com<T>(cr, ci);
    if (this->isSquare) {
        int dim = (this->dimx < this->dimy) ? this->dimx : this->dimy;
        T cent = dim / (T) 2;
        for (int y = 0; y < this->dimy; y++) {
            for (int x = 0; x < this->dimx; x++) {
                if ( std::abs(2 * (x - centx)) > dim || std::abs(2 * (y - centy)) > dim) {
                } else {
                this->pfield[y][x].reset((x - centx) / centx, (y - centy) / centy, 2 * (x - centx) / cent, 2 * (y - centy) / cent);
                static_cast<Jpoint<T>*>(this->pfield[y])[x].setC(cr, ci);
                }
            }
        }
    } else {
        for (int y = 0; y < this->dimy; y++) {
            for (int x = 0; x < this->dimx; x++) {
                this->pfield[y][x].reset(2 * (x - centx) / centx, 2 * (y - centy) / centy);
                static_cast<Jpoint<T>*>(this->pfield[y])[x].setC(cr, ci);
            }
        }
    
    }
}

template <class T>
void Jfield<T>::printC() {
    std::cout << "c = " << std::real(c) << " + " << std::imag(c) << "i\n";
}

template class Jpoint<double>;
template class Jfield<double>;