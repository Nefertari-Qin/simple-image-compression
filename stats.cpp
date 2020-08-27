#include "stats.h"

stats::stats(PNG & im) {
	unsigned int W = im.width();
	unsigned int H = im.height();
	initializeTwoDimSumVecs((int) W, (int) H);

	for (unsigned int y = 0; y < H; y++) {
		for (unsigned int x = 0; x < W; x++) {
			RGBAPixel *crnt = im.getPixel(x, y);
			setSumAtXYOf(x, y, sumRed,   (long) crnt->r);
			setSumAtXYOf(x, y, sumGreen, (long) crnt->g);
			setSumAtXYOf(x, y, sumBlue,  (long) crnt->b);

			setSumAtXYOf(x, y, sumsqRed,   (long) (crnt->r * crnt->r));
			setSumAtXYOf(x, y, sumsqGreen, (long) (crnt->g * crnt->g));
			setSumAtXYOf(x, y, sumsqBlue,  (long) (crnt->b * crnt->b));
		}
	}
}

void stats::initializeTwoDimSumVecs(int width, int height) {
	sumRed     = vector<vector<long>>(width, vector<long>(height, 0));
	sumGreen   = vector<vector<long>>(width, vector<long>(height, 0));
	sumBlue    = vector<vector<long>>(width, vector<long>(height, 0));
	sumsqRed   = vector<vector<long>>(width, vector<long>(height, 0));
	sumsqGreen = vector<vector<long>>(width, vector<long>(height, 0));
	sumsqBlue  = vector<vector<long>>(width, vector<long>(height, 0));
}

// Formular for set the sum from (0, 0) to (x, y) at (x, y) is:
// sum[North] + sum[West] + crntValue - sum[NorthWest] (use 0 if out of bounds access)
void stats::setSumAtXYOf(unsigned int x, unsigned int y, vector<vector<long>> & matrix, long crntValue) {
	int ny = (int) y - 1;
	int wx = (int) x - 1;
	long sumN  = (0 <= ny)? matrix[x][ny] : 0;
	long sumW  = (0 <= wx)? matrix[wx][y] : 0;
	long sumNW = ((0 <= wx) && (0 <= ny))? matrix[wx][ny] : 0;

	matrix[x][y] = sumN + sumW + crntValue - sumNW;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> xy, int dim) {
	double area = (double) rectArea(dim);
	double varR = (double) getSumSq('r', xy, dim) - ((double) getSum('r', xy, dim) * getSum('r', xy, dim) / area);
	double varG = (double) getSumSq('g', xy, dim) - ((double) getSum('g', xy, dim) * getSum('g', xy, dim) / area);
	double varB = (double) getSumSq('b', xy, dim) - ((double) getSum('b', xy, dim) * getSum('b', xy, dim) / area);

	double totalVar = varR + varG + varB;
	return totalVar;

}
		
RGBAPixel stats::getAvg(pair<int,int> xy, int dim) {
	int avgR = (int) ((double) getSum('r', xy, dim) / (double) rectArea(dim));
	int avgG = (int) ((double) getSum('g', xy, dim) / (double) rectArea(dim));
	int avgB = (int) ((double) getSum('b', xy, dim) / (double) rectArea(dim));

	return RGBAPixel(avgR, avgG, avgB, 1.0);
}

long stats::getSum(char channel, pair<int,int> xy, int dim) {
	int sideLength = (int) pow(2, dim);

	if (channel == 'r')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumRed,   sideLength);
	else if (channel == 'g')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumGreen, sideLength);
	else if (channel == 'b')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumBlue,  sideLength);
	else
		return -1;
}

long stats::getSumSq(char channel, pair<int,int> xy, int dim) {
	int sideLength = (int) pow(2, dim);
	
	if (channel == 'r')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumsqRed,   sideLength);
	else if (channel == 'g')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumsqGreen, sideLength);
	else if (channel == 'b')
		return getSumOfSquareAtXYWith(get<0>(xy), get<1>(xy), sumsqBlue,  sideLength);
	else
		return -1;
}

// Formular for set the sum of a square at (x, y) with size 2^dim is:
// sum[SouthEastOfSquare] + sum[NorthWestToSquare] - sum[WestToSouthWestOfSquare] - sum[NorthToNorthEastOfSquare]   
// (use 0 if out of bounds access)
long stats::getSumOfSquareAtXYWith(int x, int y, vector<vector<long>> const & matrix, int sideLength) {
	int seosX = x + sideLength - 1;
	int seosY = y + sideLength - 1;

	int nwtsX = x - 1;
	int nwtsY = y - 1;

	long sumSEOS = matrix[seosX][seosY];
	long sumNWTS = ((0 <= nwtsX) && (0 <= nwtsY))? matrix[nwtsX][nwtsY] : 0;
	long sumWSWS = ((0 <= nwtsX))? matrix[nwtsX][seosY] : 0;
	long sumNNES = ((0 <= nwtsY))? matrix[seosX][nwtsY] : 0;

	return sumSEOS + sumNWTS - sumWSWS - sumNNES;
}

long stats::rectArea(int dim) {
	return (long) (pow(2, dim) * pow(2, dim));
}

// Debug helper to examine private field:
void stats::printMatrix(vector<vector<long>> & matrix) {
	for (int y = 0; (unsigned long) y < matrix[0].size(); y++) {
		for (int x = 0; (unsigned long) x < matrix.size(); x++) {
			cout << matrix[x][y] << " "; 
		}
		cout << endl;
	}
}
