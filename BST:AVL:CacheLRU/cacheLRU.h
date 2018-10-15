#ifndef CACHE_LRU_H
#define CACHE_LRU_H

#include "splayTree.h"
#include <stdexcept>


template <typename Key, typename Value>
class cacheLRU
{
	public:

		cacheLRU(int capacity);
		~cacheLRU();
		void put(const std::pair<const Key, Value>& keyValuePair);
		std::pair<const Key, Value> get(const Key& key);

	private:

		SplayTree<Key,Value> cache;
		int MAXsize;
		int currentSize;
		
};

//constructor for cacheLRU, initializes capactiy and currentsize
template <typename Key, typename Value>
cacheLRU<Key,Value>::cacheLRU(int capacity)
{
	this->MAXsize = capacity;
	this->currentSize = 0;
}

//destuctor that calls clear on the cache to deallocate memory given to nodes
template <typename Key, typename Value>
cacheLRU<Key,Value>::~cacheLRU()
{
	cache.clear();
}

/*
inserts an item into the cache
if the cache is at capactiy then it removes the minimum leaf in the tree
*/
template <typename Key, typename Value>
void cacheLRU<Key,Value>::put(const std::pair<const Key, Value>& keyValuePair)
{
	//if key exists dont do anything
	if(this->cache.find(keyValuePair.first) != this->cache.end()) return;

	//if capacity reached
	if(this->currentSize == this->MAXsize)
	{
		cache.deleteMinLeaf();
		cache.insert(keyValuePair);
	}
	else
	{
		//insert and increase size
		cache.insert(keyValuePair);
		this->currentSize++;
	}
	
}

/*
returns the key value pair of a node with the given key
if the node does not exist, throws a logic error
*/
template <typename Key, typename Value>
std::pair<const Key, Value> cacheLRU<Key,Value>::get(const Key& key)
{
	typename SplayTree<Key, Value>::iterator it(this->cache.find(key));

	if(it == cache.end())
	{
		throw std::logic_error("ERROR: Item does not currently exist in cache");
	}
	else
	{
		return *it;
	}
}


#endif