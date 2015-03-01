//
//  Tree.h
//  Fractal
//
//  Created by Reid on 2015-02-24.
//  Copyright (c) 2015 Reid. All rights reserved.
//

#ifndef __Fractal__Tree__
#define __Fractal__Tree__

#include "Points.h"

//Interface for Nodes to allow restart
template <class T>
class NodeInterface : public drawObject<T> {
public:
    virtual void restart(T thetar, T rr) =0;
};

//A size-tree run time signifiacntly increases with each iteration
template <class T, int size>
class Node : public NodeInterface<T> {
    Node<T, size>* subNodes[size] = {nullptr};
    T theta, r;
    T startx, starty;
    T length, phi;
    int dist;
public:
    //Creates a node d should usually be 0
    Node(T thetai, T ri, T xi, T yi, T li, T phii, int d = 0);
    //Destructor recursively deletes tree
    ~Node();
    //Iterates tree n times, use sparingly
    void run(int n);
    //Draws the whole tree
    void draw();
    //Does nothing, here for interface compatibility
    void drawAxis() {}
    //Deletes all but this node and chnge this nodes theta and r
    void restart(T thetar, T rr);
};


#endif /* defined(__Fractal__Tree__) */
