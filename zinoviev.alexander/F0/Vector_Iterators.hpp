#ifndef VECTOR_ITERATORS_HPP
#define VECTOR_ITERATORS_HPP

#include <cstddef>
#include <iterator>

namespace zinoviev
{
  template < class T > class Vector;
  template < class T > class VectorConstIterator;

  template < class T >
  class VectorIterator : public std::iterator< std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T& >
  {
    T* ptr_;
    friend class Vector< T >;
    friend class VectorConstIterator< T >;

  public:
    VectorIterator() :
      ptr_(nullptr)
    {
    }

    explicit VectorIterator(T* p) :
      ptr_(p)
    {
    }

    VectorIterator< T >& operator++()
    {
      ++ptr_;
      return *this;
    }

    VectorIterator< T > operator++(int)
    {
      VectorIterator< T > temp = *this;
      ++(*this);
      return temp;
    }

    VectorIterator< T >& operator--()
    {
      --ptr_;
      return *this;
    }

    VectorIterator< T > operator--(int)
    {
      VectorIterator< T > temp = *this;
      --(*this);
      return temp;
    }

    VectorIterator< T > operator+(std::ptrdiff_t n) const
    {
      return VectorIterator< T >(ptr_ + n);
    }

    VectorIterator< T > operator-(std::ptrdiff_t n) const
    {
      return VectorIterator< T >(ptr_ - n);
    }

    std::ptrdiff_t operator-(const VectorIterator< T >& other) const
    {
      return ptr_ - other.ptr_;
    }

    VectorIterator< T >& operator+=(std::ptrdiff_t n)
    {
      ptr_ += n;
      return *this;
    }

    VectorIterator< T >& operator-=(std::ptrdiff_t n)
    {
      ptr_ -= n;
      return *this;
    }

    T& operator*() const
    {
      return *ptr_;
    }

    T* operator->() const
    {
      return ptr_;
    }

    T& operator[](std::ptrdiff_t n) const
    {
      return ptr_[n];
    }

    bool operator==(const VectorIterator< T >& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const VectorIterator< T >& other) const
    {
      return !(*this == other);
    }

    bool operator<(const VectorIterator< T >& other) const
    {
      return ptr_ < other.ptr_;
    }

    bool operator>(const VectorIterator< T >& other) const
    {
      return other < *this;
    }

    bool operator<=(const VectorIterator< T >& other) const
    {
      return !(other < *this);
    }

    bool operator>=(const VectorIterator< T >& other) const
    {
      return !(*this < other);
    }
  };

  template < class T >
  class VectorConstIterator : public std::iterator< std::random_access_iterator_tag, const T, std::ptrdiff_t, const T*, const T& >
  {
    const T* ptr_;
    friend class Vector< T >;

  public:
    VectorConstIterator() :
      ptr_(nullptr)
    {
    }

    explicit VectorConstIterator(const T* p) :
      ptr_(p)
    {
    }

    VectorConstIterator(const VectorIterator< T >& it) :
      ptr_(it.ptr_)
    {
    }

    VectorConstIterator< T >& operator++()
    {
      ++ptr_;
      return *this;
    }

    VectorConstIterator< T > operator++(int)
    {
      VectorConstIterator< T > temp = *this;
      ++(*this);
      return temp;
    }

    VectorConstIterator< T >& operator--()
    {
      --ptr_;
      return *this;
    }

    VectorConstIterator< T > operator--(int)
    {
      VectorConstIterator< T > temp = *this;
      --(*this);
      return temp;
    }

    VectorConstIterator< T > operator+(std::ptrdiff_t n) const
    {
      return VectorConstIterator< T >(ptr_ + n);
    }

    VectorConstIterator< T > operator-(std::ptrdiff_t n) const
    {
      return VectorConstIterator< T >(ptr_ - n);
    }

    std::ptrdiff_t operator-(const VectorConstIterator< T >& other) const
    {
      return ptr_ - other.ptr_;
    }

    VectorConstIterator< T >& operator+=(std::ptrdiff_t n)
    {
      ptr_ += n;
      return *this;
    }

    VectorConstIterator< T >& operator-=(std::ptrdiff_t n)
    {
      ptr_ -= n;
      return *this;
    }

    const T& operator*() const
    {
      return *ptr_;
    }

    const T* operator->() const
    {
      return ptr_;
    }

    const T& operator[](std::ptrdiff_t n) const
    {
      return ptr_[n];
    }

    bool operator==(const VectorConstIterator< T >& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const VectorConstIterator< T >& other) const
    {
      return !(*this == other);
    }

    bool operator<(const VectorConstIterator< T >& other) const
    {
      return ptr_ < other.ptr_;
    }

    bool operator>(const VectorConstIterator< T >& other) const
    {
      return other < *this;
    }

    bool operator<=(const VectorConstIterator< T >& other) const
    {
      return !(other < *this);
    }

    bool operator>=(const VectorConstIterator< T >& other) const
    {
      return !(*this < other);
    }
  };
}

#endif
