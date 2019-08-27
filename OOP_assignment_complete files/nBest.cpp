#include <iostream>
#include <vector>
#include <algorithm>
#include "classStructure.h"

std::vector<std::vector<int>> nbest_matrix(std::vector<std::vector<int>> &cluster, int &x, int &y);


/*iterates throught the large_image, creates a "forSort" object at each iteration, then compares the object to a reference.
The reference is of wally's face and therefore the method is using a form of facial recognition
The objects are then sorted into order based on the NCC value and returned to the main method */
std::vector<forSort> find_nbest_locations(large_image *clust, ref_image *wal)
{
	large_image cluster = *clust;
	ref_image wally = *wal;
	int percentageComplete = 0;
	int percentCount = 0;
	int clustCols = cluster.getNumOfColumbs();
	int clustRows = cluster.getNumOfRows();
	int wallyCols = wally.getNumOfColumbs();
	int wallyRows = wally.getNumOfRows();
	std::vector<forSort> output;
	output.reserve(1800);
	std::vector<std::vector<int>> Wally;
	std::vector<std::vector<int>> mat;
	std::vector<std::vector<int>> clusterVals = cluster.getAllVals();
	double nccVal;
	std::vector<int> line;


	//creates the wally facial reference
	for (int i = 0; i < 15; i++) {
		for (int I = 5; I < 25; I++) {
			line.push_back(wally.getVal(i, I));
		}
		Wally.push_back(line);
		line.clear();
	}


	/* increments at a rate of 9 * 12 pixels per iteration.
	calls the method to create an object matrix
	calculates the NCC value of the matrix
	creates a "forSort" object using this information
	Prints the percentage completion to the console */
	for (int i = 0; i < clustRows - 49; i += wallyRows / 4) {
		for (int I = 0; I < clustCols - 36; I += wallyCols / 4) {
			mat = nbest_matrix(clusterVals, I, i);
			nccVal = NCC(Wally, mat);

			if (nccVal > 0) {
				forSort obj(nccVal, I, i);
				output.push_back(obj);
			}
		}

		//Percentage completion bar funtionality
		percentageComplete++;
		if (percentageComplete % 3 == 0) {
			percentCount += 5;
			std::cout << percentCount << "% : ";
		}
	}
	std::cout << std::endl;


	//sorts the "forSort" objects based on the NCC values
	std::sort(output.begin(), output.end(), ncc_comp());
	return output;
}


//uses the current x and y position to iterate throught the cluster and create a matrix based on the current position of the calling method 
std::vector<std::vector<int>> nbest_matrix(std::vector<std::vector<int>> &cluster, int &x, int &y) {
	std::vector<std::vector<int>> output;
	std::vector<int> row;
	for (int i = 0; i < 15; i++) {
		for (int I = 5; I < 25; I++) {
			row.push_back(cluster[y + i][x + I]);
		}
		output.push_back(row);
		row.clear();
	}
	return output;
}
