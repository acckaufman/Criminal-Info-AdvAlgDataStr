//Amanda Kaufman
//CISS350
//Programming Assignment 6
//Suspect class
//This class stores the name and a list of known details about a suspect.

#pragma once
#include <string>
#include <fstream>
#include "UnsortedType.h"	//For list of attributes
using namespace std;

class Suspect
{
private:
	string name;					//Name of the suspect
	UnsortedType attributes;		//Linked list of attributes
public:
	//###################################################################################
	//Constructors																		#
	//###################################################################################
	//Default constructor
	Suspect()
	{
		name = "";
	}

	//Overloaded constructor that accepts a name
	Suspect(string s)
	{
		name = s;
	}

	//Destructor
	~Suspect()
	{
		attributes.~UnsortedType();
	}

	//###################################################################################
	//Accessor functions																#
	//###################################################################################

	//Returns the suspect's name.
	string getName() const
	{
		return name;
	}

	//Returns the list of the suspect's attributes.
	UnsortedType getAttributes() const
	{
		return attributes;
	}

	//Returns true if the details array contains a detail string that matches s, false otherwise.
	bool hasAttributeMatch(string s)
	{
		bool match = false;

		string temp;

		//Reset the attributes list
		attributes.resetList();

		//Go through each attribute in the list and determine if it matches the parameter.
		for (int index = 0; index < attributes.getLength(); index++)
		{
			temp = attributes.getNextItem();
			if (temp == s)
				match = true;
		}

		return match;
	}

	//Writes a suspect object to an output file, properly formatted for future processing.
	void writeToFile(ofstream &outFile)
	{
		string tempString;		//To hold attribute strings for output
		outFile << name << "$";	//Write the name to the file
		attributes.resetList();	//Reset the attribute list for processing
		for (int index = 0; index < attributes.getLength(); index++)
		{
			tempString = attributes.getNextItem();	//Write the attribute to the file
			outFile << tempString << "$";			//Separate attributes with the delimiter
		}
		outFile << endl;	//Newline at the end of the record
	}

	//###################################################################################
	//Mutator functions																	#
	//###################################################################################

	//Sets the suspect's name.
	void setName(string s)
	{
		name = s;
	}

	//Adds the string passed to the function to the list of the suspect's attributes.
	void addAttribute(string s)
	{
		attributes.putItem(s);
	}

	//###################################################################################
	//Overloaded operators																#
	//###################################################################################

	//Overloaded == operator
	bool operator == (const Suspect &right)
	{
		bool match = true;	//Flag

		string rightName = right.name;	//Temporary holder for comparison object's name

		for (int index = 0; index < name.length(); index++)
		{
			if (toupper(rightName.at(index)) == toupper(name.at(index)))
				match = true;
			else
				match = false;
		}

		return match;	//Return flag
	}

	//Overloaded < operator
	bool operator < (const Suspect &right)
	{
		bool less = false;	//Flag

		string rightName = right.name;	//Temporary holder for comparison object's name
		
		for (int index = 0; index < name.length(); index++)	//Keeps executing as long as values are equal in string.
		{
			if (toupper(name.at(index)) < toupper(right.name.at(index)))
			{
				less = true;			//If a character is found that is less than right side object, return true.
				index = name.length();	//Exit the for loop.
			}
			else if (toupper(name.at(index)) > toupper(right.name.at(index)))
			{
				less = false;			//If a character is found that is greater than right side object, return false.
				index = name.length();	//Exit the for loop.
			}
		}

		return less;	//Return flag
	}

	//Overloaded > operator
	bool operator > (const Suspect &right)
	{
		bool more = false;	//Flag

		string rightName = right.name;	//Temporary holder for comparison object's name

		for (int index = 0; index < name.length(); index++)	//Keeps executing as long as values are equal in string.
		{
			if (toupper(name.at(index)) > toupper(right.name.at(index)))
			{
				more = true;			//If a character is found that is greater than right side object, return true.
				index = name.length();	//Exit the for loop.
			}
			else if (toupper(name.at(index)) < toupper(right.name.at(index)))
			{
				more = false;			//If a character is found that is less than right side object, return false.
				index = name.length();	//Exit the for loop.
			}
		}

		return more;	//Return flag
	}

	//Overloaded = operator
	void operator = (const Suspect &right)
	{
		name = right.name;				//Copy name
		attributes = right.attributes;	//Use overloaded = operator from UnsortedType
	}

	//###################################################################################
	//Friends																			#
	//###################################################################################

	friend ostream &operator << (ostream &, const Suspect &);
	friend istream &operator >> (istream &, Suspect &);
};

//###################################################################################
//Overloaded stream operators														#
//###################################################################################

//Overloaded << operator for output
ostream &operator << (ostream &strm, const Suspect &object)
{
	strm << "Name: " << object.getName() << endl;
	strm << "Details:" << endl;
	UnsortedType list = object.getAttributes();
	strm << list << endl;

	return strm;
}

//Overloaded >> operator for input
istream &operator >> (istream &strm, Suspect &object)
{
	//Prompt the user for the suspect's name.
	cout << "Name: ";
	getline(strm, object.name, '\n');

	int count = 1;
	string attribute;
	char another = 'n';

	do {
		//Get input from the user and store in the attributes list.
		cout << "Detail #" << count << ": ";
		getline(strm, attribute, '\n');
		object.attributes.putItem(attribute);
		count++;

		//Ask if the user wants to enter another attribute.
		cout << "Do you want to enter another attribute? (y/n): ";
		strm >> another;
		strm.ignore();
	} while (another == 'y' || another == 'Y');

	return strm;
}