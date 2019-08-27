#include <iostream>
#include <vector>
#include <math.h>
#include "classStructure.h"

//compares matrix objects to a reference image using the NCC method. 
int NCCsearch(std::vector<std::vector<int>>*m1, std::vector<Matrix> &VECT, std::vector<std::vector<int>>*cds)
{
	std::vector<std::vector<int>>coOrds = *cds;
	std::vector<std::vector<int>> mat1 = *m1;
	std::vector<double> values;
	std::vector<std::vector<int>> v;
	double val;
	values.reserve(1200);
	int location = 0;
	for (int x = 0; x < VECT.size(); x++)
	{
		v = VECT[x].getAllVals(); //creates a matrix copy for each matrix object
		val = NCC(mat1, v); //calculates the NCC value for each matrix comparison against the reference image
		values.push_back(val); // adds the NCC value to a vector
	}
	double maxVal = -1;

	//iterates through the lise of NCC values.
	//Using comparisons it finds the largest item and saves the location into a variable
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i] > maxVal)
		{
			maxVal = values[i];
			location = i;
		}
	}
	std::cout << "largest NCC value = " << maxVal << std::endl;

	return location;
}

//calculates the NCC value of two matrices
double NCC(std::vector<std::vector<int>> &mat1, std::vector<std::vector<int>> &mat2)
{
	//calls the function to calculate the mean of each matrix
	double mean1 = mean(mat1);
	double mean2 = mean(mat2);


	//calls the function to find the difference between each object in the matrix and the matrix mean
	//this data is then saved to another vector
	std::vector<std::vector<double>> I = minusMean(mat1, mean1);
	std::vector<std::vector<double>> T = minusMean(mat2, mean2);


	//calls the function to multiply each of the matrices together
	double item1 = multiplyMatrices(I, T);
	double item2 = multiplyMatrices(I, I);
	double item3 = multiplyMatrices(T, T);


	//Applies item1, item2 and item3 to the NCC equation to calculate the NCC value of the comparison
	double output = item1 / (sqrt(item2 * item3));
	return output;
}


//multiplies two matrices together
double multiplyMatrices(std::vector<std::vector<double>>&matrix1, std::vector<std::vector<double>>&matrix2)
{
	std::vector<std::vector<double>> output;
	std::vector<double> line;

	//multiplies each of the values from two matrices together and saves them to a matrix
	for (int i = 0; i < matrix1.size(); i++)
	{
		for (int I = 0; I < matrix1[0].size(); I++)
		{
			line.push_back(matrix1[i][I] * matrix2[i][I]);
		}
		output.push_back(line);
		line.clear();
	}

	//calculates the sum of the resulting matrix
	double total = 0;
	for (int i = 0; i < output.size(); i++)
	{
		for (int I = 0; I < output[0].size(); I++)
		{
			total = total + output[i][I];
		}
	}
	return total;

}


//calculates the difference between each value in a matrix and the matrix mean. Saves the values to another matrix
std::vector<std::vector<double>> minusMean(std::vector<std::vector<int>>&matrix, double &mean)
{
	std::vector<double> line;
	std::vector<std::vector<double>> output;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int I = 0; I < matrix[0].size(); I++)
		{
			line.push_back(matrix[i][I] - mean);
		}
		output.push_back(line);
		line.clear();
	}
	return output;
}


//calculates the mean of a matrix
double mean(std::vector<std::vector<int>> &mat1)
{
	double total = 0;
	double itemCount = 0;
	double output = 0;
	for (int i = 0; i < mat1.size(); i++)
	{
		for (int I = 0; I < mat1[0].size(); I++)
		{
			total = total + mat1[i][I];
			itemCount++;
		}
	}
	output = total / itemCount;
	return output;
}