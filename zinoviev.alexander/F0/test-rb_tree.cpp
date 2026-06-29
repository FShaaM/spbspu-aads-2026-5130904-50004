#define BOOST_TEST_MODULE RBTreeTests
#include <boost/test/included/unit_test.hpp>
#include "RBTree.hpp"
#include <string>

using namespace zinoviev;

BOOST_AUTO_TEST_CASE(insert_and_size)
{
  RBTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty() == true);

  tree.insert(10, "Card10");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(tree.empty() == false);

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 10);
  BOOST_CHECK_EQUAL(it->second, "Card10");
}

BOOST_AUTO_TEST_CASE(insert_duplicate)
{
  RBTree<int, std::string> tree;
  auto res1 = tree.insert(42, "Original");
  BOOST_CHECK_EQUAL(res1.second, true);

  auto res2 = tree.insert(42, "Duplicate");
  BOOST_CHECK_EQUAL(res2.second, false);
  BOOST_CHECK_EQUAL(tree.size(), 1);

  auto it = tree.find(42);
  BOOST_CHECK_EQUAL(it->second, "Original");
}

BOOST_AUTO_TEST_CASE(find_elements)
{
  RBTree<int, std::string> tree;
  tree.insert(5, "Five");
  tree.insert(15, "Fifteen");

  auto it1 = tree.find(5);
  BOOST_CHECK(it1 != tree.end());
  BOOST_CHECK_EQUAL(it1->second, "Five");

  auto it2 = tree.find(999);
  BOOST_CHECK(it2 == tree.end());
}

BOOST_AUTO_TEST_CASE(erase_elements)
{
  RBTree<int, std::string> tree;
  tree.insert(1, "One");
  tree.insert(2, "Two");

  tree.erase(1);
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(tree.find(1) == tree.end());

  auto it = tree.find(2);
  tree.erase(it);
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty() == true);
}

BOOST_AUTO_TEST_CASE(clear_tree)
{
  RBTree<int, std::string> tree;
  tree.insert(10, "A");
  tree.insert(20, "B");
  BOOST_CHECK_EQUAL(tree.size(), 2);

  tree.clear();
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty() == true);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(copy_and_assignment)
{
  RBTree<int, std::string> tree1;
  tree1.insert(7, "Seven");

  RBTree<int, std::string> tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.find(7)->second, "Seven");

  RBTree<int, std::string> tree3;
  tree3 = tree1;
  BOOST_CHECK_EQUAL(tree3.size(), 1);
  BOOST_CHECK_EQUAL(tree3.find(7)->second, "Seven");
}

BOOST_AUTO_TEST_CASE(const_find_and_iterators)
{
  RBTree<int, std::string> tree;
  tree.insert(100, "ConstCard");

  const RBTree<int, std::string>& const_tree = tree;
  BOOST_CHECK(const_tree.empty() == false);
  BOOST_CHECK_EQUAL(const_tree.size(), 1);

  auto cit = const_tree.find(100);
  BOOST_CHECK(cit != const_tree.cend());
  BOOST_CHECK_EQUAL(cit->second, "ConstCard");
  BOOST_CHECK(const_tree.cbegin() == cit);
}

BOOST_AUTO_TEST_CASE(erase_non_existent)
{
  RBTree<int, std::string> tree;
  tree.insert(10, "A");
  tree.insert(20, "B");

  tree.erase(999);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(tree.find(10) != tree.end());
  BOOST_CHECK(tree.find(20) != tree.end());
}

BOOST_AUTO_TEST_CASE(iterator_increment_decrement)
{
  RBTree<int, std::string> tree;
  tree.insert(30, "Thirty");
  tree.insert(10, "Ten");
  tree.insert(20, "Twenty");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 10);
  
  ++it;
  BOOST_CHECK_EQUAL(it->first, 20);
  
  ++it;
  BOOST_CHECK_EQUAL(it->first, 30);

  --it;
  BOOST_CHECK_EQUAL(it->first, 20);
  
  --it;
  BOOST_CHECK_EQUAL(it->first, 10);
}

BOOST_AUTO_TEST_CASE(multiple_inserts_and_deletes)
{
  RBTree<int, std::string> tree;
  
  for (int i = 1; i <= 10; ++i)
  {
    tree.insert(i, "Value" + std::to_string(i));
  }
  BOOST_CHECK_EQUAL(tree.size(), 10);

  for (int i = 1; i <= 5; ++i)
  {
    tree.erase(i);
  }
  BOOST_CHECK_EQUAL(tree.size(), 5);
  BOOST_CHECK(tree.find(3) == tree.end());
  BOOST_CHECK(tree.find(8) != tree.end());
}

BOOST_AUTO_TEST_CASE(self_assignment_safety)
{
  RBTree<int, std::string> tree;
  tree.insert(55, "Safe");

  tree = tree;
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.find(55)->second, "Safe");
}

BOOST_AUTO_TEST_CASE(swap_trees)
{
  RBTree<int, std::string> tree1;
  tree1.insert(1, "One");

  RBTree<int, std::string> tree2;
  tree2.insert(2, "Two");
  tree2.insert(3, "Three");

  tree1.swap(tree2);

  BOOST_CHECK_EQUAL(tree1.size(), 2);
  BOOST_CHECK_EQUAL(tree2.size(), 1);

  BOOST_CHECK(tree1.find(1) == tree1.end());
  BOOST_CHECK(tree1.find(2) != tree1.end());
  BOOST_CHECK(tree2.find(1) != tree2.end());
}
