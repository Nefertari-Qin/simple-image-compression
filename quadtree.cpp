#include "quadtree.h"
using namespace std;
using std::pair;


// Node constructor, given.
quadtree::Node::Node(pair<int,int> ul, int d,RGBAPixel a, double v)
	:upLeft(ul),dim(d),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

// quadtree destructor, given.
quadtree::~quadtree() {
	clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree & other) {
	copy(other);
}
// quadtree assignment operator, given.
quadtree & quadtree::operator=(const quadtree & rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}


quadtree::quadtree(PNG & imIn) {
	int biggestPossibleSquareSide = min(imIn.width(), imIn.height());
	int dim = (int) log2((double) biggestPossibleSquareSide);
	edge = (int) pow(2, dim);

	stats s(imIn);
	root = buildTree(s, {0, 0}, dim);
}


quadtree::Node * quadtree::buildTree(stats & s, pair<int,int> const & xy, int dim) {	
	Node* crnt = new Node(xy, dim, s.getAvg(xy, dim), s.getVar(xy, dim));

	if (dim != 0) {
		dim--;
		crnt->NW = buildTree(s, xy, dim);
		crnt->NE = buildTree(s, {xy.first + (int) pow(2, dim), xy.second}, dim);
		crnt->SE = buildTree(s, {xy.first + (int) pow(2, dim), xy.second + (int) pow(2, dim)}, dim);
		crnt->SW = buildTree(s, {xy.first, xy.second + (int) pow(2, dim)}, dim);
	} else {
		crnt->NW = NULL;
		crnt->NE = NULL;
		crnt->SE = NULL;
		crnt->SW = NULL;
	}
	return crnt;
}

void quadtree::renderLeaf(PNG & img, RGBAPixel avgPxl, int x, int y, int sideLength) {
	for (unsigned cy = (unsigned) y; cy < (unsigned) (y + sideLength); cy++) {
		for (unsigned cx = (unsigned) x; cx < (unsigned) (x + sideLength); cx++) {
			RGBAPixel *prev = img.getPixel(cx, cy);
			*prev = avgPxl;
		}
	}
}

void quadtree::renderRec(PNG & img, Node* crnt) {
	if (crnt->NW != NULL) {
		renderRec(img, crnt->NW);
		renderRec(img, crnt->NE);
		renderRec(img, crnt->SE);
		renderRec(img, crnt->SW);
	} else {
		renderLeaf(img, crnt->avg, crnt->upLeft.first, crnt->upLeft.second, (int) pow(2, crnt->dim));
	}
}

PNG quadtree::render() {
	PNG img(edge, edge);
	renderRec(img, root);
    return img;
}

int quadtree::binarySearchIdealTol(int leftTol, int rghtTol, int leaves) {
	// Relationship of Tol and LeavesNum: Tol inc, LeavesNum dec
	while (leftTol < rghtTol) {
		int mdleTol = leftTol + (rghtTol - leftTol) / 2;

		// mdleTol gives fewer or equal leaves, it's bigger or equal than idealTol, search left 
		if (pruneSize(mdleTol) <=  leaves) rghtTol = mdleTol;

		// mdleTol gives more leaves, it's less than idealTol, search right 
		else leftTol = mdleTol + 1;
	}
	// no Tol gives exact LeavesNum, return the tightestTol possible.
	return leftTol;
}

int quadtree::idealPrune(int leaves) {
	int maxTolPossible = 3 * 255 * 255;
	return binarySearchIdealTol(0, maxTolPossible, leaves);
}

int quadtree::pruneSizeRec(Node* & crnt, int tol) {
	if (crnt == NULL) return 0;

	if (prunable(crnt, tol))
		return 1;
	return pruneSizeRec(crnt->NW, tol) +
		   pruneSizeRec(crnt->NE, tol) +
		   pruneSizeRec(crnt->SE, tol) +
		   pruneSizeRec(crnt->SW, tol);
}

int quadtree::pruneSize(int tol) {
	return pruneSizeRec(root, tol);
}

void quadtree::pruneRec(Node* & crnt, int tol) {
	if (crnt->NW == NULL) return;

	if (prunable(crnt, tol)) {
		clearRec(crnt->NW);
		clearRec(crnt->NE);
		clearRec(crnt->SE);
		clearRec(crnt->SW);
	} else {
		pruneRec(crnt->NW, tol);
		pruneRec(crnt->NE, tol);
		pruneRec(crnt->SE, tol);
		pruneRec(crnt->SW, tol);
	}
}

void quadtree::prune(int tol) {
	pruneRec(root, tol);
}

void quadtree::clearRec(Node* & crnt) {
	if (crnt != NULL) {
		clearRec(crnt->NW);
		clearRec(crnt->NE);
		clearRec(crnt->SE);
		clearRec(crnt->SW);
		delete crnt;
		crnt = NULL;
	}
}

void quadtree::clear() {
	clearRec(root);
}

void quadtree::copyRec(Node* & crnt, Node* orig) {
	crnt = new Node(orig->upLeft, orig->dim, orig->avg, orig->var);
	if (orig->NW != NULL) {
		copyRec(crnt->NW, orig->NW);
		copyRec(crnt->NE, orig->NE);
		copyRec(crnt->SE, orig->SE);
		copyRec(crnt->SW, orig->SW);
	} else {
		crnt->NW = NULL;
		crnt->NE = NULL;
		crnt->SE = NULL;
		crnt->SW = NULL;
	}
}

void quadtree::copy(const quadtree & orig){
	edge = orig.edge;
	copyRec(root, orig.root);
}


