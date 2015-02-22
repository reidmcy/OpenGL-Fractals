//
//  main.cpp
//  Fractal
//
//  Created by Reid on 2015-02-16.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include "Julia.h"
#include "Setup.h"
#include <cstdlib>
#include <random>

std::string window = "Fractal";
bool square = true;

int main(int argc, char * argv[]) {
    readArgs<double>(window, &argc, argv);
    mainloop<double>(&argc, argv);
}