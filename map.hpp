#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "RB_Tree.hpp"
#include <fstream>

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map: public RB_Tree<Key, T, Compare> {

private:
	typedef pair<const Key, T> value_type;
	typedef typename RB_Tree<Key, T, Compare>::node node;

public:

	T &operator [] (const Key &x) {
		T tmp;
		auto cur = this -> insert({x, tmp});
		return cur.first -> second;
	}
	
	const T & operator [] (const Key &key) const {
		auto res = this -> _find(key);
		node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	T & at(const Key &key) {
		auto res = this -> _find(key);
		node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	const T & at(const Key &key) const {
		auto res = this -> _find(key);
		node *x = this -> _checkFindRes(res);
		if (!x) throw index_out_of_bound{};
		return x -> value -> second;
	}
	
	bool empty() const { return this -> root == NULL; }
	
	size_t count(const Key &key) const { return this ->_checkFindRes(this -> _find(key)) ? 1 : 0; }
	
};

template<
	class Key,
	class Compare = std::less<Key>
> class MyMap: public map<Key, int, Compare> {
private:

	typedef pair<const Key, int> value_type;
	typedef typename RB_Tree<Key, int, Compare>::node node;
	typedef pair<value_type, int> pir;

	void _inTree(ifstream &fin, node *(&p), node *pnt, vector<node *> &list) {
		pir nod;
		fin.read(reinterpret_cast <char *> (&nod), sizeof(pir));
		if (nod.first.second == -1) p = NULL;
		else {
			p = new node(nod -> first);
			_inTree(fin, p -> ch[0], p, list);
			list.push_back(p);
			_inTree(fin, p -> ch[1], p, list);
			p -> pnt = pnt;
			p -> color = nod -> second;
		}
	}
	
	void _outTree(ofstream &fout, node *p) {
		pir nod;
		if (p == NULL) {
			nod.first.second = -1;
			fout.write(reinterpret_cast <const char *> (&nod), sizeof(nod));
			return;
		}
		nod = {*(p -> value), p -> color};
		fout.write(reinterpret_cast <const char *> (&nod), sizeof(nod));
		_outTree(fout, p -> ch[0]);
		_outTree(fout, p -> ch[1]);
	}

public:
	
	void inTree(ifstream &fin) {
		vector<node *> list;
		_inTree(fin, this -> root, NULL, list);
		
		this -> head = new node;
		this -> tail = new node;
		this -> head -> prev = NULL;
		this -> head -> succ = this -> tail;
		this -> tail -> succ = NULL;
		this -> tail -> prev = this -> head;
		
		this -> curSize = list.size();
		this -> _buildSeq(list);
	}
	
	void outTree(ofstream &fout) {
		_outTree(fout, this -> root);
	}
};



#endif
