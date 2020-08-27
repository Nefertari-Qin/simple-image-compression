#include "qtvar.h"
#include <iostream>
using namespace std;

/* function prunable returns true if root->var is strictly less than 
 * tol. That is, a subtree can be pruned if its variance isn't too
 * high. 
 */
bool qtvar::prunable(Node * crnt, int tol) {
    return (crnt->var < tol);
}