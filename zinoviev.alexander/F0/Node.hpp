#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace zinoviev
{

  enum class Color
  {
    RED,
    BLACK
  };

  struct NodeBase
  {
    NodeBase* parent = nullptr;
    NodeBase* left = nullptr;
    NodeBase* right = nullptr;
    Color color = Color::RED;

    explicit NodeBase(Color c);
    NodeBase(NodeBase* p, NodeBase* l, NodeBase* r, Color c);
    virtual ~NodeBase() = default;
  };

  template< class Key, class Value >
  struct Node : public NodeBase
  {
    std::pair< const Key, Value > node;

    Node(Key k, Value val, NodeBase* p, NodeBase* l, NodeBase* r, Color c);
  };

  inline NodeBase::NodeBase(Color c):
    color(c)
  {
  }

  inline NodeBase::NodeBase(NodeBase* p, NodeBase* l, NodeBase* r, Color c):
    parent(p),
    left(l),
    right(r),
    color(c)
  {
  }

  template< class Key, class Value >
  Node< Key, Value >::Node(Key k, Value val, NodeBase* p, NodeBase* l, NodeBase* r, Color c):
    NodeBase(p, l, r, c),
    node(std::make_pair(k, val))
  {
  }

}

#endif
