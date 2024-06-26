#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  List():first(nullptr),last(nullptr),list_size(0) {}

  List(const List<T> &other): first(nullptr),last(nullptr),list_size(0)
  { 
    copy_all(other);
  }

  ~List()
  {
    clear();
  }

  List<T>& operator=(const List<T> &other)
  {
    if(&other!=this)
    {
      clear();
      copy_all(other);
      return *this; 
    }
    else
    {
      return *this;
    }
  }

   //EFFECTS:  returns true if the list is empty
  bool empty() const
  {
    return list_size==0; 
  }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow.  Instead, keep track of the size
  //         with a private member variable.  That's how std::list does it.
  int size() const
  {
    return list_size;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front()
  {
    return first->datum;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back()
  {
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum)
  {
    Node *tmp = new Node{nullptr,nullptr,datum};
    if(empty())
    {
      first=tmp;
      last=tmp;
    }
    else
    {
      tmp->next = first;
      first->prev = tmp;
      first = tmp; 
    }
    ++list_size;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum)
  {
    Node * tmp = new Node{nullptr,nullptr,datum};
    if(empty())
    {
      first = tmp;
      last = tmp;
    }
    else
    {
      tmp->prev=last;
      last->next=tmp;
      last=tmp;
    }
    ++list_size;
  }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
  void pop_front()
  {
    Node * tmp = first;
    first = first->next;
    if(first!=nullptr)
    {
      first->prev=nullptr;
    }
    else
    {
      last = nullptr;
    }
    delete tmp;
    --list_size;
  }
  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
  void pop_back()
  {
    Node *tmp = last;
    last = last->prev;
    if(last!=nullptr)
    {
      last->next=nullptr;
    }
    else
    {
      first = nullptr;
    }
    delete tmp;
    --list_size;
  }

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
  void clear()
  {
    while(!empty())
    {
      pop_front();
    }
  }

  void copy_all(const List<T> &other)
  {
    for(Node *node = other.first; node!=nullptr; node=node->next)
    {
      push_back(node->datum);
    }
  }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you can omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists

private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty
  int list_size; 

public:
  ////////////////////////////////////////
  class Iterator {
    //OVERVIEW: Iterator interface to List


    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Iterators.


    // Your iterator should implement the following public operators: *,
    // ++ (prefix), default constructor, == and !=.


  public:
    Iterator() : node_ptr(nullptr) {};
    Iterator(Node* node_ptr) : node_ptr(node_ptr) {}
    Iterator(const Iterator& other) : node_ptr(other.node_ptr) {}


  Iterator& operator++() {
    assert(node_ptr);
    node_ptr = node_ptr->next;
    return *this;
  }


  T& operator*() const {
    assert(node_ptr);
    return node_ptr->datum;
  }


  Iterator& operator=(const Iterator& other) {
    node_ptr = other.node_ptr;
    return *this;
  }


  bool operator==(const Iterator& other) const {
    return node_ptr == other.node_ptr;
  }


  bool operator!=(const Iterator& other) const {
    return node_ptr != other.node_ptr;
  }


  // This operator will be used to test your code. Do not modify it.
  // Requires that the current element is dereferenceable.
  Iterator& operator--() {
    assert(node_ptr);
    node_ptr = node_ptr->prev;
    return *this;
  }


  private:
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here

    // add any friend declarations here
    friend class List<T>;


  };
  //List::Iterator
  ////////////////////////////////////////


  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(first);
  }


  // return an Iterator pointing to "past the end"
  Iterator end() const {
    return Iterator(nullptr);
  }


  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container
  void erase(Iterator i)
  {
    assert(i.node_ptr);
      if (i.node_ptr == first) {
          pop_front();
      } else if (i.node_ptr == last) {
          pop_back();
      } else {
          i.node_ptr->prev->next = i.node_ptr->next;
          i.node_ptr->next->prev = i.node_ptr->prev;
          delete i.node_ptr;
          --list_size;
      }
}


  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: inserts datum before the element at the specified position.
  void insert(Iterator i, const T &datum)
  {
    if (i == begin()) {
        push_front(datum); 
    } 
    else if (i == end()) {
        push_back(datum);
        }
    else {
      Node* new_node = new Node{nullptr,nullptr,datum};
      new_node->prev = i.node_ptr->prev;
      new_node->next = i.node_ptr;
      i.node_ptr->prev->next = new_node;
      i.node_ptr->prev = new_node;
      ++list_size;
    }
  }

};//List




////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.
#endif // Do not remove this. Write all your code above this line.
