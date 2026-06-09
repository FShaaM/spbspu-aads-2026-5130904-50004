#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <new>
#include "Vector_Iterators.hpp"

namespace zinoviev
{
  template < class T >
  class Vector
  {
    T* data_;
    size_t size_;
    size_t capacity_;

  public:

    Vector();
    ~Vector();
    Vector(const Vector< T >& r);
    Vector(Vector< T >&& r) noexcept;
    Vector(size_t size, const T& val);
    explicit Vector(size_t size);

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    void reserve(size_t cap);

    void push_back(const T& x);
    void pushBackRepeat(const T& x, size_t k);
    void pop_back();
    void push_front(const T& x);

    void insert(size_t id, const T& t);
    void insert(size_t id, const Vector< T >& rhs, size_t beg, size_t end);
    void erase(size_t id);
    void erase(size_t beg, size_t end);

    void swap(Vector< T >& rhs) noexcept;

    T& at(size_t id);
    const T& at(size_t id) const;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;

    Vector<T>& operator=(const Vector< T >&);
    Vector<T>& operator=(Vector< T >&&) noexcept;

    VectorIterator< T > begin() noexcept;
    VectorConstIterator< T > begin() const noexcept;
    VectorConstIterator< T > cbegin() const noexcept;

    VectorIterator< T > end() noexcept;
    VectorConstIterator< T > end() const noexcept;
    VectorConstIterator< T > cend() const noexcept;

    VectorIterator< T > insert(VectorConstIterator< T > pos, const T& value);
    VectorIterator< T > insert(VectorConstIterator< T > pos, size_t count, const T& value);
    template < class InputIt >
    VectorIterator< T > insert(VectorConstIterator< T > pos, InputIt first, InputIt last);

    VectorIterator< T > erase(VectorConstIterator< T > pos);
    VectorIterator< T > erase(VectorConstIterator< T > first, VectorConstIterator< T > last);
  };
}

template < class T >
zinoviev::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{
}

template < class T >
zinoviev::Vector< T >::Vector(size_t size) :
  data_(size ? static_cast<T*>(operator new[](sizeof(T)* size)) : nullptr),
  size_(size),
  capacity_(size)
{
  size_t i = 0;

  try
  {
    for (; i < size_; ++i)
      new (data_ + i) T();
  }
  catch (...)
  {
    for (size_t j; j < i; ++j)
      (data_ + j)->~T();

    ::operator delete[](data_);
    throw;
  }
}

template < class T >
zinoviev::Vector< T >::Vector(size_t size, const T& val) :
  data_(size ? static_cast<T*>(operator new[](sizeof(T)* size)) : nullptr),
  size_(size),
  capacity_(size)

{
  size_t i = 0;
  try
  {
    for (; i < size_; ++i)
      new (data_ + i) T(val);
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
      (data_ + j)->~T();

    ::operator delete[](data_);
    throw;
  }
}

template < class T >
zinoviev::Vector< T >::Vector(const Vector< T >& r) :
  data_(r.size_ ? static_cast<T*>(operator new[](sizeof(T)* r.size_)) : nullptr),
  size_(r.size_),
  capacity_(r.size_)
{
  size_t i = 0;

  try
  {
    for (; i < size_; ++i)
      new (data_ + i) T(r.data_[i]);
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
      (data_ + j)->~T();

    ::operator delete[](data_);
    throw;
  }
}

template < class T >
zinoviev::Vector< T >::Vector(Vector< T >&& rhs) noexcept :
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.capacity_ = rhs.size_ = 0;
}

template < class T >
zinoviev::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i)
    (data_ + i)->~T();

  ::operator delete[](data_);
}

template < class T >
bool zinoviev::Vector< T >::empty() const noexcept
{
  return !size_;
}

template < class T >
size_t zinoviev::Vector< T >::size() const noexcept
{
  return size_;
}

template < class T >
size_t zinoviev::Vector< T >::capacity() const noexcept
{
  return capacity_;
}

template < class T >
void zinoviev::Vector< T >::reserve(size_t cap)
{
  if (cap <= capacity_)
    return;

  T* new_data = nullptr;

  size_t i = 0;
  try
  {
    new_data = static_cast<T*>(operator new[](sizeof(T)* cap));

    for (; i < size_; ++i)
    {
      new (new_data + i) T(data_[i]);
    }

    for (size_t j = 0; j < size_; ++j)
    {
      (data_ + j)->~T();
    }
    ::operator delete[](data_);

    data_ = new_data;
    capacity_ = cap;
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
    {
      (new_data + j)->~T();
    }
    ::operator delete[](new_data);
    throw;
  }
}

