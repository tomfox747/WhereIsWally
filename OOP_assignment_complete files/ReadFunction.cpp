#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "classStructure.h"


//reads in a set of characters from a specified text file into a vector
std::vector<std::vector<int>> readInFiles(std::string input)
{
	int percent = 0;
	std::ifstream file;
	std::vector<std::vector<int>> matrix;
	matrix.reserve(1500);
	file.open(input, std::ios::in); // opens specified file

	char my_character;
	std::vector<int> currentVector;
	currentVector.reserve(1500);
	std::vector<int> *curVect = &currentVector;

	if (file.is_open()) //checks file is open
	{
		file.get(my_character); //gets the first value in the file
		while (!file.eof()) // runs until the end character is hit
		{
			if (my_character == ' ') // searches for spaces in the text file
			{
				for (int i = 0; i < 2; i++) //skips 2 characters
				{
					file.get(my_character);
				}
				float itemVal = (int)my_character - 48; //converts from ascii value and saves the current character to a float 
				for (int i = 0; i < 2; i++) // skips 2 characters
				{
					file.get(my_character);
				}
				itemVal = itemVal + ((my_character - 48) *0.1); //converts from ascii, divide by 10 and saves current character to a float
				file.get(my_character);//skips a single character
				itemVal = itemVal + ((my_character - 48) *0.01);//converts from ascii, divide by 100 and saves current character to a float
				for (int i = 0; i < 10; i++)
				{
					file.get(my_character);//skips 10 characters
				}
				if (my_character == '2') //checks the multiple of 10 of the int value within the text file and multiplies accordingly
				{
					itemVal = itemVal * 100;
				}
				else
				{
					itemVal = itemVal * 10;
				}
				currentVector.emplace_back(itemVal); //adds the value to a vector
			}
			else if (my_character == '\n')//if character is \n current vector is added as an element to a 2d vector and then cleared
			{
				matrix.emplace_back(*curVect);
				currentVector.clear();
			}

			file.get(my_character); // gets the next character in the text file
		}
	}
	else // exeption handling, in the event of the file not opening this error message will show 
	{
		std::cout << "file did not open correctly";
	}
	file.close(); //closes the file
	return matrix;
}