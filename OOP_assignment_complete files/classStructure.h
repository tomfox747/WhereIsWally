#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <string>

double NCC(std::vector<std::vector<int>>&mat1, std::vector<std::vector<int>> &mat2);
double mean(std::vector<std::vector<int>> &mat1);
std::vector<std::vector<double>> minusMean(std::vector<std::vector<int>>&matrix, double &mean);
double multiplyMatrices(std::vector<std::vector<double>>&matrix1, std::vector<std::vector<double>>&matrix2);

class Matrix {
public:
	//gets the number of rows in a matrix object
	int getNumOfRows();
	//sets the number of rows in a matrix object
	void setNumOfRows(int numIn);


	//gets the number of columbs in a matrix object
	int getNumOfColumbs();
	//sets the number of columbs in a matrix object
	void setNumOfColumbs(int numIn);


	//gets the x position of a matrix object
	int get_positionX();
	//sets the x position of a matrix object
	void set_positionX(int numIn);


	//gets the y position of a matrix object
	int get_positionY();
	//sets the y position of a matrix object
	void set_positionY(int numIn);


	//returns the full matrix of a matrix object
	std::vector<std::vector<int>> getAllVals();


	//returns a the value at a specified point
	int getVal(int, int);


	//sets the full matrix of a matrix object
	void setVals(int&, int&, std::vector<std::vector<int>>& cluster);

	//matrix object constructors & destructor
	Matrix(int&, int&, std::vector<std::vector<int>>);
	Matrix(int x, int y, double nccVal);
	Matrix(std::string);
	Matrix();
	~Matrix();

	//returns the size of a matrix based on its inheritace position
	virtual void get_image_size();


	//compares and returns the SSD comparison value of a matrix object
	void get_SSD(std::vector<std::vector<int>>one, std::vector<std::vector<int>>two);


	//compares and returns the NNC comparison value of a matrix object
	double get_NCC(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2);
	double get_NCC();


	//sets the NCC of an object
	void set_NCC(double nccVal);


	//returns information about the applied matrix. Varies based on calling position
	virtual void get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2);

private:
	double Ncc;
	int positionX;
	int positionY;
	int numOfRows;
	int numOfColumbs;
	int image_size;
	std::vector<std::vector<int>> vals;
};

class base_image : public Matrix {
public:

	//returns matrix size
	void get_image_size();

	//returns information about the image
	void get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2);

	//base_image constructors & destructor
	base_image(std::string);
	base_image();
	~base_image();
};

class ref_image : public base_image {
public:

	//returns matrix size
	void get_image_size();

	//returns information about the image
	void get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2);

	//ref_image constructors and destructors
	ref_image(std::string);
	ref_image();
	~ref_image();
};

class large_image : public base_image {
public:

	//returns the matrix size
	void get_image_size();

	//returns information about the matrix
	void get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2);

	//large_image constructors & destructor
	large_image(std::string);
	large_image();
	~large_image();
};


//creates a structure of objects to be sorted and used as N-best location points
struct forSort {
	int X;
	int Y;
	double ncc;
	forSort(double NCC, int x, int y) : ncc(NCC), X(x), Y(y) {}
};


//uses operator overloading to compare "forSort" objects
struct ncc_comp {
	inline bool operator() (const forSort& struct1, const forSort& struct2) {
		return (struct1.ncc < struct2.ncc);
	}
};