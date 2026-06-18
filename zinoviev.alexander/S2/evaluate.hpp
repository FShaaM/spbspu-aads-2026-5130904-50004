#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include <string>

namespace zinoviev
{
  long long gcd(long long a, long long b);
  long long lcm(long long a, long long b);
  int priority(const char oper);
  long long calculate(const long long first, const char oper, const long long second);
  long long evaluate(const std::string& expression);
}

#endif