template < class T >
void zinoviev::Vector< T >::push_back(const T& x)
{
  if (size_ == capacity_)
  {
    size_t new_cap = (capacity_ == 0 ? 2 : capacity_ * 2);
    T* new_data = nullptr;
    size_t old_size = size_;

    size_t i = 0;
    try
    {
      new_data = static_cast<T*>(operator new[](sizeof(T)* new_cap));

      for (; i < old_size; ++i)
      {
        new (new_data + i) T(data_[i]);
      }
      new (new_data + old_size) T(x);
      i = old_size + 1;

      for (size_t j = 0; j < old_size; ++j)
      {
        (data_ + j)->~T();
      }
      ::operator delete[](data_);

      data_ = new_data;
      ++size_;
      capacity_ = new_cap;
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        (new_data + j)->~T();
      }
      ::operator delete[](new_data);
      throw;
    }
  }
  else
  {
    new (data_ + size_) T(x);
    ++size_;
  }
}

template < class T >
void zinoviev::Vector< T >::pushBackRepeat(const T& x, size_t k)
{
  if (k == 0)
    return;

  size_t new_size = size_ + k;
  reserve(new_size);

  size_t i = 0;
  try
  {
    for (; i < k; ++i)
      new (data_ + size_ + i) T(x);
    size_ = new_size;
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
      (data_ + size_ + j)->~T();
    throw;
  }
}

template < class T >
void zinoviev::Vector< T >::pop_back()
{
  if (!data_ || !size_)
    return;

  --size_;
  data_[size_].~T();
}

template < class T >
void zinoviev::Vector< T >::push_front(const T& x)
{
  size_t new_cap = (capacity_ == 0 ? 2 : capacity_ + 20);
  T* new_data = nullptr;
  size_t old_size = size_;

  size_t i = 0;
  try
  {
    new_data = static_cast<T*>(operator new[](sizeof(T)* new_cap));

    new (new_data + i) T(x);
    ++i;
    for (size_t j = 0; j < old_size; ++j)
    {
      new (new_data + j + 1) T(data_[j]);
      ++i;
    }

    for (size_t j = 0; j < old_size; ++j)
    {
      (data_ + j)->~T();
    }
    ::operator delete[](data_);

    data_ = new_data;
    ++size_;
    capacity_ = new_cap;
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
    {
      (new_data + j)->~T();
    }
    ::operator delete[](new_data);
    throw;
  }
}

template < class T >
void zinoviev::Vector< T >::insert(size_t id, const T& t)
{
  if (id >= size_)
  {
    push_back(t);
    return;
  }
  Vector< T > v;
  v.reserve(size_ + 1);

  for (size_t i = 0; i < id; ++i)
    v.push_back(data_[i]);

  v.push_back(t);

  for (size_t i = id; i < size_; ++i)
    v.push_back(data_[i]);

  swap(v);
}

template < class T >
void zinoviev::Vector< T >::insert(size_t id, const Vector< T >& rhs, size_t beg, size_t end)
{
  if (beg > end)
    std::swap(beg, end);
  if (end >= rhs.size_)
    end = rhs.size_;
  if (beg >= rhs.size_ || beg == end)
    return;
  if (id > size_)
    id = size_;

  Vector< T > v;
  size_t new_size = size_ + end - beg;
  v.reserve(new_size);

  for (size_t i = 0; i < id; ++i)
    v.push_back(data_[i]);

  for (size_t i = beg; i < end; ++i)
    v.push_back(rhs.data_[i]);

  for (size_t i = id; i < size_; ++i)
    v.push_back(data_[i]);

  swap(v);
}

template < class T >
void zinoviev::Vector< T >::erase(size_t id)
{
  if (size_ <= id)
    return;

  Vector< T > v;

  for (size_t i = 0; i < id; ++i)
    v.push_back(data_[i]);
  for (size_t i = id + 1; i < size_; ++i)
    v.push_back(data_[i]);

  swap(v);
}

template < class T >
void zinoviev::Vector< T >::erase(size_t beg, size_t end)
{
  if (beg > end)
    std::swap(beg, end);
  if (size_ <= beg || beg == end)
    return;

  Vector< T > v;

  for (size_t i = 0; i < beg; ++i)
    v.push_back(data_[i]);
  for (size_t i = end; i < size_; ++i)
    v.push_back(data_[i]);

  swap(v);
}

template < class T >
T& zinoviev::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T >* cthis = this;
  const T& cr = (*cthis)[id];
  T& r = const_cast<T&>(cr);
  return r;
}

template < class T >
const T& zinoviev::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template < class T >
T& zinoviev::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  const T& cr = cthis->at(id);
  T& r = const_cast<T&>(cr);
  return r;
}

template < class T >
const T& zinoviev::Vector< T >::at(size_t id) const
{
  if (id < size())
    return (*this)[id];

  throw std::logic_error("id out of buond");
}

