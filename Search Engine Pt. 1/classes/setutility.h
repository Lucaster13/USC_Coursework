#include "webpage.h"
/*
TEMPLATED
setIntersection:

find the intersection of two sets
*/
template <typename T>
std::set<T> setIntersection(std::set<T>& lhs, std::set<T>& rhs)
{
	std::set<T> newIntersection;
	typename std::set<T>::iterator it;
	for(it = lhs.begin(); it != lhs.end(); ++it)
	{
		if(rhs.find(*it) != rhs.end())
		{
			newIntersection.insert(*it);
		}
	}

	return newIntersection;
};


/*
TEMPLATED
setUnion:

finds the union of two sets
*/
template <typename T>
std::set<T> setUnion(std::set<T>& Union, std::set<T>& rhs)
{
	typename std::set<T>::iterator it;
	for(it = rhs.begin(); it != rhs.end(); ++it)
	{
		if(Union.find(*it) == Union.end())
		{
			Union.insert(*it);
		}
	}

	return Union;
};


