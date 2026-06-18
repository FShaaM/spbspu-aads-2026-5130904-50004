#define BOOST_TEST_MODULE StackTest

#include <boost/test/included/unit_test.hpp>
#include "Stack.hpp"

using namespace zinoviev;

BOOST_AUTO_TEST_CASE(testStackPushTopPop)
{
  Stack<int> s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(10);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(s.top(), 10);

  s.push(20);
  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.size(), 2);

  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 10);
  BOOST_CHECK_EQUAL(s.size(), 1);

  s.pop();
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(testStackTopException)
{
  Stack<int> s;
  BOOST_CHECK_THROW(s.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(testStackCopyAndAssignment)
{
  Stack<int> s1;
  s1.push(1);
  s1.push(2);

  Stack<int> s2(s1);
  BOOST_CHECK_EQUAL(s2.top(), 2);
  s2.pop();
  BOOST_CHECK_EQUAL(s2.top(), 1);

  Stack<int> s3;
  s3 = s1;
  BOOST_CHECK_EQUAL(s3.top(), 2);
}
