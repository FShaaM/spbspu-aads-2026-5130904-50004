#ifndef BILIST_H
#define BILIST_H

namespace zinoviev
{
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
void BiList<T>::push_front(const T& d)
{
  Node* newNode = new Node(d);
  ++size_;

  if (!head)
  {
    tail = head = newNode;
  }
  else
  {
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
  }
}

template<class T>
void BiList<T>::push_back(const T& d)
{
  Node* newNode = new Node(d);
  ++size_;

  if (!tail)
  {
    tail = head = newNode;
  }
  else
  {
    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
  }
}

template<class T>
void BiList<T>::clear() noexcept
{
  Node* temp = head;

  while (head)
  {
    head = head->next;
    delete temp;
    temp = head;
  }

  size_ = 0;
  tail = head;
}

template<class T>
void BiList<T>::pop_front() noexcept
{
  if (!head)
    return;

  Node* temp = head;
  head = head->next;

  delete temp;

  --size_;

  if (head)
  {
    head->prev = nullptr;
  }
  else
  {
    tail = nullptr;
  }
}

template<class T>
void BiList<T>::pop_back() noexcept
{
  if (!tail)
    return;

  Node* temp = tail;
  tail = tail->prev;

  delete temp;

  --size_;

  if (tail)
  {
    tail->next = nullptr;
  }
  else
  {
    head = nullptr;
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
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(size_, other.size_);
}

template<class T>
BIter<T> BiList<T>::erase(BIter<T> x) noexcept
{
  if (x == end())
  {
    return x;
  }

  Node* node = x.node;
  Node* pr_node = node->prev;
  Node* nx_node = node->next;

  if (node == head)
  {
    this->pop_front();
  }
  else if (node == tail)
  {
    this->pop_back();
  }
  else
  {
    pr_node->next = node->next;
    nx_node->prev = pr_node;

    delete node;
    --size_;
  }
  return BIter<T>(nx_node);
}

