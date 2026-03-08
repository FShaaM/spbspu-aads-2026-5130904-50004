#define BOOST_TEST_MODULE BiListTests
#include <boost/test/included/unit_test.hpp>
#include "BiList.hpp"

using namespace zinoviev;

BOOST_AUTO_TEST_CASE(push_front_size)
{
  BiList<int> lst;
  lst.push_front(10);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_front(20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(push_back_size)
{
  BiList<int> lst;
  lst.push_back(10);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_back(20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  BiList<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.size(), 1);
  BOOST_CHECK_EQUAL(*lst.begin(), 3);
  lst.pop_front();
  BOOST_CHECK(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  BiList<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 1);
  BOOST_CHECK_EQUAL(*lst.begin(), 1);
  lst.pop_back();
  BOOST_CHECK(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  BiList<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.clear();
  BOOST_CHECK(lst.size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}
