#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include <cctype>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>
#include "Stack.hpp"

namespace zinoviev
{
  namespace
  {
    void checkOverflow(const long long first, const char oper, const long long second)
    {
      if (oper == '+')
      {
        if ((first > 0 && second > 0 &&
             first > std::numeric_limits<long long>::max() - second) ||
            (first < 0 && second < 0 &&
             first < std::numeric_limits<long long>::min() - second))
        {
          throw std::logic_error("Overflow");
        }
      }
      else if (oper == '-')
      {
        if ((first > 0 && second < 0 &&
             first > std::numeric_limits<long long>::max() + second) ||
            (first < 0 && second > 0 &&
             first < std::numeric_limits<long long>::min() + second))
        {
          throw std::logic_error("Overflow");
        }
      }
      else if (oper == '*')
      {
        if (first != 0 && second != 0)
        {
          if ((first > 0 && second > 0 &&
               first > std::numeric_limits<long long>::max() / second) ||
              (first > 0 && second < 0 &&
               second < std::numeric_limits<long long>::min() / first) ||
              (first < 0 && second > 0 &&
               first < std::numeric_limits<long long>::min() / second) ||
              (first < 0 && second < 0 &&
               first < std::numeric_limits<long long>::max() / second))
          {
            throw std::logic_error("Overflow");
          }
        }
      }
    }

    std::string getNextToken(const std::string& expression, size_t& pos)
    {
      const size_t len = expression.length();

      while (pos < len && std::isspace(expression[pos]))
      {
        ++pos;
      }

      if (pos >= len)
      {
        return "";
      }

      char c = expression[pos];

      if (std::isdigit(c))
      {
        size_t start = pos;
        while (pos < len && std::isdigit(expression[pos]))
        {
          ++pos;
        }
        return expression.substr(start, pos - start);
      }
      else if (std::isalpha(c))
      {
        size_t start = pos;
        while (pos < len && std::isalpha(expression[pos]))
        {
          ++pos;
        }
        return expression.substr(start, pos - start);
      }
      else
      {
        ++pos;
        return std::string(1, c);
      }
    }
  }

  long long gcd(long long a, long long b)
  {
    while (b != 0)
    {
      long long temp = b;
      b = a % b;
      a = temp;
    }
    return a;
  }

  long long lcm(long long a, long long b)
  {
    a = std::llabs(a);
    b = std::llabs(b);
    long long g = gcd(a, b);
    if (g != 0)
    {
      return (a / g) * b;
    }
    return 0;
  }

  int priority(const char oper)
  {
    if (oper == '+' || oper == '-')
    {
      return 1;
    }
    else if (oper == '/' || oper == '*' || oper == '%' || oper == 'l')
    {
      return 2;
    }
    else
    {
      throw std::logic_error("Bad operator");
    }
  }

  long long calculate(const long long first, const char oper, const long long second)
  {
    if (oper == '/')
    {
      if (second == 0)
      {
        throw std::logic_error("Division by zero");
      }
      return first / second;
    }
    else if (oper == '%')
    {
      if (second == 0)
      {
        throw std::logic_error("Division by zero");
      }
      long long rem = first % second;
      if (rem < 0)
      {
        rem += std::llabs(second);
      }
      return rem;
    }
    else if (oper == '+')
    {
      checkOverflow(first, oper, second);
      return first + second;
    }
    else if (oper == '-')
    {
      checkOverflow(first, oper, second);
      return first - second;
    }
    else if (oper == '*')
    {
      checkOverflow(first, oper, second);
      return first * second;
    }
    else if (oper == 'l')
    {
      return lcm(first, second);
    }
    else
    {
      throw std::logic_error("Unknown operation");
    }
  }

  long long evaluate(const std::string& expression)
  {
    Stack< long long > operands;
    Stack< char > operators;
    std::string token;
    size_t pos = 0;

    while (true)
    {
      token = getNextToken(expression, pos);

      if (token.empty())
      {
        break;
      }

      bool is_oper = false;

      if (token.size() == 1 && !std::isdigit(token[0]))
      {
        is_oper = true;
      }
      else if (token == "lcm")
      {
        is_oper = true;
      }

      if (is_oper)
      {
        if (token[0] == '(')
        {
          operators.push(token[0]);
        }
        else if (token[0] == ')')
        {
          while (!operators.empty() && operators.top() != '(')
          {
            if (operands.size() < 2)
            {
              throw std::logic_error("Not enough operands");
            }

            char oper = operators.top();
            operators.pop();

            long long second = operands.top();
            operands.pop();
            long long first = operands.top();
            operands.pop();

            operands.push(calculate(first, oper, second));
          }

          if (!operators.empty() && operators.top() == '(')
          {
            operators.pop();
          }
          else
          {
            throw std::logic_error("Error with operators");
          }
        }
        else if (priority(token[0]) == 1 || priority(token[0]) == 2)
        {
          while (!operators.empty() && operators.top() != '(' &&
                 priority(operators.top()) >= priority(token[0]))
          {
            if (operands.size() < 2)
            {
              throw std::logic_error("Not enough operands");
            }

            char oper = operators.top();
            operators.pop();

            long long second = operands.top();
            operands.pop();
            long long first = operands.top();
            operands.pop();

            operands.push(calculate(first, oper, second));
          }
          operators.push(token[0]);
        }
      }
      else
      {
        operands.push(std::stoll(token));
      }
    }

    while (!operators.empty())
    {
      if (operators.top() == '(')
      {
        throw std::logic_error("Mismatched parentheses");
      }

      if (operands.size() < 2)
      {
        throw std::logic_error("Not enough operands");
      }

      char oper = operators.top();
      operators.pop();
      long long second = operands.top();
      operands.pop();
      long long first = operands.top();
      operands.pop();

      operands.push(calculate(first, oper, second));
    }

    if (operands.size() != 1)
    {
      throw std::logic_error("Bad expression");
    }

    long long res = operands.top();
    operands.pop();
    return res;
  }
}

#endif
