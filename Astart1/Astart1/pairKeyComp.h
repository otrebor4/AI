#ifndef PAIRKEYVALUE_H
#define PAIRKEYVALUE_H

using namespace std;

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
	pairKeyComp(pairKeyComp<K,V> & other)
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