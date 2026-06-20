#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../S1/BiList.hpp"

namespace zinoviev
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;

    void push(const T& value);
    void push(T&& value);
    void pop();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    BiList< T > data_;
  };

  template< class T >
  void Stack< T >::push(const T& value)
  {
    data_.push_front(value);
  }

  template< class T >
  void Stack< T >::push(T&& value)
  {
    data_.push_front(std::move(value));
  }

  template< class T >
  void Stack< T >::pop()
  {
    data_.pop_front();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return *data_.cbegin();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.size() == 0;
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }
}

#endif
