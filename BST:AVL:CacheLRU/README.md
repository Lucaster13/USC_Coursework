Lucas Terr 

CSCI104

There is no Makefile for this directory,

Files included are: 

#bst.h
	
	implementation of templated binary search tree

#rotateBST.h
	
	implementation of templated rotating binary search tree
	inherits publicly from BinarySearchTree in bst.h

#splayTree.h
	
	implements a templated splay tree 
	inherits publicly from rotateBST found in rotateBST.h

#cacheLRU.h
	
	implements a templated cacheLRU class that contains a SplayTree
	as a private data member called cache
