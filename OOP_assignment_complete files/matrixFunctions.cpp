#include <iostream>
#include <vector>
#include <string>
#include "classStructure.h"

std::vector<std::vector<int>> readInFiles(std::string input);
double NCC(std::vector<std::vector<int>> &mat1, std::vector<std::vector<int>> &mat2);


//primary matrix constructor, used to create the matrices to find the ref_image
Matrix::Matrix(int &L, int &C, std::vector<std::vector<int>> cluster) {
	setVals(L, C, cluster);
	set_positionX(L);
	set_positionY(C);
	setNumOfColumbs(36);
	setNumOfRows(49);
}

//overload can be used to convert the matrix object to a forSort object or to simply get primary information
Matrix::Matrix(int x, int y, double nccVal) {
	set_positionX(x);
	set_positionY(y);
	set_NCC(nccVal);
}

//overload used to create image objects
Matrix::Matrix(std::string input) {
	vals = readInFiles(input);
	setNumOfRows(vals.size());
	setNumOfColumbs(vals[0].size());
	set_positionX(0);
	set_positionY(0);
}

//default constructor
Matrix::Matrix() {}

//destructor
Matrix::~Matrix() {}


//returns a value at a specified postition
int Matrix::getVal(int row, int col) { return vals[row][col]; }


//returns the full matrix
std::vector<std::vector<int>> Matrix::getAllVals() { return vals; }


//returns the size of the matrix
void Matrix::get_image_size() {
	std::cout << "size of matrix = " << getNumOfColumbs() * getNumOfRows() << std::endl;
}


//sets the matrix values of the object using the size of the reference image and the co-ordinate position
void Matrix::setVals(int &L, int &C, std::vector<std::vector<int>> &cluster) {
	std::vector<int> LIN;
	LIN.reserve(1200);
	int startLine = L;
	int startCol = C;
	for (int i = startLine; i < startLine + 49; i++) {
		for (int I = startCol; I < startCol + 36; I++) {
			LIN.push_back(cluster[i][I]);
		}
		vals.push_back(LIN);
		LIN.clear();
	}
}

int Matrix::getNumOfRows() { return numOfRows; }
void Matrix::setNumOfRows(int numIn) { numOfRows = numIn; }
int Matrix::getNumOfColumbs() { return numOfColumbs; }
void Matrix::setNumOfColumbs(int numIn) { numOfColumbs = numIn; }
int Matrix::get_positionX() { return positionX; }
void Matrix::set_positionX(int numIn) { positionX = numIn; }
int Matrix::get_positionY() { return positionY; }
void Matrix::set_positionY(int numIn) { positionY = numIn; }


//full SSD function for comparing 2 2d matrices
void Matrix::get_SSD(std::vector<std::vector<int>>one, std::vector<std::vector<int>>two) {
	int totalOne = 0;
	int totalTwo = 0;
	int total = 0;

	//loops through each item in the matrix pair
	for (int i = 0; i < one.size(); i++) {
		for (int I = 0; I < one[0].size(); I++) {

			//return difference between values
			int val = one[i][I] - two[i][I];

			//if the difference id below 0. Convert to a positive number
			if (val < 0)
			{
				val = val - (2 * val);
			}

			//add output to total
			total = total + val;
		}
	}

	//Print total to console
	std::cout << "SSD comparison with reference = " << total << std::endl;
}

//uses pre-written NCC algorithm to save NCC value to object
double Matrix::get_NCC(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2) {
	Ncc = NCC(mat1, mat2);
	return Ncc;
}


//returns NCC value of object
double Matrix::get_NCC() { return Ncc; }


//overload for use if the NCC has already been calculated
void Matrix::set_NCC(double nccVal) { Ncc = nccVal; }


//returns various information about the object
void Matrix::get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2) {
	std::cout << std::endl;
	std::cout << "==== Matrix information ====" << std::endl;
	std::cout << "X co-ordinate = " << positionX << " : " << "Y co-ordinate = " << positionY << std::endl;
	get_image_size();
	std::cout << "Number of Columns = " << getNumOfColumbs() << std::endl;
	std::cout << "Number of Rows = " << getNumOfRows() << std::endl;
	get_SSD(mat1, mat2);
	std::cout << "NCC value = " << get_NCC(mat1, mat2) << std::endl;
	std::cout << std::endl;
}


