#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
* A templated class for a Node in a search tree. The getters for parent/left/right are virtual so that they
* can be overridden for future kinds of search trees, such as Red Black trees, Splay trees, and AVL trees.
*/
template <typename Key, typename Value>
class Node 
{
public:
	Node(const Key& key, const Value& value, Node<Key, Value>* parent);
	virtual ~Node();

	const std::pair<const Key, Value>& getItem() const;
	std::pair<const Key, Value>& getItem();
	const Key& getKey() const;
	const Value& getValue() const;
	Value& getValue();

	virtual Node<Key, Value>* getParent() const;
	virtual Node<Key, Value>* getLeft() const;
	virtual Node<Key, Value>* getRight() const;
	int getHeight() const;

	void setParent(Node<Key, Value>* parent);
	void setLeft(Node<Key, Value>* left);
	void setRight(Node<Key, Value>* right);
	void setValue(const Value &value);
	void setHeight(int height);

protected:
	std::pair<const Key, Value> mItem;
	Node<Key, Value>* mParent;
	Node<Key, Value>* mLeft;
	Node<Key, Value>* mRight;
	int mHeight;
};

/*
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) 
	: mItem(key, value)
	, mParent(parent)
	, mLeft(NULL)
	, mRight(NULL)
	, mHeight(1)
{ 

} 

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the clear() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node() 
{ 

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const 
{ 
	return mItem; 
}

/**
* A non-const getter for the item. 
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() 
{ 
	return mItem; 
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const 
{ 
	return mItem.first; 
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const 
{ 
	return mItem.second; 
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
	return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const 
{ 
	return mParent; 
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const 
{ 
	return mLeft; 
}

/**
* An implementation of the virtual function for retreiving the right child. 
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const 
{ 
	return mRight; 
}

/**
* A const getter for the height.
*/
template<typename Key, typename Value>
int Node<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent) 
{ 
	mParent = parent; 
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left) 
{ 
	mLeft = left; 
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right) 
{ 
	mRight = right; 
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) 
{ 
	mItem.second = value; 
}

/**
* A setter for the height of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setHeight(int height) 
{ 
	mHeight = height;
}

/*
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree 
{
	public:
		BinarySearchTree(); //TODO
		virtual ~BinarySearchTree(); //TODO
		int height(); //TODO 
		bool isBalanced(); //TODO 
		virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
		virtual void remove(const Key& key); //TODO
		void clear(); //TODO
		void print() const;

	public:
		/**
		* An internal iterator class for traversing the contents of the BST.
		*/
		class iterator 
		{
			public:
				iterator(Node<Key,Value>* ptr);
				iterator();

				std::pair<const Key, Value>& operator*();
				std::pair<const Key, Value>* operator->();

				bool operator==(const iterator& rhs) const;
				bool operator!=(const iterator& rhs) const;
				iterator& operator=(const iterator& rhs);

				iterator& operator++();

			protected:
				Node<Key, Value>* mCurrent;		
		};

	public:
		iterator begin();
		iterator end();
		iterator find(const Key& key) const;

	protected:
		Node<Key, Value>* internalFind(const Key& key) const; //TODO
		Node<Key, Value>* getSmallestNode() const; //TODO
		void printRoot (Node<Key, Value>* root) const;

	protected:
		Node<Key, Value>* mRoot;

	private:
		bool BalancedHelper(Node<Key,Value>* curr);
		void clearHelper(Node<Key,Value>* curr);
		void HeightHelper(Node<Key, Value>* curr);
		Node<Key,Value>* findPred(const Node<Key,Value>* curr);
		int b(Node<Key, Value>* curr) const;
		

};

/*
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
	: mCurrent(ptr)
{ 

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
	: mCurrent(NULL)
{ 

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() 
{ 
	return mCurrent->getItem();
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() 
{ 
	return &(mCurrent->getItem()); 
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key, Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
	this->mCurrent = rhs.mCurrent;
	return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
	if(mCurrent->getRight() != NULL)
	{
		mCurrent = mCurrent->getRight();
		while(mCurrent->getLeft() != NULL)
		{
			mCurrent = mCurrent->getLeft();
		}
	}
	else if(mCurrent->getRight() == NULL)
	{
		Node<Key, Value>* parent = mCurrent->getParent();
		while(parent != NULL && mCurrent == parent->getRight())
		{
			mCurrent = parent;
			parent = parent->getParent();
		}
		mCurrent = parent;
	}
	return *this;
}

/*
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/*
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{ 
	// TODO
	mRoot = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() 
{ 
	// TODO
	clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{ 
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin()
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end()
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& key) const 
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/**
* An method to return the height of the BST.
*/
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::height()
{
	// TODO
	if(mRoot == NULL) 
		return 0;
	else
		return mRoot->getHeight();
}


