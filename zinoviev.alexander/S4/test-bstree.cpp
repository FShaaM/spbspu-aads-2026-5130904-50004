#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include "BSTree.hpp"

using namespace zinoviev;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(empty_tree)
{
    BSTree<int, std::string> tree;
    BOOST_CHECK(tree.begin() == tree.end());
    BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(push_and_get)
{
    BSTree<int, std::string> tree;
    tree.push(5, "five");
    tree.push(3, "three");
    tree.push(7, "seven");

    BOOST_CHECK_EQUAL(tree.get(5), "five");
    BOOST_CHECK_EQUAL(tree.get(3), "three");
    BOOST_CHECK_EQUAL(tree.get(7), "seven");
    BOOST_CHECK(tree.contains(5));
    BOOST_CHECK(!tree.contains(99));
}

BOOST_AUTO_TEST_CASE(push_updates_existing_key)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(1, "ONE");
    BOOST_CHECK_EQUAL(tree.get(1), "ONE");
    BOOST_CHECK(!tree.contains(2));
}

BOOST_AUTO_TEST_CASE(get_missing_key_throws)
{
    BSTree<int, std::string> tree;
    tree.push(10, "ten");
    BOOST_CHECK_THROW(tree.get(20), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_removes_and_returns_value)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    std::string val;
    tree.drop(2, val);
    BOOST_CHECK_EQUAL(val, "two");
    BOOST_CHECK(!tree.contains(2));
    BOOST_CHECK(tree.contains(1));
    BOOST_CHECK(tree.contains(3));
}

BOOST_AUTO_TEST_CASE(drop_missing_key_throws)
{
    BSTree<int, std::string> tree;
    tree.push(42, "answer");
    std::string val;
    BOOST_CHECK_THROW(tree.drop(99, val), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(begin_returns_leftmost)
{
    BSTree<int, std::string> tree;
    tree.push(3, "three");
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(4, "four");

    auto it = tree.begin();
    BOOST_CHECK_EQUAL((*it).first, 1);
    BOOST_CHECK_EQUAL((*it).second, "one");
}

BOOST_AUTO_TEST_CASE(iterator_traverses_inorder)
{
    BSTree<int, std::string> tree;
    tree.push(4, "four");
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");
    tree.push(5, "five");

    auto it = tree.begin();
    BOOST_CHECK_EQUAL((*it).first, 1);
    ++it;
    BOOST_CHECK_EQUAL((*it).first, 2);
    ++it;
    BOOST_CHECK_EQUAL((*it).first, 3);
    ++it;
    BOOST_CHECK_EQUAL((*it).first, 4);
    ++it;
    BOOST_CHECK_EQUAL((*it).first, 5);
    ++it;
    BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(const_iterator_works)
{
    BSTree<int, std::string> tree;
    tree.push(10, "ten");
    const BSTree<int, std::string>& constTree = tree;
    auto it = constTree.cbegin();
    BOOST_CHECK_EQUAL((*it).first, 10);
    BOOST_CHECK_EQUAL((*it).second, "ten");
}

BOOST_AUTO_TEST_CASE(height_empty)
{
    BSTree<int, std::string> tree;
    BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(height_single_node)
{
    BSTree<int, std::string> tree;
    tree.push(42, "answer");
    BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(height_balanced)
{
    BSTree<int, std::string> tree;
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");
    BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(height_linear)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");
    tree.push(4, "four");
    BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(rotate_left_simple)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(2, "two");

    auto it = tree.cbegin();
    tree.rotateLeft(it);
    BOOST_CHECK_EQUAL(tree.get(1), "one");
    BOOST_CHECK_EQUAL(tree.get(2), "two");
    auto new_it = tree.begin();
    BOOST_CHECK_EQUAL((*new_it).first, 1);
    ++new_it;
    BOOST_CHECK_EQUAL((*new_it).first, 2);
}

BOOST_AUTO_TEST_CASE(rotate_right_simple)
{
    BSTree<int, std::string> tree;
    tree.push(2, "two");
    tree.push(1, "one");

    auto it = tree.begin();
    ++it;
    tree.rotateRight(it);
    BOOST_CHECK_EQUAL(tree.get(1), "one");
    BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
    BSTree<int, std::string> tree;
    tree.push(3, "three");
    tree.push(1, "one");
    tree.push(2, "two");
    auto it = tree.cbegin();
    while (it != tree.cend() && (*it).first != 3) ++it;
    BOOST_REQUIRE(it != tree.cend());
    tree.rotateLargeLeft(it);
    BOOST_CHECK_EQUAL(tree.get(1), "one");
    BOOST_CHECK_EQUAL(tree.get(2), "two");
    BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(3, "three");
    tree.push(2, "two");
    auto it = tree.cbegin();
    BOOST_REQUIRE(it != tree.cend() && (*it).first == 1);
    tree.rotateLargeRight(it);
    BOOST_CHECK_EQUAL(tree.get(1), "one");
    BOOST_CHECK_EQUAL(tree.get(2), "two");
    BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(works_with_string_keys)
{
    BSTree<std::string, int> tree;
    tree.push("ten", 10);
    tree.push("twenty", 20);
    BOOST_CHECK_EQUAL(tree.get("ten"), 10);
    BOOST_CHECK_EQUAL(tree.get("twenty"), 20);
    BOOST_CHECK(tree.contains("ten"));
    BOOST_CHECK(!tree.contains("thirty"));
}

BOOST_AUTO_TEST_CASE(works_with_double_keys)
{
    BSTree<double, std::string> tree;
    tree.push(1.5, "one point five");
    tree.push(2.7, "two point seven");
    BOOST_CHECK_EQUAL(tree.get(1.5), "one point five");
    BOOST_CHECK_EQUAL(tree.get(2.7), "two point seven");
    BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_on_invalid_iterator)
{
    BSTree<int, std::string> tree;
    tree.push(1, "one");
    auto it = tree.cend();
    tree.rotateLeft(it);
    tree.rotateRight(it);
    tree.rotateLargeLeft(it);
    tree.rotateLargeRight(it);
    BOOST_CHECK_EQUAL(tree.get(1), "one");
    BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
