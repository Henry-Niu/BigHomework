#ifndef __linkList__
#define __linkList__


template <class T>
class linkList {
private:
	struct node {
		T data;
		node *prev, *next;
		
		node(const T &_data, node *p = NULL, node *n = NULL): data(_data), prev(p), next(n) {}
		node():prev(NULL), next(NULL){}
		~node() {};
	};
	
	node *head, *tail;
	size_t curSize;
	
	node *move(int x) const {
		if (x < 0 || x > curSize) throw outOfBound{};
	
		node *p;
		p = head;
		for (int i = 0; i < x; ++i) p = p -> next;
	
		return p;
	}
	
public:
	linkList();
	~linkList(){clear(); delete head; delete tail;}
	
	class const_iterator;
	class iterator {
		friend class linkList;
	private:
		 node *p;
	public:
		iterator() {
			p = NULL;
		}
		iterator(node *_p): p(_p) {}
		iterator(const iterator &other): p(other.p) {}
		iterator operator ++ (int) {
			if (p -> next == NULL) throw sjtu::invalid_iterator{};
			iterator tmp(*this);
			p = p -> next;
			return tmp;
		}
		iterator & operator ++ () {
			if (p -> next == NULL) throw sjtu::invalid_iterator{};
			p = p -> next;
			return *this;
		}
		iterator operator -- (int) {
			if (p -> prev -> prev == NULL) throw sjtu::invalid_iterator{};
			iterator tmp(*this);
			p = p -> prev;
			return *this;
		}
		iterator &operator--() {
			if (p -> prev -> prev == NULL) throw sjtu::invalid_iterator{};
			p = p -> prev;
			return *this;
		}
		T &operator * () const {
			return p -> data;
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

		T *operator -> () const noexcept {
			return &(p -> data);
		}
	};
	class const_iterator {
		friend class linkList;
	private:
		const node *p;
	public:
		const_iterator(): p(NULL) {}
		const_iterator(const const_iterator &other): p(other.p) {}
		const_iterator(const iterator &other): p(other.p) {}
		
		const_iterator(node *_p): p(_p) {}
		
		const T &operator * () const {
			return p -> data;
		}
		
		const_iterator operator ++ (int) {
			if (p -> next == NULL) throw sjtu::invalid_iterator{};
			iterator tmp(*this);
			p = p -> next;
			return tmp;
		}
		const_iterator & operator ++ () {
			if (p -> next == NULL) throw sjtu::invalid_iterator{};
			p = p -> next;
			return *this;
		}
		const_iterator operator -- (int) {
			if (p -> prev -> prev == NULL) throw sjtu::invalid_iterator{};
			iterator tmp(*this);
			p = p -> prev;
			return *this;
		}
		const_iterator &operator -- () {
			if (p -> prev -> prev == NULL) throw sjtu::invalid_iterator{};
			p = p -> prev;
			return *this;
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
		const T * operator -> () const {
			return &(p -> data);
		}
	};
	
	void clear();
	iterator insert(int x, const T &a) {
		if (x < 0 || x > curSize) throw outOfBound{};
		node *p = move(x), *q = p -> next;
		p -> next = q -> prev = new node (a, p, q);
		++curSize; 
		return iterator(p -> next);
	}
	iterator preInsert(iterator pre, const T &a) {
		node *p = pre.p;
		if (p == NULL) throw sjtu::invalid_iterator{};
		p -> prev = new node(a, p -> prev, p);
		p -> prev -> prev -> next = p -> prev;
		++curSize;
		return iterator(p -> prev);
	}
	
	iterator sufInsert(iterator suf, const T &a) {
		node *p = suf.p;
		if (p == NULL) throw sjtu::invalid_iterator{};
		p -> next = new node(a, p, p -> next);
		p -> next -> next -> prev = p -> next;
		++curSize;
		return iterator(p -> next);
	}
	void preRemove(iterator it);
	void sufRemove(iterator it);
	void remove(int x);
	void remove(iterator it);
	void traverse() const;
	size_t length() const { return curSize; }
	int search(const T &a) const;
	T visit(int x) const;
	
	iterator begin() const { return iterator(head -> next); }
	iterator end() const { return iterator(tail); }
	const_iterator cbegin() const { return const_iterator(head -> next); }
	const_iterator cend() const { return const_iterator(tail); }
};

template <class T>
linkList<T>::linkList() {
	head = new node;
	head -> next = tail = new node;
	tail -> prev = head;
	curSize = 0;
}

template <class T>
void linkList<T>::clear() {
	for (node *p = head -> next, *q; p != tail; p = q) {
		q = p -> next;
		delete p;
	}
	head -> next = tail;
	tail -> prev = head;
	curSize = 0;
}

template <class T>
void linkList<T>::remove(int x) {
	if (x < 0 || x >= curSize) throw outOfBound{};
	node *cur = move(x), *p = cur -> prev, *q = cur -> next;
	p -> next = q;
	q -> prev = p;
	delete cur;
	--curSize;
}

template <class T>
void linkList<T>::remove(iterator it) {
	node *cur = it.p;
	if (cur == NULL || cur -> prev == NULL || cur -> next == NULL) throw sjtu::invalid_iterator{};
	node *p = cur -> prev, *q = cur -> next;
	p -> next = q;
	q -> prev = p;
	delete cur;
	--curSize;
}

template <class T>
int linkList<T>::search(const T &a) const {
	int x;
	for (node *p = head -> next; p != tail; p = p -> next, ++x) {
		if (p -> data == a) return x;
	}
	return -1;
}

template <class T>
T linkList<T>::visit(int x) const {
	return move(x) -> data;
}

template <class T>
void linkList<T>::sufRemove(iterator it) {
	node *p = it.p;
	if (p -> next == NULL || p -> next == tail) throw sjtu::invalid_iterator{};
	node * tmp = p -> next;
	p -> next = tmp -> next;
	tmp -> next -> prev = p;
	--curSize;
	delete tmp;
}

template <class T>
void linkList<T>::preRemove(iterator it) {
	node *p = it.p;
	if (p == head || p == head -> next) throw sjtu::invalid_iterator{};
	node *tmp = p -> prev;
	p -> prev = tmp -> prev;
	tmp -> prev -> next = p;
	--curSize;
	delete tmp;
}
#endif
