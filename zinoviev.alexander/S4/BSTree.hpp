#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <functional>
#include <stdexcept>

namespace zinoviev
{
  template <class Key, class Value, class Compare = std::less<Key>>
  class BSTree
  {
    struct Node
    {
      std::pair<const Key, Value> data_;

      Node* left_;
      Node* right_;
      Node* parent_;

      Node(const Key& k, const Value& val, Node* p) :
        data_(k, val),
        left_(nullptr),
        right_(nullptr),
        parent_(p)
      {
      }
    };

    Node* root_;
    size_t size_;
    Compare compare_;

    size_t get_height(const Node* node) const
    {
      if (!node)
        return 0;

      size_t left_h = get_height(node->left_);
      size_t right_h = get_height(node->right_);

      return 1 + (left_h > right_h ? left_h : right_h);
    }

  public:

    class Iterator
    {
      Node* current_;
    public:

      friend class BSTree;

      Iterator(Node* cur) :
        current_(cur)
      {
      }

      std::pair<const Key, Value>& operator*()
      {
        return current_->data_;
      }

      std::pair<const Key, Value>* operator->()
      {
        return &(current_->data_);
      }

      Iterator& operator++()
      {
        if (current_->right_)
        {
          current_ = current_->right_;

          while (current_->left_)
            current_ = current_->left_;
        }
        else
        {
          Node* parent = current_->parent_;

          while (parent && current_ != parent->left_)
          {
            current_ = parent;
            parent = parent->parent_;
          }

          if (!parent)
            current_ = nullptr;
          else
            current_ = parent;
        }

        return *this;
      }

      Iterator operator++(int)
      {
        Iterator tmp = Iterator(current_);

        if (current_->right_)
        {
          current_ = current_->right_;

          while (current_->left_)
            current_ = current_->left_;
        }
        else
        {
          Node* parent = current_->parent_;

          while (parent && current_ != parent->left_)
          {
            current_ = parent;
            parent = parent->parent_;
          }

          if (!parent)
            current_ = nullptr;
          else
            current_ = parent;
        }

        return tmp;
      }

      bool operator==(const Iterator& other) const
      {
        return this->current_ == other.current_;
      }

      bool operator!=(const Iterator& other) const
      {
        return !(*this == other);
      }
    };

    class CIterator
    {
      const Node* current_;
    public:

      friend class BSTree;

      CIterator(const Node* cur) :
        current_(cur)
      {
      }

      const std::pair<const Key, Value>& operator*()
      {
        return current_->data_;
      }

      const std::pair<const Key, Value>* operator->()
      {
        return &(current_->data_);
      }

      CIterator& operator++()
      {
        if (current_->right_)
        {
          current_ = current_->right_;

          while (current_->left_)
            current_ = current_->left_;
        }
        else
        {
          Node* parent = current_->parent_;

          while (parent && current_ != parent->left_)
          {
            current_ = parent;
            parent = parent->parent_;
          }

          if (!parent)
            current_ = nullptr;
          else
            current_ = parent;
        }

        return *this;
      }

      CIterator operator++(int)
      {
        CIterator tmp = CIterator(current_);

        if (current_->right_)
        {
          current_ = current_->right_;

          while (current_->left_)
            current_ = current_->left_;
        }
        else
        {
          Node* parent = current_->parent_;

          while (parent && current_ != parent->left_)
          {
            current_ = parent;
            parent = parent->parent_;
          }

          if (!parent)
            current_ = nullptr;
          else
            current_ = parent;
        }

        return tmp;
      }

      bool operator==(const CIterator& other) const
      {
        return this->current_ == other.current_;
      }

      bool operator!=(const CIterator& other) const
      {
        return !(*this == other);
      }
    };

    BSTree(const Compare& com = Compare()) :
      root_(nullptr),
      size_(0),
      compare_(com)
    {
    }

    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;

    ~BSTree()
    {
      clear(root_);
    }

    Iterator begin();
    Iterator end();
    CIterator cbegin();
    CIterator cend();

