#include "classStructure.h"
#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <thread>

void checkPixels(std::promise<std::vector<std::vector<int>>> result_promise, ref_image *wally, large_image *cluster, int startY, int stopY);
int *SSD(std::vector<int>&, std::vector<int>&);


//checks for the possible locations of the reference image using a small string of pixels and an SSD calculation
//uses 2 threads to increase the time efficiency and half the workload of the main processor
//the large_image is split into 2 smaller matrices and applied to the threads 
std::vector<std::vector<int>> getPixelCoOrds(ref_image *wal, large_image *clust)
{
	large_image cluster = *clust;
	ref_image wally = *wal;
	large_image cluster2 = cluster;
	ref_image wally2 = wally;
	int splitY = cluster.getNumOfRows() / 2;
	int splitY2 = splitY;


	//sets up the futures and promise of the 2 threads, this allows the threaded functions to return values
	std::promise<std::vector<std::vector<int>>> result_promise;
	std::promise<std::vector<std::vector<int>>> result_promise2;
	std::future<std::vector<std::vector<int>>> result_future = result_promise.get_future();
	std::future<std::vector<std::vector<int>>> result_future2 = result_promise2.get_future();


	//defines the threading functions, joins them and calls them
	std::thread thread1{ checkPixels, std::move(result_promise), &wally, &cluster, 24, splitY };
	std::thread thread2{ checkPixels, std::move(result_promise2), &wally2, &cluster2, splitY2, cluster2.getNumOfRows() };
	std::vector<std::vector<int>> output = result_future.get();
	std::vector<std::vector<int>> output2 = result_future2.get();
	thread1.join();
	thread2.join();

	//joins the output of the two threads back into a single large vector
	for (int i = 0; i < output2.size(); i++)
	{
		output.push_back(output2[i]);
	}
	return output;
}


//creates a string of pixels as a reference
//iterates through each position in a large matrix, creates a string of pixels as a comparison
//applies an SSD method to each comparison
//saves the output to a vector and returns the vector
//**function must be threaded using futures and promises
void checkPixels(std::promise<std::vector<std::vector<int>>> result_promise, ref_image *wallyin, large_image *clusin, int startY, int stopY)
{
	ref_image wally = *wallyin;
	large_image cluster = *clusin;
	std::vector<int> wal; //snippet 1
	std::vector<int> *Wal = &wal;
	std::vector<int> temp; //snippet 2
	std::vector<int> *Temp = &temp;
	std::vector<int> diffs;
	int *L_point;
	int *C_point;
	int *Diff;

	int centreR = wally.getNumOfRows() / 2;
	int centreC = wally.getNumOfColumbs() / 2;
	int centrePixelVal = wally.getVal(centreR, centreC);

	//creates reference vector
	for (int i = -8; i < 8; i++)
	{
		wal.push_back(wally.getVal(centreR + i, centreC));
	}
	std::vector<std::vector<int>> pixelDiffVals;
	pixelDiffVals.reserve(1200);
	std::vector<int> line;

	//iterates through lines within specified boudaries
	for (int L = startY; L < stopY; L++)
	{
		L_point = &L;
		for (int C = 18; C < cluster.getNumOfColumbs(); C++) // iterates through columbs within specified boundaries
		{
			C_point = &C;
			if (L > 8 && L < stopY - 8)
			{
				for (int i = -8; i < 8; i++) //creates comparison vector
				{
					temp.push_back(cluster.getVal(*L_point + i, *C_point));
				}
				Diff = SSD(*Wal, *Temp); // calculates the SSD of the 2 values

				if (*Diff < 400) // if SSD is within a specific thresh hold, add the location to a vector
				{
					line.push_back(L - centreR);
					line.push_back(C - centreC);
					pixelDiffVals.emplace_back(line);
					line.clear();
				}
				temp.clear();
			}
		}
	}
	result_promise.set_value(pixelDiffVals); // return vector of possible locations
}


//calculates the SSD, slightly altered from traditional SSD as if output is negative, the value is converted to positive instead of squared
int *SSD(std::vector<int>&one, std::vector<int>&two)
{
	int total = 0;
	for (int i = 0; i < one.size(); i++)
	{
		int val = one[i] - two[i];
		if (val < 0)
		{
			val = val - (2 * val); // non standard SSD line
		}
		total = total + val;
	}
	return &total;
}







