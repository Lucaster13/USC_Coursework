#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include "rotateBST.h"

template <typename Key, typename Value>
class SplayTree : public rotateBST<Key, Value>
{
	public:

		SplayTree();
		void insert(const std::pair<const Key, Value>& keyValuePair);
		void remove(const Key& key);
		typename SplayTree<Key, Value>::iterator find(const Key& key);
		typename SplayTree<Key, Value>::iterator findMin();
		typename SplayTree<Key, Value>::iterator findMax();
		void deleteMinLeaf();
		void deleteMaxLeaf();

	protected:

		void splay(Node<Key, Value> *r);

	private:

		Node<Key,Value>* getLastVisited(const Key& key);
		Node<Key,Value>* findPred(const Node<Key,Value>* curr);
};

//constructor for splay tree
template <typename Key, typename Value>
SplayTree<Key, Value>::SplayTree()
{
	this->mRoot = NULL;
}

//inserts given key value pair, splays that node to the root
template <typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair)
{
	this->BinarySearchTree<Key,Value>::insert(keyValuePair);
	Node<Key,Value>* insertNode = this->internalFind(keyValuePair.first);
	splay(insertNode);
}

/*
removes item from splay tree and splays that nodes parent to root

if node to remove does not exist, splays last visited node
*/
template <typename Key, typename Value>
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* removeNode = this->internalFind(key);

	if(!removeNode)
	{
		//get last visited and splay
		Node<Key, Value>* lastVisited = getLastVisited(key);
		splay(lastVisited);
	}
	else
	{
		//if removeNode has 2 children set splay node to parent of predecessor
		if(removeNode->getLeft() && removeNode->getRight())
		{
			Node<Key, Value>* pred = this->findPred(removeNode);
			if(pred->getParent() == removeNode)
			{
				this->BinarySearchTree<Key,Value>::remove(key);
				//splay pred since it is the new parent of deleted node
				splay(pred);
			}
			else
			{
				Node<Key, Value>* splayNode = pred->getParent();
				this->BinarySearchTree<Key,Value>::remove(key);
				//splay pred->getParent() 
				splay(splayNode);
			}
		}
		//1 or no children, just remove the node and splay its parent
		else
		{
			Node<Key, Value>* splayNode = removeNode->getParent();
			this->BinarySearchTree<Key,Value>::remove(key);
			splay(splayNode);
		}	
		
	}
	
}

/*
returns an iterator to the node with given key and splays that node to root

if node does not exist, returns this->end() and splays last visited node
*/
template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key, Value>::find(const Key& key)
{
	Node<Key, Value>* curr = this->internalFind(key);

	if(!curr)
	{
		//get last visited node, splay and return iterator to end
		Node<Key, Value>* lastVisited = getLastVisited(key);
		splay(lastVisited);
		return this->end();
	}
	else
	{
		//splay found node to root, return iterator to root
		splay(curr);
		typename SplayTree<Key, Value>::iterator it(curr);
		return it;
	}
}

/*
returns an iterator to the min value node in the tree and splays that node
*/
template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key, Value>::findMin()
{
	if(!this->mRoot) return this->end();

	Node<Key, Value>* curr = this->getSmallestNode();

	splay(curr);
	typename SplayTree<Key, Value>::iterator it(curr);
	return it;
}

/*
returns an iterator to the max value node in the tree and splays that node
*/
template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key, Value>::findMax()
{
	if(!this->mRoot) return this->end();

	Node<Key, Value>* curr = this->mRoot;

	while(curr->getRight())
	{
		curr = curr->getRight();
	}

	splay(curr);
	typename SplayTree<Key, Value>::iterator it(curr);
	return it;
}

/*
deletes the leaf node with minimum value in the tree and splays the
same way as remove()
*/
template <typename Key, typename Value>
void SplayTree<Key, Value>::deleteMinLeaf()
{
	if(!this->mRoot) return;

	Node<Key, Value>* curr = this->mRoot;

	//iterate until leaf is found
	while(curr->getLeft() || curr->getRight())
	{
		if(curr->getLeft()) 
		{
			curr = curr->getLeft();
			continue;
		}

		if(curr->getRight())
		{
			curr = curr->getRight();
		}
	}

	Node<Key, Value>* splayNode = curr->getParent();
	this->BinarySearchTree<Key,Value>::remove(curr->getKey());
	splay(splayNode);
}

/*
deletes the leaf node with maximum value in the tree and splays the
same way as remove()
*/
template <typename Key, typename Value>
void SplayTree<Key, Value>::deleteMaxLeaf()
{
	if(!this->mRoot) return;

	Node<Key, Value>* curr = this->mRoot;

	//iterate until leaf is found
	while(curr->getLeft() || curr->getRight())
	{
		if(curr->getRight()) 
		{
			curr = curr->getRight();
			continue;
		}

		if(curr->getLeft())
		{
			curr = curr->getLeft();
		}
	}

	Node<Key, Value>* splayNode = curr->getParent();
	this->BinarySearchTree<Key,Value>::remove(curr->getKey());
	splay(splayNode);
}

/*
splay function that splays a node to the root by doing a series of 
rotations based on that node's position in the tree
*/
template <typename Key, typename Value>
void SplayTree<Key, Value>::splay(Node<Key, Value> *r)
{
	if(!r) return;

	while(r->getParent())
	{
		Node<Key, Value>* parent = r->getParent();
		Node<Key, Value>* grandparent = parent->getParent();

		//if parent is root
		if(!grandparent)
		{
			//if r is left child, rotate parent right
			if(r == parent->getLeft())
			{
				this->rightRotate(parent);
				this->mRoot = r;
				return;
			}

			if(r == parent->getRight())
			{
				this->leftRotate(parent);
				this->mRoot = r;
				return;
			}
		}
		else
		{	
			//parent is left, r is right
			if(parent == grandparent->getLeft() && r == parent->getRight())
			{
				this->leftRotate(parent);
				this->rightRotate(grandparent);
			}
			//parent is right, r is left
			else if(parent == grandparent->getRight() && r == parent->getLeft())
			{
				this->rightRotate(parent);
				this->leftRotate(grandparent);
			}
			//parent is left, r is left
			else if(parent == grandparent->getLeft() && r == parent->getLeft())
			{
				this->rightRotate(grandparent);
				this->rightRotate(parent);
			}
			//parent is right, r is right
			else
			{
				this->leftRotate(grandparent);
				this->leftRotate(parent);
			}
		}
	}
}

/*
used to find the last visited node if a node that was attempted to be found 
could not be found
*/
template <typename Key, typename Value>
Node<Key,Value>* SplayTree<Key, Value>::getLastVisited(const Key& key)
{
	Node<Key, Value>* curr = this->mRoot;
	Node<Key, Value>* parent = NULL;

	while(curr != NULL)
	{
		//traverse left
		if(key < curr->getKey())
		{
			parent = curr;
			curr = curr->getLeft();
			continue;
		}

		//traverse right
		if(key > curr->getKey())
		{
			parent = curr;
			curr = curr->getRight();
			continue;
		}
	}

	return parent;
}

/**
* Helper function that returns the predecessor of a given node
*/
template<typename Key, typename Value>
Node<Key,Value>* SplayTree<Key,Value>::findPred(const Node<Key,Value>* curr)
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

#endif