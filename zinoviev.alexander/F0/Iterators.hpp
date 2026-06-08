#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include "Node.hpp"

namespace zinoviev
{
  template< class Key, class Value, class Compare >
  class RBTree;

  template< class Key, class Value >
  class Iterator
  {
    NodeBase* node_;

  public:

    template<class K, class V, class C> friend class RBTree;

    explicit Iterator(NodeBase* n);

    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();
    bool operator==(Iterator< Key, Value > other);
    bool operator!=(Iterator< Key, Value > other);
    Iterator< Key, Value >& operator++();
    Iterator< Key, Value >& operator--();
  };

  template< class Key, class Value >
  class CIterator
  {
    NodeBase* node_;

  public:

    template<class K, class V, class C> friend class RBTree;

    explicit CIterator(NodeBase* n);

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    bool operator==(CIterator< Key, Value > other);
    bool operator!=(CIterator< Key, Value > other);
    CIterator< Key, Value >& operator++();
    CIterator< Key, Value >& operator--();
  };

  template< class Key, class Value >
  Iterator< Key, Value >::Iterator(NodeBase* n) :
    node_(n)
  {
  }

  template< class Key, class Value >
  std::pair< const Key, Value >& Iterator< Key, Value >::operator*()
  {
    return static_cast<Node< Key, Value >*>(node_)->node;
  }

  template< class Key, class Value >
  std::pair< const Key, Value >* Iterator< Key, Value >::operator->()
  {
    return &(static_cast<Node< Key, Value >*>(node_)->node);
  }

  template< class Key, class Value >
  bool Iterator< Key, Value >::operator==(Iterator< Key, Value > other)
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool Iterator< Key, Value >::operator!=(Iterator< Key, Value > other)
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  Iterator< Key, Value >& Iterator< Key, Value >::operator++()
  {
    auto is_nil = [](NodeBase* n)
    {
      return n->left == n;
    };

    auto is_header = [](NodeBase* n)
    {
      return n->color == Color::RED && n->parent->parent == n;
    };

    if (is_header(node_))
      return *this;

    if (!is_nil(node_->right))
    {
      node_ = node_->right;
      while (!is_nil(node_->left))
        node_ = node_->left;
    }
    else
    {
      NodeBase* p = node_->parent;
      while (!is_nil(p) && !is_header(p) && node_ == p->right)
      {
        node_ = p;
        p = p->parent;
      }
      node_ = p;
    }

    return *this;
  }

  template< class Key, class Value >
  Iterator< Key, Value >& Iterator< Key, Value >::operator--()
  {
    auto is_nil = [](NodeBase* n)
    {
      return n->left == n;
    };

    auto is_header = [](NodeBase* n)
    {
      return n->color == Color::RED && n->parent->parent == n;
    };

    if (is_header(node_))
    {
      node_ = node_->right;
    }
    else if (!is_nil(node_->left))
    {
      node_ = node_->left;
      while (!is_nil(node_->right))
        node_ = node_->right;
    }
    else
    {
      NodeBase* p = node_->parent;
      while (!is_nil(p) && !is_header(p) && node_ == p->left)
      {
        node_ = p;
        p = p->parent;
      }
      node_ = p;
    }

    return *this;
  }

  template< class Key, class Value >
  CIterator< Key, Value >::CIterator(NodeBase* n) :
    node_(n)
  {
  }

  template< class Key, class Value >
  const std::pair< const Key, Value >& CIterator< Key, Value >::operator*() const
  {
    return static_cast<Node< Key, Value >*>(node_)->node;
  }

  template< class Key, class Value >
  const std::pair< const Key, Value >* CIterator< Key, Value >::operator->() const
  {
    return &(static_cast<Node< Key, Value >*>(node_)->node);
  }

  template< class Key, class Value >
  bool CIterator< Key, Value >::operator==(CIterator< Key, Value > other)
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool CIterator< Key, Value >::operator!=(CIterator< Key, Value > other)
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  CIterator< Key, Value >& CIterator< Key, Value >::operator++()
  {
    auto is_nil = [](NodeBase* n)
    {
      return n->left == n;
    };

    auto is_header = [](NodeBase* n)
    {
      return n->color == Color::RED && n->parent->parent == n;
    };

    if (is_header(node_))
      return *this;

    if (!is_nil(node_->right))
    {
      node_ = node_->right;
      while (!is_nil(node_->left))
        node_ = node_->left;
    }
    else
    {
      NodeBase* p = node_->parent;
      while (!is_nil(p) && !is_header(p) && node_ == p->right)
      {
        node_ = p;
        p = p->parent;
      }
      node_ = p;
    }

    return *this;
  }

  template< class Key, class Value >
  CIterator< Key, Value >& CIterator< Key, Value >::operator--()
  {
    auto is_nil = [](NodeBase* n)
    {
      return n->left == n;
    };

    auto is_header = [](NodeBase* n)
    {
      return n->color == Color::RED && n->parent->parent == n;
    };

    if (is_header(node_))
    {
      node_ = node_->right;
    }
    else if (!is_nil(node_->left))
    {
      node_ = node_->left;
      while (!is_nil(node_->right))
        node_ = node_->right;
    }
    else
    {
      NodeBase* p = node_->parent;
      while (!is_nil(p) && !is_header(p) && node_ == p->left)
      {
        node_ = p;
        p = p->parent;
      }
      node_ = p;
    }

    return *this;
  }
}

#endif
