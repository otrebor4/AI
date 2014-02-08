#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include "pairKeyComp.h"
#include "AVLTree.h"

using namespace std;

template<class Key, class Value>
class hashTable
{
private:
	AVLTree< pairKeyComp<Key,Value>> * table;
	int numItems;
	int tablesize;

	int hash(Key x)
	{
		std::hash<Key> khash;
		return khash(x)%tablesize;
	}

	pairKeyComp<Key,Value> makePair(Key & k, Value & v)
	{
		
		pairKeyComp<Key,Value> pair(k,v);
		return pair;
	}

public:
	hashTable()
	{
		init(50);
	}

	hashTable(int size)
	{
		init(size);
	}

	void init(int size)
	{
		tablesize = size;
		table = new AVLTree< pairKeyComp<Key,Value>>[size];
		numItems = 0;
	}

	void put(Key k, Value v)
	{
		int hsh = hash(k);
		table[hsh].insert(makePair(k,v));
		numItems++;
	}
	double computeLoadFactor()
	{
		return static_cast<double>(numItems)/tablesize;
	}

	Value remove(Key x)
	{
		int hsh = hash(x);
		int size =  table[hsh].getSize();
		Value v;
		pairKeyComp<Key,Value> pair = table[hsh].remove( makePair(x, v) );
		if(table[hsh].getSize() < size)
		{
			tablesize--;
		}
		return pair.value;
	}

	Value get(Key x)
	{
		int hsh = hash(x);
		int size =  table[hsh].getSize();
		Value v;
		pairKeyComp<Key,Value> pair = table[hsh].get(makePair(x,v));
		return pair.value;

	}
};

#endif