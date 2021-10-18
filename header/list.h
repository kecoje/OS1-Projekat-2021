#ifndef _list_h_
#define _list_h_

#include<iostream.h>;

template<class T>
class List {
protected:
	friend class KernelSe;

	struct Elem {
		T* data;
		Elem *next, *prev;
		Elem(T* t) {
			data = t;
			next = prev = 0;
		}
	};


	int len;

public:
	Elem* head;
	Elem* tail;

	List() {
		tail = head = 0;
		len = 0;
	}
	~List() {
		while (head != 0) {
			Elem* old = head;
			head = head->prev;
			delete old;
		}

		len = 0;
		tail = head = 0;
	}

	int getLen() const {
		return len;
	}
	int isEmpty() const {
		return len == 0;
	}
	int isNotEmpty() const {
		return len != 0;
	}

	List<T>& push(T* t) {
		Elem* el = new Elem(t);
		if (!head) {
			tail = head = el;
		} else {
			head->next = el;
			el->prev = head;
			head = el;
		}
		len++;
		return *this;
	}

	List<T>& pushBack(T* t) {
		Elem* el = new Elem(t);
		if (!head) {
			tail = head = el;
		} else {
			tail->prev = el;
			el->next = tail;
			tail = el;
		}
		len++;
		return *this;
	}

	void remove(T* t) {
		if (head == tail) {
			delete head;
			head = tail = 0;
			len = 0;
			return;
		} else {
			for (Elem* curr = head; curr != 0; curr = curr->prev) {
				if (curr->data == t) {
					if (curr->next) {
						curr->next->prev = curr->prev;
						if (curr == tail) {
							tail = curr->next;
						}
					}
					if (curr->prev) {
						curr->prev->next = curr->next;
						if (curr == head) {
							head = curr->prev;
						}
					}
					delete curr;
					len--;
					return;
				}
			}
		}
	}

	T* pop() {
		Elem* el = head;
		if (head == tail) {
			head = tail = 0;
		} else {
			head = el->prev;
			head->next = 0;
		}
		if(el == 0) return 0;
		T* ret = el->data;
		len--;
		delete el;
		return ret;
	}

	T* popBack() {
		Elem* el = tail;
		if (head == tail) {
			head = tail = 0;
		} else {
			tail = el->next;
			tail->prev = 0;
		}
		if(el == 0) return 0;
		T* ret = el->data;
		len--;
		delete el;
		return ret;
	}

	T& getTail() {
		return *(tail->data);
	}

	T& getHead() {
		return *(head->data);
	}

	void print() const {
		if (len == 0)
			return;
		Elem* tek = head;
		while (tek) {
			//cout << *(tek->data) << " ";
			tek = tek->prev;
		}
		return;
	}
};

#endif // _list_h_
