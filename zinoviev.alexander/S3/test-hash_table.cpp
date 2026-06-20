#define BOOST_TEST_MODULE HashTableTests
#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include <string>

using namespace zinoviev;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  HashTable<std::string, int> ht(16, 4, 16);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("a", 5);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());

  ht.add("b", 7);
  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("c", 10);
  ht.add("d", 20);

  BOOST_CHECK(ht.has("c"));
  BOOST_CHECK(ht.has("d"));
  BOOST_CHECK(!ht.has("e"));
}

BOOST_AUTO_TEST_CASE(test_find)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("f", 15);
  ht.add("g", 25);

  BOOST_CHECK_EQUAL(*ht.find("f"), 15);
  BOOST_CHECK_EQUAL(*ht.find("g"), 25);

  *ht.find("f") = 99;
  BOOST_CHECK_EQUAL(*ht.find("f"), 99);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("first", 111);
  ht.add("second", 222);

  int val;
  ht.drop("first", val);
  BOOST_CHECK_EQUAL(val, 111);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has("first"));
  BOOST_CHECK(ht.has("second"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("x", 33);
  ht.add("x", 44);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(*ht.find("x"), 44);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("p", 1);
  ht.add("q", 2);
  ht.add("r", 3);

  ht.clear();
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(!ht.has("p"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  HashTable<std::string, int> ht1(16, 4, 16);
  ht1.add("m", 7);
  ht1.add("n", 14);

  HashTable<std::string, int> ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("m"));
  BOOST_CHECK(ht2.has("n"));
  BOOST_CHECK_EQUAL(*ht2.find("m"), 7);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  HashTable<std::string, int> ht1(16, 4, 16);
  ht1.add("first", 50);
  ht1.add("second", 60);

  HashTable<std::string, int> ht2 = std::move(ht1);

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("first"));
  BOOST_CHECK(ht2.has("second"));
  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  HashTable<std::string, int> ht(11, 4, 16);
  ht.add("q", 1);
  ht.add("w", 2);
  ht.add("e", 3);
  ht.add("r", 4);
  ht.add("t", 5);
  ht.add("y", 6);

  ht.rehash(37);
  BOOST_CHECK_EQUAL(ht.size(), 6);
  BOOST_CHECK(ht.has("q"));
  BOOST_CHECK(ht.has("w"));
  BOOST_CHECK(ht.has("e"));
  BOOST_CHECK(ht.has("r"));
  BOOST_CHECK(ht.has("t"));
  BOOST_CHECK(ht.has("y"));
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  HashTable<std::string, int> ht(16, 4, 16);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.erase("a");
  BOOST_CHECK(!ht.has("a"));
  BOOST_CHECK(ht.has("b"));
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
