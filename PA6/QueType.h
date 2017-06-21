//Amanda Kaufman
//CISS350
//Programming Assignment 6
//QueType class
//This class, borrowed and adapted from Programming Assignment 3, is used in Programming
//Assignment 6 to build a queue of items pulled from a binary search tree in a particular order.

#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstddef>	//For NULL
#include <new>		//For bad_alloc
#include "Suspect.h"
using namespace std;

//Exception class for when a client attempts to push an element onto a full queue
class FullQueue
{
};

//Exception class for when a client attempts to pop an element off an empty queue
class EmptyQueue
{
};

//Node structure for queue
struct QueNode
{
	Suspect info;
	QueNode *next;
};

//Queue implemented as a linked list.
class QueType
{
private:
	QueNode *front;
	QueNode *rear;
public:
	//Function: Class constructor.
	//Post: Front and rear are set to null.
	QueType()
	{
		front = NULL;
		rear = NULL;
	}

	//Function: Empty the queue.
	//Post: Queue is empty; all elements have been deallocated.
	void makeEmpty()
	{
		QueNode *tempPtr;

		while (front != NULL)
		{
			tempPtr = front;
			front = front->next;
			delete tempPtr;
		}
		rear = NULL;
	}

	//Function: Class destructor.
	//Post: Queue is empty; all elements have been deallocated.
	~QueType()
	{
		makeEmpty();
	}

	//Function: Observe an item in the list, without removing the item.
	//Post: A copy of the item at position index in the list is returned.
	Suspect getSuspect(int index)
	{
		Suspect temp;

		QueNode *tempPtr;

		tempPtr = front;
		for (int count = 0; count < index; count++)
		{
			tempPtr = tempPtr->next;
		}

		temp = tempPtr->info;
		return temp;
	}

	//Function: Adds a new item to the rear of the queue.
	//Pre: Queue has been initialized.
	//Post: If (queue is not full), newItem is at the rear of the queue; otherwise,
	//a FullQueue exception is thrown.
	void enqueue(Suspect newItem)
	{
		if (isFull())
			throw FullQueue();
		else
		{
			QueNode *newNode;

			newNode = new QueNode;
			newNode->info = newItem;
			newNode->next = NULL;
			if (rear == NULL)
				front = newNode;
			else
				rear->next = newNode;
			rear = newNode;
		}
	}

	//Function: Removes front item from the list and returns it in suspect reference variable.
	//Pre: Queue has been initialized.
	//Post: If (queue is not empty), the front of the queue has been removed and a copy returned
	//in suspect; otherwise, an EmptyQueue exception is thrown.
	void dequeue(Suspect &suspect)
	{
		if (isEmpty())
			throw EmptyQueue();
		else
		{
			QueNode *tempPtr;

			tempPtr = front;
			suspect = front->info;
			front = front->next;
			if (front == NULL)
				rear = NULL;
			delete tempPtr;
		}
	}

	//Function: Determine whether or not the queue is empty.
	//Post: Returns true if there are no elements on the queue and false otherwise.
	bool isEmpty() const
	{
		return (front == NULL);
	}

	//Function: Determine whether or not the queue is full.
	//Post: Returns true if there is no room for another QueNode object on the free store,
	//and false otherwise.
	bool isFull() const
	{
		QueNode *location;
		try
		{
			location = new QueNode;
			delete location;
			return false;
		}
		catch (std::bad_alloc exception)
		{
			return true;
		}
	}
};

