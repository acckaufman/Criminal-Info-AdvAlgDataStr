//Amanda Kaufman
//CISS350
//Programming Assignment 6
//This program is a search tool to help a police department conduct inquries on possible
//suspects of a crime, based on names and tippers.

#include <iostream>
#include <sstream>
#include "TreeType.h"
#include "Suspect.h"
using namespace std;

//Function prototypes
void mainMenu(ofstream &);
void addSuspect(ofstream &);
void inquiryMenu(ofstream &);
void loadData(TreeType &, ofstream &);
void conductInquiry(ofstream &);
void tipSearch(TreeType &, ofstream &);
void checkName(TreeType &, ofstream &);

int main()
{
	//Open testing output file - echo prints user commands and PRINT commands.
	ofstream outFile;
	outFile.open("Criminal.trn", ios::out);

	//Program Title
	cout << "Crime Suspect Search Tool\n\n";
	outFile << "Crime Suspect Search Tool\n\n";

	//To ask the user if they're sure they want to quit the program
	char sure = 'n';

	//Run the main program as long as the user command is not QUIT.
	do
	{
		string mainInput;				//String for user input from main menu
		mainMenu(outFile);				//Display main menu
		getline(cin, mainInput, '\n');	//Get user's command
		outFile << mainInput << endl;	//Output user's command to output file

		if (mainInput == "ADD")
		{
			addSuspect(outFile);
		}

		else if (mainInput == "INQUIRY")
		{
			conductInquiry(outFile);
		}

		else if (mainInput == "QUIT")
		{
			cout << "\nAre you sure you want to quit? (y/n): ";
			outFile << "\nAre you sure you want to quit? (y/n): ";
			cin >> sure;
			outFile << sure << endl;
			cin.ignore();
			cout << endl;
		}

		else
		{
			cout << "\nError!  Command not found.  Please try again.\n";
			outFile << "\nError!  Command not found.  Please try again.\n";
			system("pause");
			cout << endl;
		}

	} while (sure != 'y' && sure != 'Y');
	
	//Close output file.
	outFile.close();

	system("pause");

	return 0;
}

//Displays main menu
void mainMenu(ofstream &outFile)
{
	cout << "Main Menu:\n\n";
	cout << "Enter one of the following commands:\n\n"
		<< "\tADD: Add a suspect to the list.\n"
		<< "\tINQUIRY: Conduct an inquiry.\n"
		<< "\tQUIT: Save and quit the program.\n\n";
	outFile << "Main Menu:\n\n";
	outFile << "Enter one of the following commands:\n\n"
		<< "\tADD: Add a suspect to the list.\n"
		<< "\tINQUIRY: Conduct an inquiry.\n"
		<< "\tQUIT: Save and quit the program.\n\n";
}

//Adds a suspect to the data file and the tree
void addSuspect(ofstream &outFile)
{
	//Open the dataFile for output in append mode.
	ofstream appendFile;
	appendFile.open("Criminal.mf", ios::app);

	//Create temp Suspect object for user input.
	Suspect tempSuspect;

	//Get the user's input.
	cout << "\nAdd New Suspect\n\n";
	outFile << "\nAdd New Suspect\n\n";
	cin >> tempSuspect;

	//Write the tempSuspect information to the data file.
	tempSuspect.writeToFile(appendFile);

	//Close the data file.
	appendFile.close();

	//Message to user.
	cout << "New suspect added to list.\n\n";
	outFile << "New suspect added to list.\n\n";
}

//Displays the menu for an inquiry
void inquiryMenu(ofstream &outFile)
{
	cout << "Enter one of the following commands:\n\n"
		<< "\tTIP: Search suspects by attribute.\n"
		<< "\tCHECK: Search suspects by name.\n"
		<< "\tPRINT: Print set of active suspects.\n"
		<< "\tSTOP: Ends the current inquiry.\n\n";
	outFile << "Enter one of the following commands:\n\n"
		<< "\tTIP: Search suspects by attribute.\n"
		<< "\tCHECK: Search suspects by name.\n"
		<< "\tPRINT: Print set of active suspects.\n"
		<< "\tSTOP: Ends the current inquiry.\n\n";
}

//Loads data from the data file and stores it in the TreeType object.
void loadData(TreeType &Criminals, ofstream &outFile)
{
	//Open a file that contains retained criminal information.
	ifstream dataFile;
	dataFile.open("Criminal.mf", ios::in);

	cout << "\nNew Inquiry:\n\n";
	outFile << "\nNew Inquiry:\n\n";

	cout << "Populating data...Please wait.\n";
	outFile << "Populating data...Please wait.\n";

	//Temporary variable for use in input processing.
	string tempName;

	//Attempt to read the first suspect's name from the file.
	getline(dataFile, tempName, '$');

	//While the file read operation was successful, continue
	while (dataFile)
	{
		//Variables for use in file processing
		Suspect tempSuspect;
		stringstream recordString;
		string list;
		string attribute;

		//Store the name in the temporary Suspect object.
		tempSuspect.setName(tempName);

		//Convert the rest of the line into a stringstream object for processing the attribute list.
		getline(dataFile, list, '\n');
		recordString.str(list);

		//Get the first item from the attribute list.
		getline(recordString, attribute, '$');

		//For each attribute, add it to the temp suspect's list.
		while (attribute != "")
		{
			tempSuspect.addAttribute(attribute);
			getline(recordString, attribute, '$');
		}

		//Store the tempSuspect object in the tree.
		Criminals.putItem(tempSuspect);

		//Attempt to read the next suspect's name.
		getline(dataFile, tempName, '$');
	}

	//Debugging: Output the criminal information that was loaded.
	//cout << endl;
	//cout << "Current List of Suspects:\n\n";
	//Criminals.print(outFile);

	//Close dataFile.
	dataFile.close();

	cout << "Done.\n\n";
}