/**
* An method to checks if the BST is balanced. This method returns true if and only if the BST is balanced.
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced()
{
	// TODO
	Node<Key, Value>* curr = mRoot;

	if(curr == NULL) return true;
	
	bool result = BalancedHelper(curr);

	return result;
}

/**
* recursive helper function that determines if tree is balanced
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::BalancedHelper(Node<Key, Value>* curr)
{
	Node<Key, Value>* left = curr->getLeft();
	Node<Key, Value>* right = curr->getRight();

	if(left == NULL && right == NULL)
	{
		return true;
	}
	//right child nonexistent
	else if(right == NULL && left->getHeight() != 1)
	{
		return false;
	}
	//left child nonexistent
	else if(left  == NULL && right->getHeight() != 1)
	{
		return false;
	}
	else if (left != NULL && right != NULL)
	{
		if(BalancedHelper(left) & BalancedHelper(right))
		{
			int balanceCheck = left->getHeight() - right->getHeight();
			if(balanceCheck > 1 || balanceCheck < -1) 
				return false;
			else
				return true;
		}
		else 
		{
			return false;
		}
	}

	//if only one child with height 1
	return true;
	


}

/**
* An insert method to insert into a Binary Search Tree. The tree will not remain balanced when 
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair)
{
	//TODO
	//If root doesnt exist yet
	if(mRoot == NULL)
	{
		mRoot = new Node<Key,Value>(keyValuePair.first, keyValuePair.second, NULL);
		return;
	}

	Node<Key, Value>* curr = mRoot;
	while(curr != NULL)
	{
		//node exists
		if(curr->getKey() == keyValuePair.first)
		{
			return;
		}

		if(keyValuePair.first < curr->getKey())
		{
			if(curr->getLeft() == NULL)
			{
				curr->setLeft(new Node<Key,Value>(keyValuePair.first, keyValuePair.second, curr));
				//Increase Height of Ancesters
				HeightHelper(curr);
				return;
			}
			else
			{
				curr = curr->getLeft();
				continue;
			}
		}
		else if(keyValuePair.first > curr->getKey())
		{
			if(curr->getRight() == NULL)
			{
				curr->setRight(new Node<Key,Value>(keyValuePair.first, keyValuePair.second, curr));
				//Increase Height of Ancesters
				HeightHelper(curr);
				return;
			}
			else
			{
				curr = curr->getRight();
				continue;
			}
		}
	}
		
}
/**
* An remove method to remove a specific key from a Binary Search Tree. The tree may not remain balanced after
* removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
	//TODO
	Node<Key, Value>* removeNode = internalFind(key);

	//node doesnt exist
	if(!removeNode) return;

	//Internal Node with 2 Children
	if(removeNode->getLeft() && removeNode->getRight())
	{
		//get Predecessor
		Node<Key, Value>* predecessor = findPred(removeNode);

		//keep track of parent and children of removeNode and predecessor
		Node<Key, Value>* predParent = predecessor->getParent();
		Node<Key, Value>* predLeft = predecessor->getLeft();
		//no right child for predecessor

		Node<Key, Value>* rnParent = removeNode->getParent();
		Node<Key, Value>* rnLeft = removeNode->getLeft();
		Node<Key, Value>* rnRight = removeNode->getRight();

		//change parent of predecessor
		if(rnParent)
		{
			predecessor->setParent(rnParent);
			if(rnParent->getLeft() == removeNode) rnParent->setLeft(predecessor);
			if(rnParent->getRight() == removeNode) rnParent->setRight(predecessor);
		}
		else
		{
			predecessor->setParent(NULL);
			mRoot = predecessor;
		}

		//change parent of removeNode
		if(rnLeft == predecessor)
		{
			removeNode->setParent(predecessor);
		}
		else
		{
			removeNode->setParent(predParent);
			predParent->setRight(removeNode);
		}

		//change children of predecessor
		if(rnLeft == predecessor)
		{
			predecessor->setLeft(removeNode);
		}
		else
		{
			predecessor->setLeft(rnLeft);
			rnLeft->setParent(predecessor);
		}
		predecessor->setRight(rnRight);
		rnRight->setParent(predecessor);

		//change children of removeNode
		if(predLeft)
		{
			removeNode->setLeft(predLeft);
			predLeft->setParent(removeNode);
		}
		else
		{
			removeNode->setLeft(NULL);
		}
		removeNode->setRight(NULL);

		//now send the newly situated removeNode to one of the if statements below
	}

	//Internal Node w/ left child only
	if(removeNode->getLeft() && !removeNode->getRight())
	{
		Node<Key, Value>* leftChild = removeNode->getLeft();
		Node<Key, Value>* parent = removeNode->getParent();

		if(parent != NULL)
		{
			leftChild->setParent(parent);
			if(parent->getLeft() == removeNode) parent->setLeft(leftChild);
			if(parent->getRight() == removeNode) parent->setRight(leftChild);
		}
		else
		{
			leftChild->setParent(NULL);
			mRoot = leftChild;
		}

		HeightHelper(leftChild);

		removeNode->setParent(NULL);
		removeNode->setLeft(NULL);
		removeNode->setRight(NULL);

		delete removeNode;
		return;
	}

	//Internal Node w/ right child only
	if(removeNode->getRight() && !removeNode->getLeft())
	{
		Node<Key, Value>* rightChild = removeNode->getRight();
		Node<Key, Value>* parent = removeNode->getParent();

		if(parent != NULL)
		{
			rightChild->setParent(parent);
			if(parent->getLeft() == removeNode) parent->setLeft(rightChild);
			if(parent->getRight() == removeNode) parent->setRight(rightChild);
		}
		else
		{
			rightChild->setParent(NULL);
			mRoot = rightChild;
		}
		
		HeightHelper(rightChild);

		removeNode->setParent(NULL);
		removeNode->setLeft(NULL);
		removeNode->setRight(NULL);

		delete removeNode;
		return;
	}

	//LEAF NODE
	if(!removeNode->getRight() && !removeNode->getLeft())
	{
		//if only one node in tree
		if(!removeNode->getParent())
		{
			mRoot = NULL;
			delete removeNode;
			return;
		}

		//else
		Node<Key, Value>* parent = removeNode->getParent();

		if(removeNode == parent->getLeft()) parent->setLeft(NULL);

		if(removeNode == parent->getRight()) parent->setRight(NULL);

		HeightHelper(parent);

		removeNode->setParent(NULL);
		delete removeNode;
		return;
	}
}

/**
* A method to remove all contents of the tree and reset the values in the tree 
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	// TODO
	clearHelper(mRoot);
	mRoot = NULL;
}

/**
* recursive helper for clear function
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key,Value>* curr)
{
	if(curr == NULL) return;

	Node<Key,Value>* left = curr->getLeft();
	Node<Key,Value>* right = curr->getRight();

	curr->setParent(NULL);
	curr->setLeft(NULL);
	curr->setRight(NULL);
	delete curr;

	if(left != NULL) clearHelper(left);

	if(right != NULL) clearHelper(right);
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
	// TODO
	//If root doesnt exist yet
	if(mRoot == NULL) return NULL;

	Node<Key, Value>* curr = mRoot;

	while(curr->getLeft())
	{
		curr = curr->getLeft();
	}

	return curr;

}

/**
* Helper function to find a node with given key, k and 
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const 
{
	// TODO
	//If root doesnt exist yet
	if(mRoot == NULL) return NULL;

	Node<Key, Value>* curr = mRoot;
	while(curr != NULL)
	{
		if(curr == NULL) break;

		//key found
		if(curr->getKey() == key)
		{
			return curr;
		}

		//key < current key
		if(key < curr->getKey())
		{
			curr = curr->getLeft();
			continue;
		}
		
		//key > current key
		if(key > curr->getKey())
		{
			curr = curr->getRight();
			continue;
		}
	}
	//node not found
	return NULL;
}

/**
* Helper function to print the tree's contents
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::printRoot (Node<Key, Value>* root) const
{
	if(root != NULL)
	{
		std::cout << "[";
		printRoot (root->getLeft());
		std::cout << " (" << root->getKey() << ", " << root->getValue() << ") ";
		printRoot (root->getRight());
		std::cout << "]";
	}
}


/**
* Helper function to decrease heights when removing a node
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::HeightHelper(Node<Key, Value>* curr)
{
	Node<Key, Value>* parent = curr;

	while(parent != NULL)
	{
		int balanceFactor = b(parent);

		//left and right subtrees are same height or parent is leaf node
		if(balanceFactor == 0)
		{
			if(parent->getRight())
			{
				parent->setHeight(parent->getRight()->getHeight() + 1);
			}
			else
			{
				parent->setHeight(1);
			}
		}
		//left tree greater
		else if(balanceFactor <= -1)
		{
			parent->setHeight(parent->getLeft()->getHeight() + 1);
		}
		//right tree greater
		else if(balanceFactor >= 1)
		{
			parent->setHeight(parent->getRight()->getHeight() + 1);
		}

		//if top reached, set current node to root
		if(parent->getParent() == NULL)
		{
			mRoot = parent;
			return;
		} 

		parent = parent->getParent();
	}
}

/**
* Helper function that returns the predecessor of a given node
*/
template<typename Key, typename Value>
Node<Key,Value>* BinarySearchTree<Key, Value>::findPred(const Node<Key,Value>* curr)
{
	//left child exists
	if(curr->getLeft() != NULL)
	{
		//traverse until rightmost child found
		Node<Key, Value>* predecessor = curr->getLeft();
		while(predecessor->getRight() != NULL)
		{
			predecessor = predecessor->getRight();
		}

		return predecessor;
	}

	//left child does not exist
	Node<Key, Value>* predecessor = curr->getParent();
	while(predecessor != predecessor->getParent()->getRight())
	{
		predecessor = predecessor->getParent();
	}
	return predecessor->getParent();	
}


//Determines balance factor of current node
template<typename Key, typename Value>
int BinarySearchTree<Key,Value>::b(Node<Key, Value>* curr) const 
{
	//curr NULL
	if(!curr) return 0;
	//Both children NULL
	if(!curr->getRight() && !curr->getLeft()) return 0;
	//Right child NULL
	if(!curr->getRight()) return curr->getLeft()->getHeight() * -1;
	//Left child NULL
	if(!curr->getLeft()) return curr->getRight()->getHeight();
	//Both Children exist
	return curr->getRight()->getHeight() - curr->getLeft()->getHeight();
}



/*
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif
