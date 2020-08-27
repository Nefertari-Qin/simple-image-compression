#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <utility>
#include <cmath>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"
//#include "stats.h"
using namespace std;
using namespace cs221util;

class qtcount:public quadtree {

    /* this derived class allows for a generic prune algorithm */

    public:
        qtcount(PNG & im):quadtree(im){} // invokes base class constructor
    private:

        /* function prunable returns true if all of the leaf nodes in root's 
        * subtree have color within tol of root->avg. Distance is measured as 
        * square distance over the color channels: (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2 
        * and "within" is inclusive of tol.
        */
        int colorDistance(RGBAPixel p1, RGBAPixel p2);

        bool prunableRec(Node *crnt, int tol, RGBAPixel rootAvg);

        bool prunable(Node * rt, int tol);
};

#endif
