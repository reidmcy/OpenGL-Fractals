//
//  main.cpp
//  Fractal
//
//  Created by Reid on 2015-02-16.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#include "Setup.h"

std::string window = "Fractal";
bool square = true;

int main(int argc, char * argv[]) {
    readArgs<double>(window, &argc, argv);
    mainloop<double>(&argc, argv);
}

/*
Add pan and zoom
Add export as image
Add Cantour set
*/