template < class T >
zinoviev::Vector< T >& zinoviev::Vector< T >::operator=(const Vector< T >& rhs)
{
  if (this == std::addressof(rhs))
    return *this;

  Vector< T > cpy(rhs);
  swap(cpy);

  return *this;
}

template < class T >
zinoviev::Vector< T >& zinoviev::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this == std::addressof(rhs))
    return *this;

  Vector< T > cpy = std::move(rhs);
  swap(cpy);
  return *this;
}

template < class T >
void zinoviev::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template < class T >
zinoviev::VectorIterator< T > zinoviev::Vector< T >::begin() noexcept
{
  return VectorIterator< T >(data_);
}

template < class T >
zinoviev::VectorConstIterator< T > zinoviev::Vector< T >::begin() const noexcept
{
  return VectorConstIterator< T >(data_);
}

template < class T >
zinoviev::VectorConstIterator< T > zinoviev::Vector< T >::cbegin() const noexcept
{
  return VectorConstIterator< T >(data_);
}

template < class T >
zinoviev::VectorIterator< T > zinoviev::Vector< T >::end() noexcept
{
  return VectorIterator< T >(data_ + size_);
}

template < class T >
zinoviev::VectorConstIterator< T > zinoviev::Vector< T >::end() const noexcept
{
  return VectorConstIterator< T >(data_ + size_);
}

template < class T >
zinoviev::VectorConstIterator< T > zinoviev::Vector< T >::cend() const noexcept
{
  return VectorConstIterator< T >(data_ + size_);
}

template < class T >
zinoviev::VectorIterator< T >
zinoviev::Vector< T >::insert(VectorConstIterator< T > pos, const T& value)
{
  size_t index = pos - cbegin();
  if (index >= size_)
  {
    push_back(value);
    return begin() + index;
  }
  if (size_ == capacity_)
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);

  new (data_ + size_) T(std::move(data_[size_ - 1]));
  for (size_t i = size_ - 1; i > index; --i)
    data_[i] = std::move(data_[i - 1]);
  data_[index] = value;
  ++size_;
  return begin() + index;
}

template < class T >
zinoviev::VectorIterator< T >
zinoviev::Vector< T >::insert(VectorConstIterator< T > pos, size_t count, const T& value)
{
  size_t index = pos - cbegin();
  if (count == 0)
    return begin() + index;
  size_t new_size = size_ + count;
  if (new_size > capacity_)
    reserve(new_size);

  for (size_t i = size_; i > index; --i)
  {
    if (i + count - 1 < new_size)
      new (data_ + i + count - 1) T(std::move(data_[i - 1]));
    else
      data_[i + count - 1] = std::move(data_[i - 1]);
  }
  for (size_t i = 0; i < count; ++i)
    data_[index + i] = value;
  size_ = new_size;
  return begin() + index;
}

template < class T >
template < class InputIt >
zinoviev::VectorIterator< T >
zinoviev::Vector< T >::insert(VectorConstIterator< T > pos, InputIt first, InputIt last)
{
  size_t index = pos - cbegin();
  size_t count = std::distance(first, last);
  if (count == 0)
    return begin() + index;
  size_t new_size = size_ + count;
  if (new_size > capacity_)
    reserve(new_size);

  for (size_t i = size_; i > index; --i)
  {
    if (i + count - 1 < new_size)
      new (data_ + i + count - 1) T(std::move(data_[i - 1]));
    else
      data_[i + count - 1] = std::move(data_[i - 1]);
  }
  size_t i = 0;
  try
  {
    for (auto it = first; it != last; ++it, ++i)
      new (data_ + index + i) T(*it);
    size_ = new_size;
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
      (data_ + index + j)->~T();
    throw;
  }
  return begin() + index;
}

template < class T >
zinoviev::VectorIterator< T >
zinoviev::Vector< T >::erase(VectorConstIterator< T > pos)
{
  size_t index = pos - cbegin();
  if (index >= size_)
    return end();
  (data_ + index)->~T();
  for (size_t i = index + 1; i < size_; ++i)
  {
    new (data_ + i - 1) T(std::move(data_[i]));
    (data_ + i)->~T();
  }
  --size_;
  return begin() + index;
}

template < class T >
zinoviev::VectorIterator< T >
zinoviev::Vector< T >::erase(VectorConstIterator< T > first, VectorConstIterator< T > last)
{
  size_t f = first - cbegin();
  size_t l = last - cbegin();
  if (f >= size_ || f >= l)
    return begin() + f;
  if (l > size_)
    l = size_;
  for (size_t i = f; i < l; ++i)
    (data_ + i)->~T();
  size_t shift = l - f;
  for (size_t i = l; i < size_; ++i)
  {
    new (data_ + i - shift) T(std::move(data_[i]));
    (data_ + i)->~T();
  }
  size_ -= shift;
  return begin() + f;
}

#endif
