//Amanda Kaufman
//CISS350
//Programming Assignment 6
//TreeType class
//This class is a binary search tree of Suspect items.

#pragma once
#include <new>
#include <iostream>
#include "Suspect.h"
#include "QueType.h"

//Structure for each node in the tree
struct TreeNode
{
	Suspect info;
	TreeNode *left;
	TreeNode *right;
};

//Enum type to determine order for traversal
enum OrderType {PRE_ORDER, IN_ORDER, POST_ORDER};

//TreeType class
class TreeType
{
private:
	//Attribute(s)
	TreeNode *root;			//Root of the tree
	QueType preOrderQue;	//Preorder queue
	QueType inOrderQue;		//In-Order queue
	QueType postOrderQue;	//Postorder queue

	//Helper Functions

	//Recursive helper function for getLength().
	int countNodes(TreeNode *tree)
	{
		if (tree == NULL)
			return 0;
		else
			//Return the number of nodes in each subtree plus 1 for this node.
			return countNodes(tree->left) + countNodes(tree->right) + 1;
	}

	//Recursive helper function for getItem().
	void retrieve(TreeNode *tree, Suspect &item, bool &found)
	{
		if (tree == NULL)
			found = false;						//Empty tree, item not found.
		else if (item < tree->info)
			retrieve(tree->left, item, found);	//Search the left subtree.
		else if (item > tree->info)
			retrieve(tree->right, item, found);	//Search the right subtree.
		else
		{
			item = tree->info;					//Item's key matches current node.
			found = true;						//Return true and a copy of node's info.
		}
	}

	//Recursive helper function for putItem().
	void insert(TreeNode *&tree, Suspect item)
	{
		//If the pointer passed to the function is null, insert the node here.
		if (tree == NULL)
		{
			tree = new TreeNode;
			tree->right = NULL;
			tree->left = NULL;
			tree->info = item;
		}

		//Otherwise, recursively call insert using the correct child pointer,
		//to insert the item based on its key.
		else if (item < tree->info)
			insert(tree->left, item);
		else
			insert(tree->right, item);
	}

	//Helper function that finds the logical predecessor of a node to be deleted,
	//used by deleteItem().
	void getPredecessor(TreeNode *tree, Suspect &data)
	{
		while (tree->right != NULL)
			tree = tree->right;
		data = tree->info;
	}

	//Recursive helper function for deleteItem().  Deletes a node with zero or one
	//child nodes.  For nodes with two children, the node's info is replaced by its
	//logical predecessor from the tree.
	void deleteNode(TreeNode *&tree)
	{
		Suspect data;
		TreeNode *tempPtr;

		//Case leaf or one child node, left child pointer is null.
		tempPtr = tree;
		if (tree->left == NULL)
		{
			tree = tree->right;
			delete tempPtr;
		}

		//Case one child node, right child pointer is null.
		else if (tree->right == NULL)
		{
			tree = tree->left;
			delete tempPtr;
		}

		//Case deleting a node with two children.  Get logical predecessor of tree and
		//assign node's info to this data.
		else
		{
			getPredecessor(tree->left, data);
			tree->info = data;
			Delete(tree->left, data);
		}
	}

	//Deletes a node from the tree if its key matches item's key.
	void Delete(TreeNode *&tree, Suspect item)
	{
		if (item < tree->info)
			Delete(tree->left, item);
		else if (item > tree->info)
			Delete(tree->right, item);
		else
			deleteNode(tree);
	}

	//Recursive helper function that prints all nodes in order on outFile.
	//Used by print().
	void printTree(TreeNode *tree, ofstream &outFile) const
	{
		if (tree != NULL)
		{
			printTree(tree->left, outFile);		//Print left subtree.
			outFile << tree->info;				//Print this node's information.
			cout << tree->info;					//Show the node's information on the screen (debugging)
			printTree(tree->right, outFile);	//Print right subtree.
		}
	}

	//Recursive helper function for class destructor.  Destroys a tree by
	//traversing it in postorder.
	void destroy(TreeNode *tree)
	{
		if (tree != NULL)			//If this node exists,
		{
			destroy(tree->left);	//delete its left and right children
			destroy(tree->right);	//in post-order,
			delete tree;			//then delete this node.
		}
	}

