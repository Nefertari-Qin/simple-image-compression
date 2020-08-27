#include "qtcount.h"

/* function prunable returns true if all of the leaf nodes in root's 
 * subtree have color within tol of root->avg. Distance is measured as 
 * square distance over the color channels: (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2 
 * and "within" is inclusive of tol.
 */
int qtcount::colorDistance(RGBAPixel p1, RGBAPixel p2) {
	return (int) ((p1.r - p2.r) * (p1.r - p2.r) +
				  (p1.g - p2.g) * (p1.g - p2.g) +
				  (p1.b - p2.b) * (p1.b - p2.b));
}

bool qtcount::prunableRec(Node *crnt, int tol, RGBAPixel rootAvg) {
	if (crnt->NW == NULL) {
		return (colorDistance(crnt->avg, rootAvg) <= tol);
	}

    return (prunableRec(crnt->NW, tol, rootAvg) &&
	    	prunableRec(crnt->NE, tol, rootAvg) &&
	    	prunableRec(crnt->SE, tol, rootAvg) &&
	    	prunableRec(crnt->SW, tol, rootAvg));
}

bool qtcount::prunable(Node *rt, int tol) {
	if (rt->NW == NULL) return (0 <= tol);

    return (prunableRec(rt->NW, tol, rt->avg) &&
	    	prunableRec(rt->NE, tol, rt->avg) &&
	    	prunableRec(rt->SE, tol, rt->avg) &&
	    	prunableRec(rt->SW, tol, rt->avg));
}