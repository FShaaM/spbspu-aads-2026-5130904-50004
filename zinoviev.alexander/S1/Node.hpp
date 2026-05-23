#ifndef NODE_HPP
#define NODE_HPP

namespace zinoviev
{
  template < class T >
  struct Node
  {
    T val;
    Node* next;
    Node* prev;

    Node() :
      next(nullptr),
      prev(nullptr)
    {}

    Node(const T& value) :
      val(value),
      next(nullptr),
      prev(nullptr)
    {}
  };
}

#endif
