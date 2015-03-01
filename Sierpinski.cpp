//
//  Sierpinski.cpp
//  Fractal
//
//  Created by Reid on 2015-02-22.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include "Sierpinski.h"

template <class T>
bool withinCheck(T x, T min, T max) {
    //Helper function for iterator function
    if (x >= min && x <= max) {
        return true;
    }
    else {
        return false;
    }
}

template <class T>
com<T> Spoint<T>::f(com<T> x) {
    T tmpR = pow(1 / (T) 3, this->iterations - 1);
    com<T> ret = x;
    if (withinCheck<T>(std::real(x), tmpR, 2 * tmpR) && withinCheck<T>(std::imag(x), tmpR, 2 * tmpR)) {
        return {2, 2};
    }
    if(withinCheck<T>(std::real(x), tmpR, 2 * tmpR)){
        ret = {std::real(x) - tmpR, std::imag(ret)};
    } else if(withinCheck<T>(std::real(x), 2 * tmpR, 3 * tmpR)){
        ret = {std::real(x) - 2 * tmpR, std::imag(ret)};
    }
    if(withinCheck<T>(std::imag(x), tmpR, 2 * tmpR)){
        ret = {std::real(ret), std::imag(x) - tmpR};
    } else if(withinCheck<T>(std::imag(x), 2 * tmpR, 3 * tmpR)){
        ret = {std::real(ret),std::imag(x) - 2 * tmpR};
    }
    return ret;
}

template <class T>
Sfield<T>::Sfield(int sx, int sy, bool square) : Field<T>(sx, sy, square, false) {
    T centx = sx / (T) 2;
    T centy = sy / (T) 2;
    if (square) {
        int dim = (sx < sy) ? sx : sy;
        T cent = dim / (T) 2;
        for (int y = 0; y < this->dimy; y++) {
            this->pfield[y] = new Spoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                if (std::abs( 2 * (x - centx)) > dim || std::abs(2 * (y - centy)) > dim) {
                    this->pfield[y][x].makeExterior();
                    this->pfield[y][x].set(0,0);
                } else {
                    this->pfield[y][x].set((x - centx) / centx, (y - centy) / centy, ((x - centx) / cent + 1) / 2,((y - centy) / cent + 1) / 2);
                }
            }
        }
    } else{
        for (int y = 0; y < this->dimy; y++) {
            this->pfield[y] = new Spoint<T>[this->dimx];
            for (int x = 0; x < this->dimx; x++) {
                this->pfield[y][x].set((x - centx) / centx, (y - centy) / centy, ((x - centx) / centx + 1) / 2,((y - centy) / centy + 1) / 2);
            }
        }
    }
    
}

template <class T>
Sfield<T>::~Sfield() {
    for (int y = 0; y < this->dimy; y++) {
        delete [] this->pfield[y];
    }
}

template class Spoint<double>;
template class Sfield<double>;