	//Recursive helper function for copying a tree (used by copy constructor and
	//overloaded assignment operator).
	void copyTree(TreeNode *&copy, const TreeNode *original)
	{
		if (original == NULL)
			copy = NULL;
		else
		{
			copy = new TreeNode;
			copy->info = original->info;
			copyTree(copy->left, original->left);
			copyTree(copy->right, original->right);
		}
	}

	//Recursive helper function to enqueue items in preorder.
	void preOrder(TreeNode *tree, QueType &preQue)
	{
		if (tree != NULL)
		{
			preQue.enqueue(tree->info);
			preOrder(tree->left, preQue);
			preOrder(tree->right, preQue);
		}

	}

	//Recursive helper function to enqueue items in order.
	void inOrder(TreeNode *tree, QueType &inQue)
	{
		if (tree != NULL)
		{
			inOrder(tree->left, inQue);
			inQue.enqueue(tree->info);
			inOrder(tree->right, inQue);
		}
	}

	//Recursive helper function to enqueue items in postorder.
	void postOrder(TreeNode *tree, QueType &postQue)
	{
		if (tree != NULL)
		{
			postOrder(tree->left, postQue);
			postOrder(tree->right, postQue);
			postQue.enqueue(tree->info);
		}
	}

public:
	//Default constructor.
	TreeType()
	{
		root = NULL;
	}

	//Destructor.
	~TreeType()
	{
		destroy(root);
	}

	//Copy constructor.
	TreeType(const TreeType &original)
	{
		copyTree(root, original.root);	//Use the copyTree() helper function.
	}

	//Overloaded = operator.
	void operator=(TreeType &original)
	{
		if (&original == this)	//Cannot copy a tree into itself.
			return;
		destroy(root);			//Deallocate any nodes already existing in this tree.
		copyTree(root, original.root);	//Use the copyTree() helper function.
	}

	//Empties the tree and deletes all nodes.
	void makeEmpty()
	{

	}

	//Returns true if the tree is empty, false otherwise.
	bool isEmpty() const
	{
		return (root == NULL);
	}

	//Returns true if there is no more space in memory for another node, false otherwise.
	bool isFull() const
	{
		TreeNode *location;

		try
		{
			//Attempt to allocate a new node.  Throws an exception if no more memory available.
			location = new TreeNode;
			delete location;
			return false;
		}
		catch (std::bad_alloc exception)
		{
			return true;
		}
	}

	//Returns the number of nodes in the tree using a helper function.
	int getLength()
	{
		return countNodes(root);
	}

	//Searches the tree for an item matching the parameter's key.
	//If the item is found, a copy of the full node is returned and found is true.
	//Otherwise, item is returned unchanged and found is false.
	Suspect getItem(Suspect item, bool &found)
	{
		retrieve(root, item, found);	//Search the tree starting with root
		return item;					//Return a copy of the item
	}

	//Inserts a new item into the tree.
	void putItem(Suspect item)
	{
		try
		{
			insert(root, item);
		}
		catch (std::bad_alloc exception)
		{
			cout << "Error: bad_alloc exception thrown in putItem.\n\n";
		}
	}

	void deleteItem(Suspect item)
	{
		Delete(root, item);
	}

	//Calls a helper function to create a queue of the tree elements in the desired order.
	void resetTree(OrderType order)
	{
		switch (order)
		{
		case PRE_ORDER:		preOrder(root, preOrderQue);
							break;
		case IN_ORDER:		inOrder(root, inOrderQue);
							break;
		case POST_ORDER:	postOrder(root, postOrderQue);
							break;
		}
	}

	//Gets the next item from the queue containing the items in the desired order.
	//If item is the last one in the queue, finished is true; otherwise it is false.
	Suspect getNextItem(OrderType order, bool &finished)
	{
		Suspect item;
		finished = false;
		switch (order)
		{
		case PRE_ORDER:		preOrderQue.dequeue(item);
							if (preOrderQue.isEmpty())
								finished = true;
							break;
		case IN_ORDER:		inOrderQue.dequeue(item);
							if (inOrderQue.isEmpty())
								finished = true;
							break;
		case POST_ORDER:	postOrderQue.dequeue(item);
							if (postOrderQue.isEmpty())
								finished = true;
							break;
		}

		return item;
	}

	//Prints all the nodes in the tree to the output file in sorted order.
	void print(ofstream &outFile) const
	{
		if (root == NULL)
		{
			cout << "There are currently no suspects to display.\n\n";
			outFile << "There are currently no suspects to display.\n\n";
		}

		else
		{
			printTree(root, outFile);
		}
	}
};