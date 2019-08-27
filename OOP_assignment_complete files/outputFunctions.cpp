#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "classStructure.h"


//creates a black border of 2 pixels wide around a section in the large_image to highlight a location
std::vector<std::vector<int>> createBox(std::vector<std::vector<int>> &matrix, int *row, int *col)
{
	int rowNum = *row;
	int colNum = *col;
	//these IF statements check that the co-ordinates are within the required boundaries and can therefore be used to create a box
	if (rowNum < 2)
	{
		rowNum = 2;
	}
	if (rowNum > 717)
	{
		rowNum = 717;
	}
	if (colNum < 2)
	{
		colNum = 2;
	}
	if (colNum > 986)
	{
		colNum = 986;
	}
	int boxWidth = 36;
	int boxHeight = 49;
	int setVal = 0; // this is the value which individual pixels are changed to (black)

	//Draws the top line of the box
	for (int i = -2; i < boxWidth + 2; i++)
	{
		matrix[rowNum - 1][i + colNum] = setVal;
		matrix[rowNum - 2][i + colNum] = setVal;
	}

	//Draws the left and right lines of the box
	for (int i = 0; i < boxHeight + 2; i++)
	{
		matrix[i + rowNum][colNum - 2] = setVal;
		matrix[i + rowNum][colNum - 1] = setVal;
		matrix[i + rowNum][colNum + boxWidth] = setVal;
		matrix[i + rowNum][colNum + boxWidth + 1] = setVal;
	}

	//Draws the bottom line of the box
	for (int i = -2; i < boxWidth + 2; i++)
	{
		matrix[rowNum + boxHeight + 1][colNum + i] = setVal;
		matrix[rowNum + boxHeight + 2][colNum + i] = setVal;
	}

	return matrix;
}


//converts a 2d vector into a 1d array
double* convertToOneD(std::vector<std::vector<int>> *mat)
{
	std::vector<std::vector<int>> matrix = *mat;
	double *arr = new double[786432];
	int counter = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int I = 0; I < matrix[0].size(); I++)
		{
			arr[counter] = static_cast<double>(matrix[i][I]);
			counter++;
		}
	}
	return arr;
}

// Converts a 1D array of doubles of size R*C to .pgm image of R rows and C Columns 
// and stores .pgm in filename
// Use Q = 255 for greyscale images and 1 for binary images.
void write_pgm(std::string filename, double *data, int sizeR, int sizeC, int Q)
{
	int i, j;
	unsigned char *image;
	std::ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	// convert the integer values to unsigned char

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!myfile) {
		std::cout << "Can't open file: " << filename << std::endl;
		exit(1);
	}

	myfile << "P5" << std::endl;
	myfile << sizeC << " " << sizeR << std::endl;
	myfile << Q << std::endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC) * sizeof(unsigned char));

	if (myfile.fail()) {
		std::cout << "Can't write image " << filename << std::endl;
		exit(0);
	}

	myfile.close();
	delete[] image;
}

//takes a vector of forSort objects, checks the boundaries of each item and compares all items within the same boundaries
//only the item with the highest NCC within each set of overlaps is retained. The rest are deleted.
std::vector<forSort> removeOverlap(std::vector<forSort> *mats, ref_image *wal)
{
	ref_image wally = *wal;
	std::vector<forSort> matrices = *mats;
	std::vector<forSort> output;
	for (int i = 0; i < matrices.size(); i++) {
		output.push_back(matrices[i]);
	}

	for (int i = 0; i < output.size(); i++) {
		for (int I = i + 1; I < output.size(); I++) {
			if (output[I].X - output[i].X < 36 && output[I].X - output[i].X > -36) {
				if (output[I].Y - output[i].Y < 49 && output[I].Y - output[i].Y > -49) {
					if (output[i].ncc < output[I].ncc) {
						output.erase(output.begin() + 0);
						I = I - 1;
					}
					else {
						output.erase(output.begin() + I);
						I = I - 1;
					}
				}
			}

		}

	}
	return output;
}
