/*
 * hash.h
 *
 *  Created on: 15 déc. 2021
 *      Author: rapha
 */

#ifndef INC_HASH_H_
#define INC_HASH_H_

// utility to use easily the hash table
template <typename T, typename U> struct hash_obj{
	T key;
	U value;
};

template <class K, class V>
class myHash{
private:
	struct hash_obj<K,V>* p_table;
public:
	// vars
	unsigned int size = 0; // number of element in the table
	//constructor
	myHash(unsigned int size);

	// return true if ok and false otherwise
	bool add(K key, V value, unsigned int pos);

	// find with the key and change the value of that key
	bool modify(K key, V new_value);

	//find a value with a key
	V find(K key);

	V operator[](K const& key){return find(key);}
};

#include "hash.inl"


#endif /* INC_HASH_H_ */
