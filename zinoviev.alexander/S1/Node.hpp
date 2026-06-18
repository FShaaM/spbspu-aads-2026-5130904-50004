#ifndef NODE_HPP
#define NODE_HPP

namespace zinoviev
{
  template< class T >
  struct Node
  {
    T val;
    Node* next;
    Node* prev;

    Node();
    Node(const T& value);
  };

  template< class T >
  Node< T >::Node():
    val(),
    next(nullptr),
    prev(nullptr)
  {}

  template< class T >
  Node< T >::Node(const T& value):
    val(value),
    next(nullptr),
    prev(nullptr)
  {}

}

#endif
