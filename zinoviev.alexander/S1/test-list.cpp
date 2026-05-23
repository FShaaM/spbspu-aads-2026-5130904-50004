#define BOOST_TEST_MODULE BiListTests
#include <boost/test/included/unit_test.hpp>
#include "BiList.hpp"

using namespace zinoviev;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  BiList< int > lst;
  BOOST_CHECK(lst.size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(push_front_size)
{
  BiList< int > lst;
  lst.push_front(10);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_front(20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BIter< int > it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(push_back_size)
{
  BiList< int > lst;
  lst.push_back(10);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_back(20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BIter< int > it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BIter< int > it = lst.begin();
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
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BIter< int > it = lst.begin();
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
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.clear();
  BOOST_CHECK(lst.size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(erase_begin)
{
  BiList< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);
  BIter< int > it = lst.erase(lst.begin());
  BOOST_CHECK_EQUAL(*it, 20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 30);
}

BOOST_AUTO_TEST_CASE(erase_middle)
{
  BiList< int > lst;
  for (int i = 0; i < 5; ++i) lst.push_back(i);
  BIter< int > it = lst.begin();
  ++it;
  it = lst.erase(it);
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(lst.size(), 4);
  int expected[] = {0,2,3,4};
  BIter< int > check = lst.begin();
  for (int i = 0; i < 4; ++i, ++check)
  {
    BOOST_CHECK_EQUAL(*check, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(erase_end)
{
  BiList< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);
  BIter< int > it = lst.begin();
  ++it; ++it;
  it = lst.erase(it);
  BOOST_CHECK(it == lst.end());
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BIter< int > check = lst.begin();
  BOOST_CHECK_EQUAL(*check, 10);
  ++check;
  BOOST_CHECK_EQUAL(*check, 20);
}

BOOST_AUTO_TEST_CASE(iterator_increment_decrement)
{
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  BIter< int > it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BIter< int > it2 = ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(*it2, 2);
  it2 = it++;
  BOOST_CHECK_EQUAL(*it, 3);
  BOOST_CHECK_EQUAL(*it2, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
  it--;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  BIter< int > it1 = lst.begin();
  BIter< int > it2 = lst.begin();
  BOOST_CHECK(it1 == it2);
  ++it1;
  BOOST_CHECK(it1 != it2);
  BOOST_CHECK(it1 != lst.end());
  BOOST_CHECK(it2 != lst.end());
  ++it1;
  BOOST_CHECK(it1 == lst.end());
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  BiList< int > lst;
  lst.push_back(5);
  lst.push_back(6);
  const BiList< int >& const_lst = lst;
  CBIter< int > cit = const_lst.cbegin();
  BOOST_CHECK_EQUAL(*cit, 5);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 6);
  ++cit;
  BOOST_CHECK(cit == const_lst.cend());

  CBIter< int > cit2 = lst.begin();
  BOOST_CHECK_EQUAL(*cit2, 5);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  BiList< int > copy(lst);
  BOOST_CHECK_EQUAL(copy.size(), lst.size());
  BIter< int > it1 = lst.begin();
  BIter< int > it2 = copy.begin();
  while (it1 != lst.end() && it2 != copy.end())
  {
    BOOST_CHECK_EQUAL(*it1, *it2);
    ++it1;
    ++it2;
  }
  copy.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 3);
  BOOST_CHECK_EQUAL(copy.size(), 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  BiList< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  BiList< int > moved(std::move(lst));
  BOOST_CHECK_EQUAL(moved.size(), 2);
  BIter< int > it = moved.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  BOOST_CHECK(lst.size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  BiList< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  BiList< int > assign;
  assign = lst;
  BOOST_CHECK_EQUAL(assign.size(), 2);
  BIter< int > it = assign.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  assign = assign;
  BOOST_CHECK_EQUAL(assign.size(), 2);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  BiList< int > lst;
  lst.push_back(5);
  lst.push_back(6);
  BiList< int > assign;
  assign = std::move(lst);
  BOOST_CHECK_EQUAL(assign.size(), 2);
  BIter< int > it = assign.begin();
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
  BOOST_CHECK(lst.size() == 0);
}
