#ifndef BILIST_HPP
#define BILIST_HPP

#include <cstddef>
#include <algorithm>

namespace zinoviev
{
  template <class T> class BIter;
  template <class T> class CBIter;

  template <class T>
  class BiList
  {
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

    Node* head;
    Node* tail;
    size_t size_;

  public:
    friend class BIter<T>;
    friend class CBIter<T>;

    BiList() :
      head(nullptr),
      tail(nullptr),
      size_(0)
    {}

    BiList(const BiList<T>& other);
    BiList(BiList<T>&& other) noexcept;

    BiList& operator=(const BiList<T>& other);
    BiList& operator=(BiList<T>&& other) noexcept;

    ~BiList()
    {
      clear();
    }

    void push_front(const T& d);
    void push_back(const T& d);
    void clear() noexcept;
    size_t size() const noexcept;
    BIter<T> erase(BIter<T> pos) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(BiList& other) noexcept;

    BIter<T> begin();
    BIter<T> end();
    CBIter<T> cbegin() const;
    CBIter<T> cend() const;
  };

  template<class T>
  BiList<T>::BiList(const BiList<T>& other) :
    head(nullptr),
    tail(nullptr),
    size_(0)
  {
    for (Node* cur = other.head; cur != nullptr; cur = cur->next)
    {
      push_back(cur->val);
    }
  }

  template<class T>
  BiList<T>::BiList(BiList<T>&& other) noexcept :
    head(other.head),
    tail(other.tail),
    size_(other.size_)
  {
    other.head = other.tail = nullptr;
    other.size_ = 0;
  }

  template<class T>
  BiList<T>& BiList<T>::operator=(const BiList<T>& other)
  {
    if (this != &other)
    {
      BiList tmp(other);
      this->swap(tmp);
    }
    return *this;
  }

  template<class T>
  BiList<T>& BiList<T>::operator=(BiList<T>&& other) noexcept
  {
    if (this != &other)
    {
      std::swap(this->head, other.head);
      std::swap(this->tail, other.tail);
      std::swap(this->size_, other.size_);
    }
    return *this;
  }

  template<class T>
  void BiList<T>::push_front(const T& d)
  {
    Node* newNode = new Node(d);
    ++this->size_;

    if (!this->head)
    {
      this->tail = this->head = newNode;
    }
    else
    {
      newNode->next = this->head;
      this->head->prev = newNode;
      this->head = newNode;
    }
  }

  template<class T>
  void BiList<T>::push_back(const T& d)
  {
    Node* newNode = new Node(d);
    ++this->size_;

    if (!this->tail)
    {
      this->tail = this->head = newNode;
    }
    else
    {
      newNode->prev = this->tail;
      this->tail->next = newNode;
      this->tail = newNode;
    }
  }

  template<class T>
  void BiList<T>::clear() noexcept
  {
    while (head != nullptr)
      pop_front();
  }

  template<class T>
  void BiList<T>::pop_front() noexcept
  {
    if (!this->head)
      return;

    Node* temp = this->head;
    this->head = this->head->next;

    delete temp;

    --this->size_;

    if (this->head)
    {
      this->head->prev = nullptr;
    }
    else
    {
      this->tail = nullptr;
    }
  }

  template<class T>
  void BiList<T>::pop_back() noexcept
  {
    if (!this->tail)
      return;

    Node* temp = this->tail;
    this->tail = this->tail->prev;

    delete temp;

    --this->size_;

    if (this->tail)
    {
      this->tail->next = nullptr;
    }
    else
    {
      this->head = nullptr;
    }
  }

  template<class T>
  size_t BiList<T>::size() const noexcept
  {
    return this->size_;
  }

  template<class T>
  void BiList<T>::swap(BiList& other) noexcept
  {
    std::swap(this->head, other.head);
    std::swap(this->tail, other.tail);
    std::swap(this->size_, other.size_);
  }

  template<class T>
  BIter<T> BiList<T>::erase(BIter<T> x) noexcept
  {
    if (x == this->end())
    {
      return x;
    }

    Node* node = x.node;
    Node* pr_node = node->prev;
    Node* nx_node = node->next;

    if (node == this->head)
    {
      this->pop_front();
    }
    else if (node == this->tail)
    {
      this->pop_back();
    }
    else
    {
      pr_node->next = node->next;
      nx_node->prev = pr_node;

      delete node;
      --this->size_;
    }
    return BIter<T>(nx_node);
  }

