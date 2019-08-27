#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "classStructure.h"

std::vector<std::vector<int>> getPixelCoOrds(ref_image *wally, large_image *cluster);
void write_pgm(std::string filename, double *data, int sizeR, int sizeC, int Q);
double *convertToOneD(std::vector<std::vector<int>>*);
std::vector<std::vector<int>> createBox(std::vector<std::vector<int>> &matrix, int *rowNum, int *colNum);
int NCCsearch(std::vector<std::vector<int>> *m1, std::vector<Matrix>&, std::vector<std::vector<int>>*);
std::vector<Matrix> createComparisons(std::vector<std::vector<int>> *coords, std::vector<std::vector<int>> *clusvals, int stopX, int stopY);
std::vector<forSort> find_nbest_locations(large_image *cluster, ref_image *wally);
std::vector<forSort> removeOverlap(std::vector<forSort> *matrices, ref_image *wally);

int main()
{
	clock_t begin = clock();


	//reads the .txt files into image objects
	std::string wallyFile = "Wally_grey.txt";
	std::string fullImg = "Cluttered_scene.txt";
	std::cout << "Reading in images..." << std::endl;
	std::cout << std::endl;


	//creates objects, and pointers to the objects for each of the input images
	ref_image Wally(wallyFile);
	ref_image *wally = new ref_image;
	*wally = Wally;
	large_image Cluster(fullImg);
	large_image *cluster = new large_image;
	*cluster = Cluster;


	//These pointers can be used to call the virtual function which returns the object information
	Matrix *img1 = wally;
	Matrix *img2 = cluster;


	//This section prints the information of the input images to the console screen using virtual functions
	std::cout << "==== Referance and full size image information ====";
	img1->get_info(img1->getAllVals(), Wally.getAllVals());
	img2->get_info(img2->getAllVals(), Wally.getAllVals());
	std::cout << std::endl;


	//calls an SSD function to find the possible locations of the ref_image within the large_image
	std::cout << "checking for possible locations..." << std::endl;
	std::vector<std::vector<int>> close_match_coOrdinates;
	close_match_coOrdinates.reserve(1000);
	close_match_coOrdinates = getPixelCoOrds(wally, cluster); //returns the co-ordinates of the possible locations
	delete wally, cluster, img1, img2;


	//Creates pointer to vector of co-ordinates
	std::vector<std::vector<int>> *coOrdinates_pointer = new std::vector<std::vector<int>>;
	*coOrdinates_pointer = close_match_coOrdinates;


	//creates a vector of matrix objects within the boudaries of the cluster image using the possible location points
	std::vector<Matrix> matrices;
	std::vector<std::vector<int>> cluster_values;
	cluster_values.reserve(1200);
	cluster_values = Cluster.getAllVals();
	std::vector<std::vector<int>> *cluster_values_pointer = new std::vector<std::vector<int>>;
	*cluster_values_pointer = cluster_values;
	matrices.reserve(1200);
	std::cout << "Creating image comparisons..." << std::endl;
	matrices = createComparisons(coOrdinates_pointer, cluster_values_pointer, Cluster.getNumOfColumbs() - (Wally.getNumOfColumbs()), Cluster.getNumOfRows() - (Wally.getNumOfRows())); //returns created objects of possible locations


	//Creates a vector of pointers to the Matrix objects within "matrices"																																											 //created vector of pointers to the created objects
	std::vector<Matrix> mtxs;
	mtxs.reserve(1200);
	for (int i = 0; i < matrices.size(); i++) {
		Matrix *m = &matrices[i];
		mtxs.push_back(*m);
	}


	//uses an NCC algorithm to compare the possible location matrices with the ref_image
	std::cout << "Finding Wally..." << std::endl;
	std::cout << std::endl;
	int locationOfMatrix = NCCsearch(&Wally.getAllVals(), mtxs, coOrdinates_pointer);
	delete coOrdinates_pointer, mtxs;


	//shows the row and columb number for the upper left corner of the most likely position
	std::cout << "Wally found at co-ordinate" << " X =  " << close_match_coOrdinates[locationOfMatrix][1] << " Y = " << close_match_coOrdinates[locationOfMatrix][0] << std::endl;
	std::cout << std::endl;


	//applys a black box around the location on the large_image to highlight the location of the ref_image
	std::vector<std::vector<int>> revisedCluster;
	revisedCluster.reserve(1200);
	revisedCluster = createBox(*cluster_values_pointer, &close_match_coOrdinates[locationOfMatrix][0], &close_match_coOrdinates[locationOfMatrix][1]);


	//converts the updated lagre_image and the ref_image to a 1D array
	double *ARR = new double[Cluster.getNumOfColumbs() * Cluster.getNumOfRows()];
	double *ARR2 = new double[Wally.getNumOfColumbs() * Wally.getNumOfRows()];
	ARR = convertToOneD(&revisedCluster);
	ARR2 = convertToOneD(&Wally.getAllVals());


	//writes the information from the converted arrays into a .PGM file
	write_pgm("Wally_ref_image.pgm", ARR2, Wally.getNumOfRows(), Wally.getNumOfColumbs(), 255);
	write_pgm("cluster_highlighting_wally.pgm", ARR, Cluster.getNumOfRows(), Cluster.getNumOfColumbs(), 255);
	delete ARR, ARR2;
	

	//calculates the time which the program has taken to run
	clock_t end = clock();
	double elapsed_secs = double(end - begin);
	std::cout << "Program completed in " << elapsed_secs << std::endl;
	
	
	//waits for user input
	std::cout << "would you like to find the N-best locations? [Y][N]" << std::endl;
	char answer;
	std::cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		std::cout << "how many locations would you like to return? (enter an integer value)" << std::endl;


		//saves user input to a variable
		int number_of_locations;
		std::cin >> number_of_locations;


		std::cout << "Getting N-best locations for wally" << std::endl;
		clock_t begin2 = clock();


		//calls the function which returns the possible locations, sorted by their NCC value
		std::vector<forSort> nBest_locations = find_nbest_locations(&Cluster, &Wally);


		//adds the number of locations determined by user input with the hight NCC values
		std::vector<forSort> nBestOutput;
		std::vector<std::vector<int>> nBest_cluster = *cluster_values_pointer;
		delete cluster_values_pointer;
		for (int i = nBest_locations.size() - 1; i > nBest_locations.size() - number_of_locations - 1; i--) { nBestOutput.push_back(nBest_locations[i]); }
		std::cout << std::endl;


		//Prints N-best locations to console and creates a seperate image with boxes highlighting these locations.
		for (int i = 0; i < nBestOutput.size(); i++) {
			std::cout << "Location " << i + 1 << " --> " << "X = " << nBestOutput[i].X << ": Y = " << nBestOutput[i].Y << std::endl;
		}

		std::cout << std::endl;

		//depending on user input, function calls the removeOverlap function and writes the output to a vector
		std::cout << "Would you like to eliminate all overlapping locations? This may reduce the number of N-best locations which are returned and drawn to the N-best output image [Y][N]" << std::endl;
		char input;
		std::cin >> input;
		if (input == 'Y' || input == 'y')
		{
			//calls the function to remove overlap from the vector of N-best locations
			nBestOutput = removeOverlap(&nBestOutput, wally);
			
			//Draws a box around the remaining positions
			for (int i = 0; i < nBestOutput.size(); i++)
			{
				nBest_cluster = createBox(nBest_cluster, &nBestOutput[i].Y, &nBestOutput[i].X);
			}
			std::cout << "remaining locations.." << std::endl;
			
				//prints N-best locations to console
				for (int i = 0; i < nBestOutput.size(); i++) {
				std::cout << "Location " << i + 1 << " --> " << "X = " << nBestOutput[i].X << ": Y = " << nBestOutput[i].Y << std::endl;
			}
		}

		//writes created image to a 1D array and then to a PGM file
		double *array = new double[Cluster.getNumOfColumbs() * Cluster.getNumOfRows()];
		array = convertToOneD(&nBest_cluster);
		write_pgm("N-best locations image.pgm", array, nBest_cluster.size(), nBest_cluster[0].size(), 255);

		delete array;
		//calculates time taken for the program to run
		std::cout << std::endl;
		clock_t end2 = clock();
		double timeTaken = double(end2 - begin2);
		std::cout << "program completed in " << timeTaken << std::endl;
	}

	std::cout << "Program ended" << std::endl;
	getchar();
	getchar();
	return 0;
}