/*
 * hash.cpp
 *
 *  Created on: 15 déc. 2021
 *      Author: rapha
 */

template <class K, class V>
myHash<K,V>::myHash(unsigned int size):size(size){
	p_table = new struct hash_obj<K,V>[size];
}

template <class K, class V>
bool myHash<K,V>::add(K key, V value, unsigned int pos){
	if (pos < size){
		struct hash_obj<K,V> el = {key,value};
		p_table[pos] = el;
		return true;
	}
	return false;
}

template <class K, class V>
bool myHash<K,V>::modify(K key, V new_value){
	unsigned int i = 0;
	for (i=0; i < size; i++){
		if (key == p_table[i].key){
			p_table[i].value = new_value;
			return true;
		}
	}
	return false;
}

template <class K, class V>
V myHash<K,V>::find(K key){
	unsigned int i = 0;
	for (i=0; i < size; i++){
		if (key == p_table[i].key){
			return p_table[i].value;
		}
	}
	return false;
}


