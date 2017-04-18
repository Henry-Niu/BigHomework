#ifndef __MAP_HPP__
#define __MAP_HPP__

namespace bgh {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map: public RB_Tree<Key, T, Compare> {

public:

	typedef pair<const Key, T> value_type;

	T &operator [] (const Key &x) {
		T tmp;
		auto cur = this -> insert({x, tmp});
		return cur.first -> second;
	}
	
	const T & operator [] (const Key &key) const {
		auto res = this -> _find(key);
		typename RB_Tree<Key, T, Compare>::node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	T & at(const Key &key) {
		auto res = this -> _find(key);
		typename RB_Tree<Key, T, Compare>::node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	const T & at(const Key &key) const {
		auto res = this -> _find(key);
		typename RB_Tree<Key, T, Compare>::node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	bool empty() const { return this -> root == NULL; }
	
	size_t count(const Key &key) const { return this ->_checkFindRes(this -> _find(key)) ? 1 : 0; }

};

}

#endif
