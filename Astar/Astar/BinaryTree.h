#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
using namespace std;

class BinaryTree
{
private:
	class Node
	{
	public:
		Node * right;
		Node * left;
		int data;
		Node()
		{
			data = 0;
			right = NULL;
			left = NULL;
		}
		Node(int d)
		{
			data = d;
			right = NULL;
			left = NULL;
		}
	};
	Node * root;

	Node * findNode(int val)
	{
		Node * curr = root;
		while(curr != NULL)
		{
			if(val == curr->data)
				return curr;
			else if(val < curr->data)
				curr = curr->left;
			else
				curr = curr->right;
		}
		return NULL;
	}

	//get node for the value val 
	Node ** getNode(int val)
	{
		Node ** curr = &root;
		while( *curr != NULL && (curr) != NULL)
		{
			if(val == (**curr).data)
				return curr;
			else if(val < (**curr).data)
				curr = &(**curr).left;
			else
				curr = &(**curr).right;
		}
		return curr;
	}

	void insertRecursive(int val, Node ** head)
	{
		if(*head == NULL){
			*head = new Node(val);
			return;
		}
		if(val <= (**head).data)
		{
			insertRecursive(val, &(**head).left);
		}
		else{
			insertRecursive(val, &(**head).right);
		}

	}

	int getSize(Node * current)
	{
		if(current == NULL)
			return 0;
		return getSize(current->left) + getSize(current->right) + 1;
	}

	int getHeigth(Node * current)
	{
		if(current == NULL)
			return -1;
		int out = getHeigth(current->left);
		int r = getHeigth(current->right);
		if(r > out)
			out = r;
		return 1 + out;
	}

	int countLeafs(Node * current)
	{
		if(current != NULL)
		{
			if(current->left == NULL && current->right == NULL)
				return 1;
			return countLeafs(current->left) + countLeafs(current->right);
		}
		return 0;
	}

	void clearTree(Node * &current)
	{
		if(current != NULL)
		{
			clearTree(current->right);
			clearTree(current->left);
			delete current;
			current = NULL;
		}
	}

	void print(Node * head)
	{
		if(head != NULL)
		{
			print(head->left);
			std::cout << head->data << "\n";
			print(head->right);
		}
	}

	int remove(Node ** node)
	{
		if(*node == NULL)
			return -1;
		Node * temp = *node;
		makeNode(temp->left, temp->right, node);
		int data = temp->data;
		delete temp;
		temp = NULL;
		return data;
	}

	//connect left & right node to a existing node.
	void makeNode(Node * &left, Node * &right, Node ** &out)
	{
		if(left == NULL){
			*out = right;
			return;
		}if(right == NULL){
			*out = left;
			return;
		}
		int c_r;
		Node ** _right = NULL;
		getLastNode(left, true, c_r, _right);
		int c_l;
		Node ** target = NULL;
		getLastNode(right, false, c_l, target);
		if(c_r > c_l)
		{
			target = _right;
		}
		int val = remove(target);

		(*out) = new Node(val);
		(*out)->right = right;
		(*out)->left = left;
	}

	//get last node to a direction & count
	void getLastNode(Node * &head, bool right, int & count, Node ** & out)
	{
		if(head == NULL)
		{
			out = &head;
			return;
		}
		if(right)
		{
			if(head->right != NULL)
			{
				count++;
				getLastNode(head->right, right, count, out);
				return;
			}
		}
		else
		{
			if(head->left != NULL)
			{
				count++;
				getLastNode(head->left, false, count, out);
				return;
			}
		}
		count += getHeigth(head);
		out = &head;
	}



public:
	BinaryTree()
	{
		root = NULL;
	}
	~BinaryTree()
	{
		clearTree(root);
	}

	//recurive insert
	void insertR(int val)
	{
		insertRecursive(val, &root);
	}

	//iteration insert
	int insert(int val)
	{
		Node **curr = getNode(val);
		if((*curr) == NULL)
			*curr = new Node(val);
		return val;
	}

	//test insert
	BinaryTree* inst(int val)
	{
		insert(val);
		return this;
	}

	bool have(int val)
	{
		return  *getNode(val) != NULL;
	}

	void print()
	{
		print(root);
	}

	int getSize()
	{
		return getSize(root);
	}

	int getHeight()
	{
		return getHeigth(root);
	}

	int getLeafCount()
	{
		return countLeafs(root);
	}

	int extractMin()
	{
		int i = 0;
		Node ** out;
		getLastNode(root, false, i, out);
		return remove( out);
	}

	int remove(int val)
	{
		Node ** n = getNode(val);
		if(*n == NULL)
			return -1;
		int out = remove(n);

		return out;
	}
};


#endif
//*/