// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 03.02.2021.
//

#ifndef LIST_H_
#define LIST_H_

#include <stdexcept>
#include <cstddef>

template<typename T>
class List {
 public:
  List() {
    node* n = new node{0, nullptr, nullptr};
    head = n;
    tail = n;
    _size = 0;
  }

  List(const List<T>& l) : List() {
    node* p = l.tail;
    while (p != nullptr) {
      push_front(p->_data);
      p = p->next;
    }
  }

  List<T>& operator=(const List<T>& l) {
    if (&l == this)
      return *this;

    node* p = tail;
    while (p != nullptr) {
      node* next = p->next;
      delete p;
      p = next;
    }
    node* n = new node{0, nullptr, nullptr};
    head = n;
    tail = n;
    _size = 0;
    p = l.tail;
    while (p != nullptr) {
      push_front(p->_data);
      p = p->next;
    }

    return *this;
  }

  ~List() {
    node* p = tail;
    while (p != nullptr) {
      node* next = p->next;
      delete p;
      p = next;
    }
  };

  void push_front(T elem) {
    if (_size != 0) {
      node* n = new node{elem, nullptr, head};
      head->next = n;
      head = n;
    } else {
      head->_data = elem;
    }
    _size++;
  }

  void push_back(T elem) {
    if (_size != 0) {
      node* n = new node{elem, tail, nullptr};
      tail->prev = n;
      tail = n;
    } else {
      tail->_data = elem;
    }
    _size++;
  }

  void remove_front() {
    if (_size > 1) {
      node* temp = head->prev;
      head->prev->next = nullptr;
      delete head;
      head = temp;
    }
    if (_size > 0)
      _size--;
  }

  void remove_back() {
    if (_size > 1) {
      node* temp = tail->next;
      tail->next->prev = nullptr;
      delete tail;
      tail = temp;
    }
    if (_size > 0)
      _size--;
  }

  T pop_back() {
    T ret = get_back();
    remove_back();
    return ret;
  }

  T pop_front() {
    T ret = get_front();
    remove_front();
    return ret;
  }

  T get_front() const {
    if (_size == 0)
      throw std::runtime_error("Get call on empty list!");
    return head->_data;
  }

  T get_back() const {
    if (_size == 0)
      throw std::runtime_error("Get call on empty list!");
    return tail->_data;
  }

  size_t size() const {
    return _size;
  }

  void remove_if_equal_to(T elem) {
    node* p = tail;
    while (p != nullptr) {
      node* next = p->next;

      if (p->_data == elem) {
        if (p == tail) {
          remove_back();
        } else if (p == head) {
          remove_front();
        } else {
          p->next->prev = p->prev;
          p->prev->next = p->next;
          delete p;
          _size--;
        }
      }

      p = next;
    }
  }

 private:
  struct node {
    T _data;
    node* next;
    node* prev;
  };

  node* head;
  node* tail;
  size_t _size;
};

#endif  // LIST_H_