    void push(Key k, Value v);
    Value get(Key k) const;
    void drop(Key k, Value& result);
    size_t height(CIterator it) const;
    size_t height() const;
    CIterator rotateLeft(CIterator it);
    CIterator rotateRight(CIterator it);
    CIterator rotateLargeLeft(CIterator it);
    CIterator rotateLargeRight(CIterator it);
    bool contains(const Key& k) const;
    void clear(Node* node);
  };

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::Iterator BSTree<Key, Value, Compare>::begin()
  {
    if (!root_)
      return Iterator{ nullptr };

    Node* node = root_;
    while (node->left_)
      node = node->left_;

    return Iterator{ node };
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::Iterator BSTree<Key, Value, Compare>::end()
  {
    return Iterator{ nullptr };
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator BSTree<Key, Value, Compare>::cbegin()
  {
    if (!root_)
      return CIterator{ nullptr };

    const Node* node = root_;
    while (node->left_)
      node = node->left_;

    return CIterator{ node };
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator BSTree<Key, Value, Compare>::cend()
  {
    return CIterator{ nullptr };
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::push(Key k, Value v)
  {
    Node* node = root_;
    Node* parent = nullptr;

    while (node)
    {
      parent = node;

      if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first))
      {
        node->data_.second = v;
        return;
      }
      else if (compare_(node->data_.first, k))
        node = node->right_;
      else
        node = node->left_;
    }

    Node* new_node = new Node(k, v, parent);

    if (!parent)
      root_ = new_node;
    else if (compare_(k, parent->data_.first))
      parent->left_ = new_node;
    else
      parent->right_ = new_node;

    ++size_;
  }

  template <class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::get(Key k) const
  {
    Node* node = root_;

    while (node)
    {
      if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first))
        return node->data_.second;
      else if (compare_(node->data_.first, k))
        node = node->right_;
      else
        node = node->left_;
    }

    throw std::out_of_range("key not found");
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::drop(Key k, Value& result)
  {
    Node* target = root_;
    Node* parent = nullptr;
    bool isLeft = false;

    while (target)
    {
      if (!compare_(target->data_.first, k) && !compare_(k, target->data_.first))
        break;
      parent = target;
      if (compare_(target->data_.first, k))
      {
        target = target->right_;
        isLeft = false;
      }
      else
      {
        target = target->left_;
        isLeft = true;
      }
    }

    if (!target)
      throw std::out_of_range("key not found");

    result = target->data_.second;

    if (!target->left_ || !target->right_)
    {
      Node* child = target->left_ ? target->left_ : target->right_;
      if (child)
        child->parent_ = parent;
      if (!parent)
        root_ = child;
      else if (isLeft)
        parent->left_ = child;
      else
        parent->right_ = child;
      delete target;
      --size_;
      return;
    }

    Node* succ = target->right_;
    while (succ->left_)
      succ = succ->left_;

    Node* succParent = succ->parent_;
    bool succIsLeft = (succParent && succ == succParent->left_);

    if (succParent)
    {
      if (succIsLeft)
        succParent->left_ = succ->right_;
      else
        succParent->right_ = succ->right_;
      if (succ->right_)
        succ->right_->parent_ = succParent;
    }

    succ->parent_ = parent;
    if (!parent)
      root_ = succ;
    else if (isLeft)
      parent->left_ = succ;
    else
      parent->right_ = succ;

    succ->left_ = target->left_;
    if (target->left_)
      target->left_->parent_ = succ;
    succ->right_ = target->right_;
    if (target->right_)
      target->right_->parent_ = succ;

    delete target;
    --size_;
  }

  template <class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::height(CIterator it) const
  {
    return get_height(it.current_);
  }

  template <class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::height() const
  {
    return get_height(root_);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator
    BSTree<Key, Value, Compare>::rotateLeft(CIterator it)
  {
    Node* cur = it.current_;
    Node* right = cur->right_;

    if (!cur->parent_)
      root_ = right;
    else if (cur == cur->parent_->left_)
      cur->parent_->left_ = right;
    else if (cur == cur->parent_->right_)
      cur->parent_->right_ = right;

    right->parent_ = cur->parent_;
    cur->right_ = right->left_;
    if(right->left_)
      right->left_->parent_ = cur;
    cur->parent_ = right;
    right->left_ = cur;

    return CIterator(right);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator
    BSTree<Key, Value, Compare>::rotateRight(CIterator it)
  {
    Node* cur = it.current_;
    Node* left = cur->left_;

    if (!cur->parent_)
      root_ = left;
    else if (cur == cur->parent_->left_)
      cur->parent_->left_ = left;
    else if (cur == cur->parent_->right_)
      cur->parent_->right_ = left;

    left->parent_ = cur->parent_;
    cur->left_ = left->right_;
    if (left->right_)
      left->right_->parent_ = cur;
    cur->parent_ = left;
    left->right_ = cur;

    return CIterator(left);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator
    BSTree<Key, Value, Compare>::rotateLargeLeft(CIterator it)
  {
    Node* node = it.current_;
    rotateRight(CIterator(node->left_));
    return rotateLeft(it);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::CIterator
    BSTree<Key, Value, Compare>::rotateLargeRight(CIterator it)
  {
    Node* node = it.current_;
    rotateLeft(CIterator(node->right_));
    return rotateRight(it);
  }

  template <class Key, class Value, class Compare>
  bool BSTree<Key, Value, Compare>::contains(const Key& k) const
  {
    Node* node = root_;

    while (node)
    {
      if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first))
        return true;
      else if (compare_(node->data_.first, k))
        node = node->right_;
      else
        node = node->left_;
    }

    return false;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::clear(Node* node)
  {
    if (!node)
      return;
    clear(node->left_);
    clear(node->right_);
    delete node;
  }
}
#endif
