#include <iostream>
#include <vector>
#include "classStructure.h"


//Creates a matrix object for each location provided within a vector of co-ordinates, add them to a vector of matrix objects and returns the vector
std::vector<Matrix> createComparisons(std::vector<std::vector<int>> *cds, std::vector<std::vector<int>> *clsvls, int stopX, int stopY)
{
	std::vector<std::vector<int>> coOrdinates = *cds;
	std::vector<std::vector<int>> clusvals = *clsvls;
	std::vector<Matrix> matrices;
	matrices.reserve(1200);
	std::vector<std::vector<int>> *clusVals = &clusvals;

	//Loops through the vector of co-ordinates
	for (int i = 0; i < coOrdinates.size(); i++) {
		int L = coOrdinates[i][0];
		int C = coOrdinates[i][1];

		//if statements checking that the co-ordinate points are within the boundaries of the large_image when a matrix object is created
		if (L < stopY) {
			if (C < stopX) {
				Matrix M(L, C, *clusVals); // create matrix object
				matrices.push_back(M); // add matrix object to a vector
			}
		}
	}
	return matrices;
}