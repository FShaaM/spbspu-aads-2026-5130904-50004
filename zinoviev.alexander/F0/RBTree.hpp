#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "Node.hpp"
#include "Iterators.hpp"

namespace zinoviev
{

  template< class Key, class Value, class Compare = std::less< Key > >
  class RBTree
  {
    NodeBase header_;
    NodeBase nil_;
    Compare comp_;

    size_t size_;

    void leftRotate(NodeBase* x);
    void rightRotate(NodeBase* x);

    void insertFixup(NodeBase* x);
    void eraseFixup(NodeBase* x);

    void transplant(NodeBase* x, NodeBase* y);
    NodeBase* minimum(NodeBase* x);

    NodeBase* findNodeBase(const Key& key);

    void clearRecursive(NodeBase* x);

  public:

    friend class Iterator< Key, Value >;
    friend class CIterator< Key, Value >;

    RBTree();
    ~RBTree();

    bool empty() const;
    size_t size() const;
    std::pair< Iterator< Key, Value >, bool > insert(const Key& key, const Value& value);
    Iterator< Key, Value > find(const Key& key);
    CIterator< Key, Value > find(const Key& key) const;
    void erase(const Key& key);
    void erase(Iterator<Key, Value> pos);
    void clear();

    Iterator< Key, Value > begin();
    Iterator< Key, Value > end();
    CIterator< Key, Value > cbegin() const;
    CIterator< Key, Value > cend() const;
  };

  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::leftRotate(NodeBase* x)
  {
    NodeBase* father = x->parent;
    NodeBase* new_x = x->right;

    if (father == &header_)
      father->parent = new_x;
    else if (father->left == x)
      father->left = new_x;
    else
      father->right = new_x;

    x->right = new_x->left;
    if (new_x->left != &nil_)
      new_x->left->parent = x;
    new_x->left = x;

    x->parent = new_x;
    new_x->parent = father;
  }

  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::rightRotate(NodeBase* x)
  {
    NodeBase* father = x->parent;
    NodeBase* new_x = x->left;

    if (father == &header_)
      father->parent = new_x;
    else if (father->left == x)
      father->left = new_x;
    else
      father->right = new_x;

    x->left = new_x->right;
    if (new_x->right != &nil_)
      new_x->right->parent = x;
    new_x->right = x;

    x->parent = new_x;
    new_x->parent = father;
  }

  template<class Key, class Value, class Compare>
  void RBTree<Key, Value, Compare>::insertFixup(NodeBase* x)
  {
    NodeBase* node = x;
    NodeBase* father = node->parent;
    NodeBase* uncle = nullptr;

    while (father != &header_ && father->color == Color::RED)
    {
      bool father_is_left = false;
      if (father == father->parent->left)
      {
        uncle = father->parent->right;
        father_is_left = true;
      }
      else
        uncle = father->parent->left;

      if (uncle->color == Color::RED)
      {
        uncle->color = Color::BLACK;
        father->color = Color::BLACK;
        father->parent->color = Color::RED;

        node = father->parent;
        father = node->parent;
      }
      else
      {
        if (father_is_left)
        {
          if (node == father->right)
          {
            leftRotate(father);
            node = father;
            father = node->parent;
          }

          father->color = Color::BLACK;
          father->parent->color = Color::RED;

          rightRotate(father->parent);
          break;
        }
        else
        {
          if (node == father->left)
          {
            rightRotate(father);
            node = father;
            father = node->parent;
          }

          father->color = Color::BLACK;
          father->parent->color = Color::RED;

          leftRotate(father->parent);
          break;
        }
      }
    }

    header_.parent->color = Color::BLACK;
  }

