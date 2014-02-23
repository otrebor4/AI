#ifndef PAIRKEYVALUE_H
#define PAIRKEYVALUE_H
//
//  pairKeyComp.h
//
//  Created by Roberto Flores.
//  Copyright (c) 2013 Roberto Flores. All rights reserved.
//
//  pairKeyComp template class,
//  data pair holder, key value is used for binary comparasion <, >, <=, >=
//  while key and value values are used for binary comparasion ==, !=
//

template<class K, class V>
class pairKeyComp
{
public:
	K key;
	V value;
	pairKeyComp()
	{
	}
	pairKeyComp(K _key, V _val)
	{
		key = _key;
		value = _val;
	}
	pairKeyComp(pairKeyComp<K,V> const & other)
	{
		key = other.key;
		value = other.value;
	}
	
	

};
template<class K, class V>
inline bool operator==(const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return lhs.key == rhs.key && lhs.value == rhs.value;}
template<class K, class V>
inline bool operator!=(const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return !operator==(lhs,rhs);}
template<class K, class V>
inline bool operator< (const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return lhs.key < rhs.key; }
template<class K, class V>
inline bool operator> (const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return  operator< (rhs,lhs);}
template<class K, class V>
inline bool operator<=(const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return !operator> (lhs,rhs);}
template<class K, class V>
inline bool operator>=(const pairKeyComp<K,V>& lhs, const pairKeyComp<K,V>& rhs){return !operator< (lhs,rhs);}
#endif