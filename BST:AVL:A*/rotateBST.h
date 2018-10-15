#ifndef ROTATEBST_H
#define ROTATEBST_H

#include "bst.h"
#include <iostream>


template <typename Key, typename Value>
class rotateBST : public BinarySearchTree<Key,Value>
{
	public:
		bool sameKeys(const rotateBST& t2) const;
		void transform(rotateBST& t2) const;
	protected:
		void leftRotate(Node<Key, Value>* r);
		void rightRotate(Node<Key, Value>* r);
	private:
		bool sameKeysHelper(Node<Key,Value>* nodeT1, Node<Key,Value>* nodeT2) const;
		Node<Key,Value>* makeLinkedList(rotateBST& t2, Node<Key,Value>* curr) const;
		void transformHelper(rotateBST& t2, const Node<Key,Value>* currT1, Node<Key,Value>* currT2) const;
		void updateHeight(Node<Key, Value>* curr);
		int b(Node<Key, Value>* curr) const; 

}; 

//returns true if trees have the same key set
template <typename Key, typename Value>
bool rotateBST<Key,Value>::sameKeys(const rotateBST& t2) const
{
	Node<Key,Value>* nodeT1 = this->getSmallestNode();
	Node<Key,Value>* nodeT2 = t2.getSmallestNode();

	return sameKeysHelper(nodeT1, nodeT2);
}

//recursive helper for sameKeys: does in-order traversal
template <typename Key, typename Value>
bool rotateBST<Key,Value>::sameKeysHelper(Node<Key,Value>* nodeT1, Node<Key,Value>* nodeT2) const
{
	if(nodeT1 == NULL && nodeT2 == NULL)
	{
		return true;
	}
	else if(nodeT1 == NULL || nodeT2 == NULL)
	{
		return false;
	}
	
	if(nodeT1->getKey() != nodeT2->getKey()) return false;

	//traverse to find successor of nodeT1
	if(nodeT1->getRight() != NULL)
	{
		nodeT1 = nodeT1->getRight();
		while(nodeT1->getLeft() != NULL)
		{
			nodeT1 = nodeT1->getLeft();
		}
	}
	else
	{
		Node<Key, Value>* parent = nodeT1->getParent();
		while(parent != NULL && nodeT1 == parent->getRight())
		{
			nodeT1 = parent;
			parent = parent->getParent();
		}
		nodeT1 = parent;
	}

	//traverse to find successor of nodeT2
	if(nodeT2->getRight() != NULL)
	{
		nodeT2 = nodeT2->getRight();
		while(nodeT2->getLeft() != NULL)
		{
			nodeT2 = nodeT2->getLeft();
		}
	}
	else
	{
		Node<Key, Value>* parent = nodeT2->getParent();
		while(parent != NULL && nodeT2 == parent->getRight())
		{
			nodeT2 = parent;
			parent = parent->getParent();
		}
		nodeT2 = parent;
	}

	return sameKeysHelper(nodeT1, nodeT2);
}

//transforms input tree to be exactly the same as this
template <typename Key, typename Value>
void rotateBST<Key,Value>::transform(rotateBST& t2) const
{
	if(!sameKeys(t2)) return;

	transformHelper(t2, this->mRoot, t2.mRoot);

	t2.updateHeight(t2.mRoot);
}

//helper function for transform: right rotations until tree is linked list in increasing order
template <typename Key, typename Value>
Node<Key,Value>* rotateBST<Key,Value>::makeLinkedList(rotateBST& t2, Node<Key,Value>* curr) const
{
	if(!curr) return NULL;

	while(curr->getLeft())
	{
		t2.rightRotate(curr);
		curr = curr->getParent();
	}

	if(curr->getRight())
	{
		return (makeLinkedList(t2, curr->getRight()))->getParent();
	}

	return curr;	
}

//helper function for transform: performs left rotations on root and both children until nodes match
template <typename Key, typename Value>
void rotateBST<Key,Value>::transformHelper(rotateBST& t2, const Node<Key,Value>* currT1, Node<Key,Value>* currT2) const
{
	if(!currT1) return;

	currT2 = makeLinkedList(t2, currT2);

	while(currT1->getKey() != currT2->getKey())
	{
		t2.leftRotate(currT2);
		currT2 = currT2->getParent();
	}

	transformHelper(t2, currT1->getLeft(), currT2->getLeft());
	transformHelper(t2, currT1->getRight(), currT2->getRight());
}

template <typename Key, typename Value>
void rotateBST<Key,Value>::leftRotate(Node<Key, Value>* r)
{
	if(!r || !r->getRight()) return;

	Node<Key, Value>* rP = r->getParent();
	Node<Key, Value>* newRoot = r->getRight();
	Node<Key, Value>* newRootLeft = r->getRight()->getLeft();

	if(rP) 
	{
		if(rP->getLeft() == r) rP->setLeft(newRoot);
		if(rP->getRight() == r) rP->setRight(newRoot);
	}
	else
	{
		this->mRoot = newRoot;
	}

	newRoot->setParent(rP);
	newRoot->setLeft(r);

	r->setParent(newRoot);
	r->setRight(newRootLeft);

	if(newRootLeft) newRootLeft->setParent(r);

}
		
template <typename Key, typename Value>
void rotateBST<Key,Value>::rightRotate(Node<Key, Value>* r)
{
	if(!r || !r->getLeft()) return;

	Node<Key, Value>* rP = r->getParent();
	Node<Key, Value>* newRoot = r->getLeft();
	Node<Key, Value>* newRootRight = r->getLeft()->getRight();

	if(rP) 
	{
		if(rP->getLeft() == r) rP->setLeft(newRoot);
		if(rP->getRight() == r) rP->setRight(newRoot);
	}
	else
	{
		this->mRoot = newRoot;
	}

	newRoot->setParent(rP);
	newRoot->setRight(r);

	r->setParent(newRoot);
	r->setLeft(newRootRight);

	if(newRootRight) newRootRight->setParent(r);

}

//helper function to fix the heights after transform, post-order traversal
template <typename Key, typename Value>
void rotateBST<Key,Value>::updateHeight(Node<Key, Value>* curr)
{
	if(!curr) return;

	this->updateHeight(curr->getLeft());
	this->updateHeight(curr->getRight());

	int balanceFactor = this->b(curr);

	//left and right subtrees are same height or parent is leaf node
	if(balanceFactor == 0)
	{
		if(curr->getRight())
		{
			curr->setHeight(curr->getRight()->getHeight() + 1);
		}
		else
		{
			curr->setHeight(1);
		}
	}
	//left tree greater
	else if(balanceFactor <= -1)
	{
		curr->setHeight(curr->getLeft()->getHeight() + 1);
	}
	//right tree greater
	else if(balanceFactor >= 1)
	{
		curr->setHeight(curr->getRight()->getHeight() + 1);
	}
}


//Determines balance factor of current node (copied from bst.h)
template<typename Key, typename Value>
int rotateBST<Key, Value>::b(Node<Key, Value>* curr) const 
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



#endif 