  template<class Key, class Value, class Compare>
  void RBTree<Key, Value, Compare>::eraseFixup(NodeBase* x)
  {
    while (x != header_.parent && x->color == Color::BLACK)
    {
      if (x == x->parent->left)
      {
        NodeBase* w = x->parent->right;
        if (w->color == Color::RED)
        {
          w->color = Color::BLACK;
          x->parent->color = Color::RED;
          leftRotate(x->parent);
          w = x->parent->right;
        }

        if (w->left->color == Color::BLACK && w->right->color == Color::BLACK)
        {
          w->color = Color::RED;
          x = x->parent;
        }
        else
        {
          if (w->right->color == Color::BLACK)
          {
            w->left->color = Color::BLACK;
            w->color = Color::RED;
            rightRotate(w);
            w = x->parent->right;
          }

          w->color = x->parent->color;
          x->parent->color = Color::BLACK;
          w->right->color = Color::BLACK;
          leftRotate(x->parent);
          x = header_.parent;
        }
      }
      else
      {
        NodeBase* w = x->parent->left;
        if (w->color == Color::RED)
        {
          w->color = Color::BLACK;
          x->parent->color = Color::RED;
          rightRotate(x->parent);
          w = x->parent->left;
        }

        if (w->right->color == Color::BLACK && w->left->color == Color::BLACK)
        {
          w->color = Color::RED;
          x = x->parent;
        }
        else
        {
          if (w->left->color == Color::BLACK)
          {
            w->right->color = Color::BLACK;
            w->color = Color::RED;
            leftRotate(w);
            w = x->parent->left;
          }

          w->color = x->parent->color;
          x->parent->color = Color::BLACK;
          w->left->color = Color::BLACK;
          rightRotate(x->parent);
          x = header_.parent;
        }
      }
    }
    x->color = Color::BLACK;
  }

  template<class Key, class Value, class Compare>
  void RBTree<Key, Value, Compare>::transplant(NodeBase* x, NodeBase* y)
  {
    NodeBase* father_x = x->parent;

    if (father_x == &header_)
      header_.parent = y;
    else if (x == father_x->left)
      father_x->left = y;
    else
      father_x->right = y;

    if (y != &nil_)
      y->parent = father_x;
  }

  template<class Key, class Value, class Compare>
  NodeBase* RBTree<Key, Value, Compare>::minimum(NodeBase* x)
  {
    if (x == &nil_)
      return &nil_;

    while (x->left != &nil_)
      x = x->left;

    return x;
  }

  template<class Key, class Value, class Compare>
  NodeBase* RBTree<Key, Value, Compare>::findNodeBase(const Key& key)
  {
    NodeBase* node = header_.parent;

    while (node != &nil_)
    {
      const Key& curr = static_cast<Node<Key, Value>*>(node)->node.first;

      if (!comp_(key, curr) && !comp_(curr, key))
        return node;
      else if (comp_(key, curr))
        node = node->left;
      else
        node = node->right;
    }

    return &nil_;
  }

  template<class Key, class Value, class Compare>
  void RBTree<Key, Value, Compare>::clearRecursive(NodeBase* x)
  {
    if (x == &nil_ || x == nullptr)
      return;

    clearRecursive(x->left);
    clearRecursive(x->right);
    delete static_cast<Node< Key, Value >*>(x);
  }

  template< class Key, class Value, class Compare>
  RBTree< Key, Value, Compare >::RBTree() :
    header_(NodeBase(Color::RED)),
    nil_(NodeBase(Color::BLACK)),
    size_(0)
  {
    header_.parent = &nil_;
    header_.left = header_.right = &header_;

    nil_.parent = nil_.left = nil_.right = &nil_;
  }

  template< class Key, class Value, class Compare >
  RBTree< Key, Value, Compare >::~RBTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  bool RBTree< Key, Value, Compare >::empty() const
  {
    return header_.parent == &nil_;
  }