  template <class T>
  BIter<T> BiList<T>::begin()
  {
    return BIter<T>(this->head);
  }

  template <class T>
  BIter<T> BiList<T>::end()
  {
    return BIter<T>(nullptr);
  }

  template <class T>
  CBIter<T> BiList<T>::cbegin() const
  {
    return CBIter<T>(this->head);
  }

  template <class T>
  CBIter<T> BiList<T>::cend() const
  {
    return CBIter<T>(nullptr);
  }

  // ---------- BIter ----------
  template <class T>
  class BIter
  {
    typename BiList<T>::Node* node;
    friend class BiList<T>;

  public:
    BIter() : node(nullptr) {}
    BIter(typename BiList<T>::Node* n) : node(n) {}

    BIter<T>& operator++();
    BIter<T>& operator--();
    BIter<T> operator++(int);
    T& operator*() const;
    T* operator->() const;
    bool operator==(const BIter<T>& x) const;
    bool operator!=(const BIter<T>& x) const;

    bool has_next();
    BIter<T> next();
  };

  template <class T>
  BIter<T>& BIter<T>::operator++()
  {
    node = node->next;
    return *this;
  }

  template <class T>
  BIter<T> BIter<T>::operator++(int)
  {
    BIter<T> temp = *this;
    node = node->next;
    return temp;
  }

  template <class T>
  BIter<T>& BIter<T>::operator--()
  {
    node = node->prev;
    return *this;
  }

  template <class T>
  T& BIter<T>::operator*() const
  {
    return node->val;
  }

  template <class T>
  T* BIter<T>::operator->() const
  {
    return &(node->val);
  }

  template <class T>
  bool BIter<T>::operator==(const BIter<T>& x) const
  {
    return node == x.node;
  }

  template <class T>
  bool BIter<T>::operator!=(const BIter<T>& x) const
  {
    return !(*this == x);
  }

  template <class T>
  bool BIter<T>::has_next()
  {
    if (!node) return false;
    return node->next;
  }

  template <class T>
  BIter<T> BIter<T>::next()
  {
    if (!has_next()) return BIter<T>();
    return BIter<T>(node->next);
  }

  // ---------- CBIter ----------
  template <class T>
  class CBIter
  {
    typename BiList<T>::Node const* node;
    friend class BiList<T>;

  public:
    CBIter() : node(nullptr) {}
    CBIter(typename BiList<T>::Node const* n) : node(n) {}
    CBIter(BIter<T> const& it) : node(it.node) {}

    CBIter<T>& operator++();
    CBIter<T> operator++(int);
    CBIter<T>& operator--();
    CBIter<T> operator--(int);
    const T& operator*() const;
    const T* operator->() const;
    bool operator==(CBIter<T> const& other) const;
    bool operator!=(CBIter<T> const& other) const;

    bool has_next() const;
    CBIter<T> next() const;
  };

  template <class T>
  CBIter<T>& CBIter<T>::operator++()
  {
    node = node->next;
    return *this;
  }

  template <class T>
  CBIter<T> CBIter<T>::operator++(int)
  {
    CBIter<T> temp = *this;
    ++(*this);
    return temp;
  }

  template <class T>
  CBIter<T>& CBIter<T>::operator--()
  {
    node = node->prev;
    return *this;
  }

  template <class T>
  CBIter<T> CBIter<T>::operator--(int)
  {
    CBIter<T> temp = *this;
    --(*this);
    return temp;
  }

  template <class T>
  const T& CBIter<T>::operator*() const
  {
    return node->val;
  }

  template <class T>
  const T* CBIter<T>::operator->() const
  {
    return &(node->val);
  }

  template <class T>
  bool CBIter<T>::operator==(CBIter<T> const& other) const
  {
    return node == other.node;
  }

  template <class T>
  bool CBIter<T>::operator!=(CBIter<T> const& other) const
  {
    return !(*this == other);
  }

  template <class T>
  bool CBIter<T>::has_next() const
  {
    return node && node->next;
  }

  template <class T>
  CBIter<T> CBIter<T>::next() const
  {
    if (!has_next()) return CBIter<T>();
    return CBIter<T>(node->next);
  }

}

#endif // BILIST_H
