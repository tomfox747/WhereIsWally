#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "classStructure.h"

//primary contructor, default constructor and destructor for base_image class
base_image::base_image(std::string filename) : Matrix(filename) {}
base_image::base_image() {};
base_image::~base_image() {};

//prints the size of the base_image matrix to the console
void base_image::get_image_size() { std::cout << "size of image = " << getNumOfColumbs() * getNumOfRows() << std::endl; }

//prints various information about the base_image object to the console
void base_image::get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2)
{
	std::cout << std::endl;
	std::cout << "Image type = Base image" << std::endl;
	get_image_size();
	std::cout << "Number of Columns = " << getNumOfColumbs();
	std::cout << std::endl;
	std::cout << "Number of Rows = " << getNumOfRows();
	std::cout << std::endl;
}


//primary contructor, default constructor and destructor for ref_image class
ref_image::ref_image(std::string fileName) : base_image(fileName) {}
ref_image::ref_image() {};
ref_image::~ref_image() {};

//prints the size of the ref_image matrix to the console
void ref_image::get_image_size() { std::cout << "size of referance image = " << getNumOfColumbs() * getNumOfRows() << std::endl; };

//prints various information about teh ref_image object to the console
void ref_image::get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2)
{
	std::cout << std::endl;
	std::cout << "Image type = Reference image of Wally" << std::endl;
	get_image_size();
	std::cout << "Number of Columns = " << getNumOfColumbs();
	std::cout << std::endl;
	std::cout << "Number of Rows = " << getNumOfRows();
	std::cout << std::endl;
}


//primary contructor, default constructor and destructor for large_image class
large_image::large_image(std::string fileName) : base_image(fileName) {}
large_image::large_image() {};
large_image::~large_image() {};

//prints the size of the large_image matrix to the console
void large_image::get_image_size() { std::cout << "size of cluster image = " << getNumOfColumbs() * getNumOfRows() << std::endl; };

//prints various information about the large_image object to the console
void large_image::get_info(std::vector<std::vector<int>> mat1, std::vector<std::vector<int>> mat2)
{
	std::cout << std::endl;
	std::cout << "Image type = Large cluster image containing reference" << std::endl;
	get_image_size();
	std::cout << "Number of Columns = " << getNumOfColumbs();
	std::cout << std::endl;
	std::cout << "Number of Rows = " << getNumOfRows();
	std::cout << std::endl;
}