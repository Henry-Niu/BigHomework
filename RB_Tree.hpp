#ifndef __RB_TREE_HPP__
#define __RB_TREE_HPP__

namespace bgh {

const int B = 1, R = 4, RB = 2,  BB = 3;
int nullPtr;

template <class Key, class T, class Compare = std::less<Key> >
class RB_Tree {
protected:
	typedef pair<const Key, T> value_type;
	struct node {
		value_type *value;
		node *pnt, *ch[2];
		int color;
		
		node *prev, *succ;

		node() { value = NULL; pnt = NULL; ch[0] = ch[1] = NULL; color = B; prev = succ = NULL;}
		node(const value_type &_key, node *_left = NULL, node *_right = NULL, node *_pnt = NULL, int _color = B):
			color(_color), pnt(_pnt)
		{
			ch[0] = _left; ch[1] = _right;
			prev = succ = NULL;
			value = new value_type(_key);
		}	
		~node(){delete value;}
	}*root, *head, *tail;
	
	int curSize;
	
	int& _color(node *p) const {
		nullPtr = B;
		return p == NULL ? nullPtr : p -> color;
	}

	node *_copy(node *cur, node *list[], int &len) {
		if (cur == NULL) return NULL;
		node *p = new node(*cur -> value, NULL, NULL, NULL, cur -> color);
		p -> ch[0] = _copy(cur -> ch[0], list, len);
		list[len++] = p;
		p -> ch[1] = _copy(cur -> ch[1], list, len);
		if (p -> ch[0]) p -> ch[0] -> pnt = p;
		if (p -> ch[1]) p -> ch[1] -> pnt = p;
		return p;
	}
	
	void _buildSeq(node *list[], int len) {
		if (!len) return;
		for (int i = len - 1; i >= 0; i--) {
			_updateLink(list[i], head, head -> succ);
		}
	}

	void _clear(node *cur) {
		if (cur == NULL) return;
		_clear(cur -> ch[0]);
		_clear(cur -> ch[1]);
		delete cur;
	}
	
	void _matchPntChild(node *pnt, node *child, int par) {
		if (child) child -> pnt = pnt;
		if (par == -1) root = child;
		else pnt -> ch[par] = child;
	}

	void _rotate(node *x, bool b) {
		node *y = x -> pnt;
		node *z = y -> pnt;
		node *son = x -> ch[b];
		_matchPntChild(z, x, z ? (z -> ch[1] == y) : -1);
		_matchPntChild(x, y, b);
		_matchPntChild(y, son, !b)
	}

	pair<node *, bool> _find(const Key &k) const {
		node *p = root;
		node *f = NULL;
		bool b = 0;
		Compare cmp;

		while (p != NULL) {
			if (!cmp(k, p -> value -> first) && !cmp(p -> value -> first, k)) return {f, b};
			f = p;
			if (cmp(k, p -> value -> first)) p = p -> ch[b = 0]; 
			else p = p -> ch[b = 1];
		}
		return {f, b};
	}


	void _insertFixUp(node *const p) {
		node *x = p;
		while (_color(x -> pnt) == R) {
			node *y = x -> pnt;
			node *z = y -> pnt;
			node *u = z -> ch[z -> ch[0] == y];
			if (_color(u) == R) {
				_color(y) = _color(u) = B;
				_color(z) = R;
				x = z;
			}
			else if ((z -> ch[1] == u) == (y -> ch[1] == x)) {
				_rotate(x, y -> ch[0] == x);
				x = y;
			}
			else {
				_color(y) = B;
				_color(z) = R;
				_rotate(y, z -> ch[0] == y);
			}
		}
		_color(root) = B;
	}


	void _deleteFixUp(node *const p, node *fa) {
		node *x = p, *y = fa, *b = fa ? fa -> ch[fa -> ch[0] == p] : NULL;
		while (x != root && _color(x) == B) {
			if (_color(b) == R) {
				_color(b) = B;
				_color(y) = R;
				_rotate(b, y -> ch[0] == b);
				b = y -> ch[y -> ch[0] == x];
			}
			if (_color(b -> ch[0]) == B && _color(b -> ch[1]) == B) {
				_color(b) = R;
				x = y;
				y = x -> pnt;
				if (y) b = y -> ch[y -> ch[0] == x];
			}
			else {
				if (_color(b -> ch[y -> ch[0] == x]) == B) {
					node *tmp = b -> ch[y -> ch[1] == x];
					_color(b) = R;
					_color(tmp) = B;
					_rotate(tmp, b -> ch[0] == tmp);
					b =  y -> ch[y -> ch[0] == x];	
				}
				node *tmp = b -> ch[y -> ch[0] == x];
				_color(b) = _color(y);
				_color(y) = B;
				_color(tmp) = B;
				_rotate(b, y -> ch[0] == b);
				x = root;
			}
		}
		_color(x) = B;
	}
	