  template< class Key, class Value, class Compare >
  size_t RBTree< Key, Value, Compare >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  std::pair< Iterator< Key, Value >, bool >
    RBTree< Key, Value, Compare >::insert(const Key& key, const Value& value)
  {
    NodeBase* place = header_.parent;
    NodeBase* father = &header_;

    while (place != &nil_)
    {
      father = place;

      const Key& curr_key = (static_cast<Node< Key, Value >*>(place))->node.first;
      if (comp_(key, curr_key))
        place = place->left;
      else
        place = place->right;
    }

    if (father != &header_)
    {
      Iterator<Key, Value> it(father);

      const Key& curr_key = it->first;
      if (!comp_(curr_key, key) && !comp_(key, curr_key))
      {
        return std::make_pair(it, false);
      }

      if (it != begin())
      {
        --it;
        const Key& c_key = it->first;
        if (!comp_(c_key, key) && !comp_(key, c_key))
        {
          return std::make_pair(it, false);
        }
      }
    }

    NodeBase* node = new Node< Key, Value >(key, value, &nil_, &nil_, &nil_, Color::RED);
    node->parent = father;

    if (father == &header_)
    {
      header_.parent = node;
      header_.left = header_.right = node;
    }
    else
    {
      const Key& father_key = static_cast<Node< Key, Value >*>(father)->node.first;
      if (comp_(key, father_key))
      {
        father->left = node;
        if (father == header_.left)
          header_.left = node;
      }
      else
      {
        father->right = node;
        if (father == header_.right)
          header_.right = node;
      }
    }

    insertFixup(node);
    ++size_;
    return std::make_pair(Iterator<Key, Value>(node), true);
  }

  template< class Key, class Value, class Compare >
  Iterator< Key, Value > RBTree< Key, Value, Compare >::find(const Key& key)
  {
    NodeBase* node = findNodeBase(key);

    if (node != &nil_)
      return Iterator<Key, Value>(node);
    else
      return end();
  }

  template< class Key, class Value, class Compare >
  CIterator< Key, Value > RBTree< Key, Value, Compare >::find(const Key& key) const
  {
    NodeBase* node = header_.parent;
    while (node != &nil_)
    {
      const Key& curr = static_cast<Node<Key, Value>*>(node)->node.first;
      if (!comp_(key, curr) && !comp_(curr, key))
        return CIterator<Key, Value>(node);
      else if (comp_(key, curr))
        node = node->left;
      else
        node = node->right;
    }
    return cend();
  }

  template< class Key, class Value, class Compare >
  void RBTree<Key, Value, Compare>::erase(const Key& key)
  {
    NodeBase* del = findNodeBase(key);

    if (del == &nil_)
      return;
    Color del_color = del->color;

    NodeBase* balance = &nil_;

    if (del->left == &nil_)
    {
      balance = del->right;
      transplant(del, del->right);
    }
    else if (del->right == &nil_)
    {
      balance = del->left;
      transplant(del, del->left);
    }
    else
    {
      NodeBase* new_node = minimum(del->right);
      del_color = new_node->color;
      balance = new_node->right;

      if (new_node->parent != del)
      {
        transplant(new_node, new_node->right);
        new_node->right = del->right;
        del->right->parent = new_node;
      }

      transplant(del, new_node);
      new_node->left = del->left;
      del->left->parent = new_node;
      new_node->color = del->color;
    }

    delete static_cast<Node< Key, Value >*>(del);
    --size_;

    if (header_.parent == &nil_)
    {
      header_.left = header_.right = &header_;
    }
    else
    {
      header_.left = minimum(header_.parent);

      NodeBase* max = header_.parent;
      while (max->right != &nil_)
        max = max->right;
      header_.right = max;
    }

    if (del_color == Color::BLACK)
      eraseFixup(balance);
  }

  template< class Key, class Value, class Compare >
  void RBTree<Key, Value, Compare>::erase(Iterator<Key, Value> pos)
  {
    erase(pos->first);
  }

  template<class Key, class Value, class Compare>
  void RBTree<Key, Value, Compare>::clear()
  {
    clearRecursive(header_.parent);
    header_.parent = &nil_;
    header_.left = header_.right = &header_;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  Iterator< Key, Value > RBTree< Key, Value, Compare >::begin()
  {
    Iterator< Key, Value > b(header_.left);
    return b;
  }

  template< class Key, class Value, class Compare >
  Iterator< Key, Value > RBTree< Key, Value, Compare >::end()
  {
    Iterator< Key, Value > e(&header_);
    return e;
  }

  template< class Key, class Value, class Compare >
  CIterator< Key, Value > RBTree< Key, Value, Compare >::cbegin() const
  {
    CIterator< Key, Value > b(header_.left);
    return b;
  }

  template< class Key, class Value, class Compare >
  CIterator< Key, Value > RBTree< Key, Value, Compare >::cend() const
  {
    NodeBase* header = header_.parent->parent;
    CIterator< Key, Value > e(header);
    return e;
  }
}

#endif
