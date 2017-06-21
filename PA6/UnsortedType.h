//Amanda Kaufman
//CISS350
//Programming Assignment 6
//UnsortedType class
//This class, borrowed and adapted from Programming Assignment 2, is used in Programming
//Assignment 6 to contain a list of known details about a suspect.

#pragma once
#include <string>
#include <new>	//Needed to catch bad_alloc exception
using namespace std;

//Define the NodeType structure for use in this list.
struct NodeType
{
	string info;
	NodeType* next;
};

class UnsortedType
{
private:
	NodeType* listData;
	int length;
	NodeType* currentPos;
public:
	//Constructor initializes the length of the list and the head pointer.
	UnsortedType()
	{
		length = 0;
		listData = nullptr;
	}

	//Destructor makes the list empty, freeing up memory for other applications.
	~UnsortedType()
	{
	}

	//Empties the list and deallocates all items.
	void makeEmpty()
	{
		NodeType *tempPtr;

		while (listData != NULL)
		{
			tempPtr = listData;
			listData = listData->next;
			delete tempPtr;
		}

		length = 0;
	}

	//Returns true if there is no room for another node on the free store; false otherwise.
	bool isFull() const
	{
		NodeType *location;

		try
		{
			location = new NodeType;
			delete location;
			return false;
		}
		catch (std::bad_alloc exception)
		{
			return true;
		}
	}

	//Returns the number of items in the list.
	int getLength() const
	{
		return length;
	}

	//Returns currentPos
	NodeType* getCurrentPos() const
	{
		return currentPos;
	}

	//If the object passed as a parameter matches an object in the list, matching item is returned.
	//Otherwise, object passed as a parameter is returned.
	//Note: This function assumes that the object passed to the function has been initialized.
	string getItem(string item, bool& found)
	{
		bool moreToSearch;
		NodeType *location;

		location = listData;
		found = false;
		moreToSearch = (location != NULL);

		while (moreToSearch && !found)
		{
			if (item == location->info)
			{
				found = true;
				item = location->info;
			}
			else
			{
				location = location->next;
				moreToSearch = (location != NULL);
			}
		}

		return item;
	}

	//Inserts an item into the list and increments the list length.
	void putItem(string item)
	{
		NodeType *location;			//Declare a pointer to a node

		location = new NodeType;	//Allocate a new node
		location->info = item;		//Store the item in the node
		location->next = listData;	//Store address of first node in next field of new node
		listData = location;		//Store address of new node into external pointer
		length++;					//Increment the length of the list
	}

	//Deletes a node from the list.
	//Note: this function assumes that the object passed as a parameter exists in the list,
	//based on a matching key.  It also assumes the object passed as a parameter was initialized.
	void deleteItem(string item)
	{
		NodeType *location = listData;
		NodeType *tempLocation;

		//Locate node to be deleted.
		//If the first node matches the parameter, delete the first node.
		if (item == listData->info)
		{
			tempLocation = location;
			listData = listData->next;
		}
		//Otherwise, search the list for the node and delete it.
		else
		{
			while (item != (location->next)->info)
				location = location->next;

			//Once found, delete the node at location->next;
			tempLocation = location->next;	//Store address of the node to be deleted
			location->next = (location->next)->next;	//Move the pointer from the previous node to the one after
		}

		delete tempLocation;	//Delete the node
		length--;	//Decrement the length of the list
	}
	
	//Initialize current position at the start of the list.
	void resetList()
	{
		currentPos = NULL;
	}

	//Returns a copy of the next item in the list.
	//When the end of the list is reached, currentPos is reset to the beginning of the list.
	string getNextItem()
	{
		if (currentPos == NULL)		//If currentPos is pointing to the end of the list
			currentPos = listData;	//reset it at the beginning of the list
		else
			currentPos = currentPos->next;	//Otherwise, move currentPos to the next list item
		return currentPos->info;			//Return the information stored in the next list item
	}

	//Overloaded = operator
	void operator = (const UnsortedType &right)
	{
		//Copy the length member
		length = right.length;

		NodeType *ptr1;
		NodeType *ptr2;

		if (right.listData == NULL)
			listData = NULL;
		else
		{
			listData = new NodeType;
			listData->info = right.listData->info;
			//cout << "listData->info = " << listData->info << endl;				//Debugging
			//cout << "right.listData->info = " << right.listData->info << endl;	//Debugging
			ptr1 = right.listData->next;
			ptr2 = listData;

			while (ptr1 != NULL)
			{
				ptr2->next = new NodeType;
				ptr2 = ptr2->next;
				ptr2->info = ptr1->info;
				//cout << "ptr2->info = " << ptr2->info << endl;					//Debugging
				//cout << "ptr1->info = " << ptr1->info << endl;					//Debugging
				ptr1 = ptr1->next;
			}

			ptr2->next = NULL;
		}
	}

	//Friends
	friend ostream &operator << (ostream &, UnsortedType &);
};

//Overloaded << operator
ostream &operator << (ostream &strm, UnsortedType &list)
{
	string temp;		//Temporary string for output
	list.resetList();	//Reset the current position in the attribute list

	for (int count = 0; count < list.getLength(); count++)
	{
		temp = list.getNextItem();		//Get the next string item from the list
		strm << temp << endl;			//Output the item to the stream object
	}

	return strm;
}