	node *_checkFindRes(const pair<node *, bool> &res) const {
		node *y = res.first;
		if (!y && !root) return NULL;
		if (y && !y -> ch[res.second]) return NULL;
		return !y ? root : y -> ch[res.second];
	}
	
	void _updateLink(node *x, node *prev, node *succ) {
		x -> prev = prev;
		x -> succ = succ;
		if (prev) prev -> succ = x;
		if (succ) succ -> prev = x;
	}
	
	node *_lower_bound(const Key &key) const {
		node *p = root, res = NULL;
		Compare cmp;
		
		while (p) {
			if (cmp(p -> value -> first, key)) p = p -> ch[1];
			else {
				res = p;
				p = p -> ch[0];
			}
		}
		
		return res;
	}
	
	node *_upper_bound(const Key &key) const {
		node *p = root, res = NULL;
		Compare cmp;
		
		while (p) {
			if (cmp(key , p -> value -> first)) {
				res = p;
				p = p -> ch[0];
			}
			else p = p -> ch[1];
		}
		
		return res;
	}
	
public:

	class const_iterator;
	class iterator {
		friend class RB_Tree;
	private:
		node *p;
	public:
		iterator(){}
		iterator(node *pp): p(pp) {}
		iterator(const iterator &other): p(other.p) {}
		iterator operator ++ (int) {
			if (p -> succ == NULL) throw invalid_iterator{};
			iterator tmp(*this);
			p = p -> succ;
			return tmp;
		}
		iterator &operator ++ () {
			if (p -> succ == NULL) throw invalid_iterator{};
			p = p -> succ;
			return *this;
		}
		iterator operator -- (int) {
			if (p -> prev -> prev == NULL) throw invalid_iterator{};
			iterator tmp(*this);
			p = p -> prev;
			return tmp;
		}
		iterator &operator -- () {
			if (p -> prev -> prev == NULL) throw invalid_iterator{};
			p = p -> prev;
			return *this;
		}
		value_type &operator * () const {
			return *(p -> value);
		}
		bool operator == (const iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator == (const const_iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator != (const iterator &rhs) const {
			return p != rhs.p;
		}
		bool operator != (const const_iterator &rhs) const {
			return p != rhs.p;
		}
		value_type *operator -> () const noexcept {
			return p -> value;
		}
	};
	
	class const_iterator {
		friend class RB_Tree;
	private:
		const node *p;
	public:
		const_iterator(){}
		const_iterator(node *pp): p(pp) {}
		const_iterator(const iterator &other): p(other.p) {}
		const_iterator(const const_iterator &other): p(other.p) {}
		const_iterator operator ++ (int) {
			if (p -> succ == NULL) throw invalid_iterator{};
			const_iterator tmp(*this);
			p = p -> succ;
			return tmp;
		}
		const_iterator &operator ++ () {
			if (p -> succ == NULL) throw invalid_iterator{};
			p = p -> succ;
			return *this;
		}
		const_iterator operator -- (int) {
			if (p -> prev -> prev == NULL) throw invalid_iterator{};
			const_iterator tmp(*this);
			p = p -> prev;
			return tmp;
		}
		const_iterator &operator -- () {
			if (p -> prev -> prev == NULL) throw invalid_iterator{};
			p = p -> prev;
			return *this;
		}
		const value_type &operator * () const {
			return *(p -> value);
		}
		bool operator == (const iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator == (const const_iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator != (const iterator &rhs) const {
			return p != rhs.p;
		}
		bool operator != (const const_iterator &rhs) const {
			return p != rhs.p;
		}
		const value_type *operator -> () const noexcept {
			return p -> value;
		}
	};

	RB_Tree() {
		root = NULL;
		head = new node;
		tail = new node;
		head -> succ = tail;
		tail -> prev = head;
		curSize = 0;
		
	}
	RB_Tree(const RB_Tree &other) {
		node **list = new node* [other.size()];
		curSize = 0;
		root = _copy(other.root, list, curSize);
		head = new node;
		tail = new node;
		head -> succ = tail;
		tail -> prev = head;
		_buildSeq(list, curSize);
		delete [] list;
	}
	
	~RB_Tree() {
		_clear(root);
		delete head;
		delete tail;
	}
	
	RB_Tree &operator = (const RB_Tree &other) {
		if (this == &other) return *this;
		
		node **list = new node* [other.size()];
		_clear(root); root = NULL;
		head -> succ = tail;
		tail -> prev = head;
		
		root = _copy(other.root, list, curSize);
		_buildSeq(list, curSize);
		delete [] list;
	}
	
	pair<iterator, bool> insert(const value_type &elem) {
		auto p = _find(elem.first);
		node *y = p.first, *x;
		bool b = p.second;
		
		if (x = _checkFindRes(p)) return {iterator(x), false};
		
		++curSize;
		
		x = new node(elem, NULL, NULL, y, R);
		if (y == NULL) {
			root = x; 
			_updateLink(x, head, tail);
		}
		else {
			y -> ch[b] = x;
			if (!b) {
				_updateLink(x, y -> prev, y);
			}
			else {
				_updateLink(x, y, y -> succ);
			}
		}
		
		_insertFixUp(x);
		return {iterator(x), true};
	}
	
	void erase(iterator pos) {
		if (pos == end() || pos.p == NULL) throw invalid_iterator{};
		auto res = _find(pos -> first);
		if (!res.first && root != pos.p || res.first && pos.p != res.first -> ch[res.second]) throw invalid_iterator{};
		
		--curSize;
		
		int referCol;
		node *a = pos.p;
		node *pnta = a -> pnt, *cha[2] = {a -> ch[0], a -> ch[1]}, *x;
		
		_updateLink(a -> prev, a -> prev -> prev, a -> succ);
		_updateLink(a -> succ, a -> prev, a -> succ -> succ);
		if (!cha[0] || !cha[1]) {
			referCol = a -> color;
			x = cha[0] ? cha[0] : cha[1];
			_matchPntChild(pnta, x, pnta ? (pnta -> ch[1] == a) : -1);
			if (referCol == B) _deleteFixUp(x, pnta);
			delete a;
		}
		else {
			node *b = a -> succ;
			node *pntb = b -> pnt, *chb[2] = {b -> ch[0], b -> ch[1]};
			referCol = b -> color;
			b -> color = a -> color;
			if (b == cha[1]) {
				_matchPntChild(pnta, b, pnta ? (pnta -> ch[1] == a) : -1);
				_matchPntChild(b, cha[0], 0);
				x = chb[1];
				if (referCol == B) _deleteFixUp(x, b);
				delete a;
			}
			else {
				x = chb[0] ? chb[0] : chb[1];
				_matchPntChild(pnta, b, pnta ? (pnta -> ch[1] == a) : -1);
				_matchPntChild(pntb, x, pntb -> ch[1] == b);
				_matchPntChild(b, cha[0], 0);
				_matchPntChild(b, cha[1], 1);
				_matchPntChild(pntb, x, pntb -> ch[1] == b);
				if (referCol == B) _deleteFixUp(x, pntb);
				delete a;
			}
		}
	}

	iterator begin() const { return iterator(head -> succ); }
	const_iterator cbegin() const { return const_iterator(head -> succ); }
	
	iterator end() const { return iterator(tail); }
	const_iterator cend() const { return const_iterator(tail); }
	
	bool empty() const { return curSize == 0; }
	
	size_t size() const { return curSize; }
	
	iterator find(const Key &key) {
		auto res = _find(key);
		node *x;
		if (x = _checkFindRes(res)) return iterator(x);
		else return end();
	}
	const_iterator find(const Key &key) const {
		auto res = _find(key);
		node *x;
		if (x = _checkFindRes(res)) return const_iterator(x);
		else return cend();
	}
	
	void clear() {
		_clear(root);
		root = NULL;
		head -> succ = tail;
		tail -> prev = head;
		curSize = 0;
	}
	
	iterator lower_bound(const Key &key) const {
		node *p = _lower_bound(key);
		return p == NULL ? end() : iterator(p);
	}
	
	const_iterator lower_bound(const Key &) const {
		node *p = _lower_bound(key);
		return p == NULL ? cend() : const_iterator(p);
	}
	
	iterator upper_bound(const Key &key) const {
		node *p = _upper_bound(key);
		return p == NULL ? end() : iterator(p);
	}
	
	const_iterator upper_bound(const Key &key) const {
		node *p = _upper_bound(key);
		return p == NULL ? cend() : const_iterator(p);
	}
};

}

#endif