//Allows the user to conduct an inquiry using TIP, CHECK, and PRINT commands.
void conductInquiry(ofstream &outFile)
{
	//Build a binary search tree to hold retained suspect information.
	TreeType Criminals;

	//Load stored information from a file into the binary search tree
	loadData(Criminals, outFile);

	//Get a code name for this inquiry
	string codeName;
	cout << "Code name: ";
	outFile << "Code name: ";
	getline(cin, codeName, '\n');
	outFile << codeName << endl;
	cout << endl;

	//To ask the user if they're sure they want to stop the inquiry
	char stopInquiry = 'n';

	//Repeat the inquiry menu as long as user command is not STOP
	do
	{
		string inquiryInput;				//String for user input from inquiry menu
		inquiryMenu(outFile);				//Display the inquiry menu
		getline(cin, inquiryInput, '\n');	//Get the user's command
		outFile << inquiryInput << endl;	//Output user's command to output file

		//Search through the tree and delete suspects that do not have the tip attribute.
		if (inquiryInput == "TIP")
		{
			tipSearch(Criminals, outFile);
		}

		else if (inquiryInput == "CHECK")
		{
			checkName(Criminals, outFile);
		}
		
		else if (inquiryInput == "PRINT")
		{
			cout << "\nCurrent Suspect List:\n\n";
			outFile << "\nCurrent Suspect List:\n\n";
			Criminals.print(outFile);
			system("pause");
			cout << endl;
		}
		
		else if (inquiryInput == "STOP")
		{
			cout << "\nAre you sure you want to stop this inquiry? (y/n): ";
			outFile << "\nAre you sure you want to stop this inquiry? (y/n): ";
			cin >> stopInquiry;
			outFile << stopInquiry << endl;
			cin.ignore();
			cout << endl;
		}
		
		else
		{
			cout << "\nError!  Command not found.  Please try again.\n";
			outFile << "\nError!  Command not found.  Please try again.\n";
			system("pause");
			cout << endl;
		}

	} while (stopInquiry != 'y' && stopInquiry != 'Y');
}

void tipSearch(TreeType &Criminals, ofstream &outFile)
{
	string tipInfo;			//User input for tip information
	bool finished = false;	//Flag for finished tree search

	cout << "Tip Info: ";
	outFile << "Tip Info: ";
	getline(cin, tipInfo, '\n');
	outFile << tipInfo << endl;

	//Reset the tree for traversal in order
	Criminals.resetTree(IN_ORDER);

	while (finished == false)
	{
		Suspect checkSuspect;			//Temporary holder for suspect to be checked
		checkSuspect = Criminals.getNextItem(IN_ORDER, finished);	//Get the next item in the tree

																	//If the subject does not have any matching fields related to this tip, delete it.
		if (!(checkSuspect.hasAttributeMatch(tipInfo)))
			Criminals.deleteItem(checkSuspect);
	}

	cout << "\nTip processed. Type PRINT to see possible suspects.\n\n";
	outFile << "\nTip processed. Type PRINT to see possible suspects.\n\n";
	system("pause");
}

void checkName(TreeType &Criminals, ofstream &outFile)
{
	string checkName;		//Name to be checked, input by the user
	Suspect checkSuspect;	//Temporary holder for comparing suspects
	bool finished = false;	//Flag for finished tree search
	bool found = false;		//Flag for found matching suspect

	cout << "Name: ";
	outFile << "Name: ";
	getline(cin, checkName, '\n');
	outFile << checkName << endl;

	//Reset the list for inorder traversal.
	Criminals.resetTree(IN_ORDER);

	while (finished == false)
	{
		//Get each item from the tree and check its name against the string input by the user.
		checkSuspect = Criminals.getNextItem(IN_ORDER, finished);
		if (checkSuspect.getName() == checkName)
		{
			cout << "\nSuspect " << checkName << " found in list.\n\n";
			outFile << "\nSuspect " << checkName << " found in list.\n\n";
			found = true;

			system("pause");
			cout << endl;
		}
	}

	//If the suspect was not found, inform the user.
	if (found == false)
	{
		cout << "\nSuspect " << checkName << " not found in list.\n\n";
		outFile << "\nSuspect " << checkName << " not found in list.\n\n";
		system("pause");
		cout << endl;
	}
}