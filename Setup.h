//
//  Setup.h
//  Fractal
//
//  Created by Reid on 2015-02-17.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Setup__
#define __Fractal__Setup__

#include "Points.h"
#include "Julia.h"
#include "Mandelbrot.h"
#include "Sierpinski.h"
#include "Tree.h"

//deviation of the gaussian used to generate Julia set's c
static const double normalDeviation = 0.6;

//Reads through argv and setups the field and GL window accordingly
template <class T>
void readArgs(std::string w, int* argc, char* argv[]);

/*for all setups:
square controls whether or not the field is square or scaled to fit the given rectangle
t is set for testing
*/

//Creates a Julia field of dimensions x by y
template <class T>
void setupJulia(int x, int y, T r, T i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);

//Creates a fullscreen Julia field
template <class T>
void setupJulia(T r, T i, std::string w, int* argc, char* argv[], bool square = false, bool t = false);
 
//Creates a Mandlbrot field of dimensions x by y
template <class T>
void setupMandl(int x, int y, std::string w, int* argc, char* argv[], bool square = false, bool t = false);

//Sets main field to Julia field
template <class T>
void setToJulia();

//Creates a fullscreen Mandlbrot field
template <class T>
void setupMandl(std::string w, int* argc, char* argv[], bool square = false, bool t = false);

//Sets main field to mandlbrot field
template <class T>
void setToMandl();

//Creates a Sierpinski carpet of dimensions x by y
template <class T>
void setupSerp(int x, int y, std::string w, int* argc, char* argv[], bool square = false, bool t = false);

//Creates a fullscreen Sierpinski carpet
template <class T>
void setupSerp(std::string w, int* argc, char* argv[], bool square = false, bool t = false);

//Sets main field to Sierpinski carpet
template <class T>
void setToSerp();

//Creates a n-tree of dimensions x by y
template <class T, int size>
void setupTree(int x, int y, T theta, T r, std::string w, int* argc, char* argv[], bool square = true, bool t = false);

//Creates a fullscreen n-tree
template <class T, int size>
void setupTree(T theta, T r, std::string w, int* argc, char* argv[], bool square = true, bool t = false);

//Sets main field to a n-tree
template <class T, int size>
void setToTree();

//Starts the main GLUT loop, starts everything running
template <class T>
void mainloop(int* argc, char* argv[]);

//Restarts the current field with random values if the field supports restarting
void restartMain();

//Exits the program, freeing evrything as required
void exitMain();

//GLUT MainLoop functions
void display();
void idle();
void keyb(unsigned char k, int x, int y);
void skeyb(int k, int x, int y);

#endif /* defined(__Fractal__Setup__) */