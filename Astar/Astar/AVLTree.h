#ifndef AVLTREE_H
#define AVLTREE_H


template<class T>
class AVLTree{
private:
	T noValue()
	{
		T out;
		return out;
	}

	template<class T>
	class Node
	{
	public:
		Node<T> * right;
		Node<T> * left;
		T data;
		int height;
		Node()
		{
			data = 0;
			right = NULL;
			left = NULL;
			height = 0;
		}
		Node(T d)
		{
			data = d;
			right = NULL;
			left = NULL;
			height = 0;
		}
	};

	Node<T> * root;

	int max(int i, int j)
	{
		return i > j ? i : j;
	}

	Node<T> ** getNode(T val)
	{
		Node<T> ** curr = &root;
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

	int NodeHeight(Node<T> * Node)
	{
		return Node != NULL ? Node->height : -1;
	}

	void UpdateHeight(Node<T> * Node)
	{
		Node->height = max(NodeHeight(Node->right), NodeHeight(Node->left))+1;
	}

	T insert(T val, Node<T> *&head)
	{
		T out;
		if(head == NULL)
		{
			head = new Node<T>(val);
			return val;
		}
		else{
			if(val < (*head).data)
			{
				out = insert(val, (*head).left);
			}
			else{
				out = insert(val, (*head).right);
			}
			UpdateHeight(head);
			autoRotation(head);
		}
		return out;
	}

	void autoRotation(Node<T> *&root)
	{
		if(root == NULL)
			return;
		if(NodeHeight(root->left) > NodeHeight(root->right) + 1 )
		{
			Node<T> * left = root->left;
			if(NodeHeight(left->right) > NodeHeight(left->left) + 1)
				lrotation(left);
			rrotation(root);
		}
		else if(NodeHeight(root->right) > NodeHeight(root->left) + 1)
		{
			Node<T> * right = root->right;
			if(NodeHeight(right->right) > NodeHeight(right->left) + 1)
				rrotation(right);
			lrotation(root);
		}
	}

	void lrotation(Node<T> *&root)
	{
		if(root == NULL || root->right == NULL)
			return;
		Node<T> * A = root;
		Node<T> * B = root->right;
		A->right = B->left;
		B->left = A;
		root = B;
		//UpdateHeight(A);
		//UpdateHeight(B);

		A->height = (A->height - 2);
		A->height = A->height > 0 ? A->height : 0;
	}

	void rrotation(Node<T> *&root)
	{
		if(root == NULL || root->left == NULL)
			return;
		Node<T> * A = root;
		Node<T> * B = root->left;
		A->left = B->right;
		B->right = A;
		root = B;
		//UpdateHeight(A);
		//UpdateHeight(B);
		A->height = (A->height - 2);
		A->height = A->height > 0 ? A->height : 0;
	}

	int getSize(Node<T> * current)
	{
		if(current == NULL)
			return 0;
		return getSize(current->left) + getSize(current->right) + 1;
	}

	int getHeigth(Node<T> * current)
	{
		if(current == NULL)
			return -1;
		int out = getHeigth(current->left);
		int r = getHeigth(current->right);
		if(r > out)
			out = r;
		return 1 + out;
	}

	int countLeafs(Node<T> * current)
	{
		if(current != NULL)
		{
			if(current->left == NULL && current->right == NULL)
				return 1;
			return countLeafs(current->left) + countLeafs(current->right);
		}
		return 0;
	}

	void clearTree(Node<T> * &current)
	{
		if(current != NULL)
		{
			clearTree(current->right);
			clearTree(current->left);
			delete current;
			current = NULL;
		}
	}

	//to remove Node<T> keep valance
	//==========================

	//find Node<T> to remove
	T remove(int val, Node<T> **head)
	{
		if(head == NULL)
			return NULL;
		T rv = NULL;
		if((*head)->data == val)
		{
			rv = remove(head);
		}else if((*head)->data > val)
		{
			rv = remove(val, &(*head)->left);
		}else{
			rv = remove(val, &(*head)->right);
		}
		autoRotation((*head));
		return rv;
	}

	//remove Node<T>
	T remove(Node<T> **head)
	{
		if(*head == NULL)
		{
			T out;
			return out;
		}
		T data = (*head)->data;
		if((*head)->right == NULL && (*head)->left == NULL)
		{
			delete *head;
			*head = NULL;
		}else if((*head)->right != NULL && (*head)->left != NULL)
		{
			if( (*head)->right->data > (*head)->left->data)
			{
				(*head)->data = removeLast( (*head)->right,false);
			}else{
				(*head)->data = removeLast( (*head)->left,true);
			}
		}else{
			Node<T> * temp = (*head);
			if(temp->right != NULL)
			{
				(*head) = temp->right;
				temp->right = NULL;
				delete temp;
			}else{
				(*head) = temp->left;
				temp->left = NULL;
				delete temp;
			}
		}
		return data;
	}

	T removeLast(Node<T> *&head, bool right)
	{
		if(head == NULL)
			return noValue();
		T val = noValue();
		if(right)
		{
			if(head->right == NULL)
			{
				val = remove(&head);
			}else{
				val = removeLast(head->right,right);
			}
		}
		else{
			if(head->left == NULL)
			{
				val = remove(&head);
			}else{
				val = removeLast(head->left,right);
			}
		}
		autoRotation(head);
		return val;
	}

public:
	AVLTree()
	{
		root = NULL;
	}
	~AVLTree()
	{
		clearTree(root);
	}

	void clearTree()
	{
		clearTree(root);
	}

	T insert(T val)
	{
		return insert(val, root);
	}

	bool have(T val)
	{
		return  *getNode(val) != NULL;
	}

	bool empty()
	{
		return root == NULL;
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
		return NodeHeight(root);//return getHeigth(root);
	}

	int getLeafCount()
	{
		return countLeafs(root);
	}

	T removeSmaller()
	{
		return removeLast(root, false);
	}

	T removeBiger()
	{
		return removeLast(root, true);
	}

	T remove(T val)
	{
		Node<T> ** n = getNode(val);
		if(*n == NULL)
		{
			T out;
			return out;
		}
		T out = remove(n);

		return out;
	}

	T get(T val)
	{
		Node<T> ** n = getNode(val);
		if(*n == NULL)
		{
			T out;
			return out;
		}
		return (*n)->data;
	}
};



#endif