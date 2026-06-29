#define BOOST_TEST_MODULE FunctionTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "Set_Operations.hpp"
#include "Collection.hpp"
#include "RBTree.hpp"

using namespace zinoviev;

BOOST_AUTO_TEST_CASE(assemble_by_id_success)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});
  SetCollection sets;

  std::stringstream input("Set1 1");
  std::stringstream out;

  assemble_set_by_id(input, out, catalog, sets);
  BOOST_CHECK(sets.hasSet("Set1") == true);
}

BOOST_AUTO_TEST_CASE(assemble_by_id_bad_name)
{
  RBTree<size_t, Card> catalog;
  SetCollection sets;
  sets.addSet("Set1", Vector<size_t>());

  std::stringstream input("Set1 1");
  std::stringstream out;

  assemble_set_by_id(input, out, catalog, sets);
  BOOST_CHECK_EQUAL(out.str(), "<ERROR> : BAD NAME\n");
}

BOOST_AUTO_TEST_CASE(assemble_by_type_success)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});
  SetCollection sets;

  std::stringstream input("Set2 Magic");
  std::stringstream out;

  assemble_set_by_type(input, out, catalog, sets);
  BOOST_CHECK(sets.hasSet("Set2") == true);
}

BOOST_AUTO_TEST_CASE(upgrade_set_completed)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);

  Collection collection;
  collection.addCard(Card{1, "Card1", "Magic", 3, 100, 10});

  std::stringstream out;
  upgradeSet("Set1", 500, "rarity", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "Set already completed\n");
}

BOOST_AUTO_TEST_CASE(upgrade_set_no_budget)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);

  Collection collection;
  std::stringstream out;
  upgradeSet("Set1", 10, "rarity", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "Cannot upgrade set within budget : 10\n");
}

BOOST_AUTO_TEST_CASE(completable_sets_no_money)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);

  Collection collection;
  std::stringstream out;
  completableSets(10, "rarity", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "No completable sets within budget 10\n");
}

BOOST_AUTO_TEST_CASE(min_cost_not_found)
{
  RBTree<size_t, Card> catalog;
  SetCollection sets;
  Collection collection;

  Vector<std::string> names;
  names.push_back("GhostSet");

  std::stringstream out;
  minCostForSets(names, collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "Set 'GhostSet' not found\n");
}

BOOST_AUTO_TEST_CASE(min_cost_overlap)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);
  sets.addSet("Set2", ids);

  Collection collection;
  Vector<std::string> names;
  names.push_back("Set1");
  names.push_back("Set2");

  std::stringstream out;
  minCostForSets(names, collection, catalog, sets, out);

  BOOST_CHECK(out.str().find("Minimum cost to complete all given sets: 100") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(max_sets_no_budget)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);

  Collection collection;
  std::stringstream out;
  maxSets(10, "count", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "No sets can be completed within budget 10\n");
}

BOOST_AUTO_TEST_CASE(check_set_not_found)
{
  RBTree<size_t, Card> catalog;
  SetCollection sets;
  Collection collection;

  std::stringstream out;
  checkSet("Set1", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "Set not found.\n");
}

BOOST_AUTO_TEST_CASE(check_set_completed)
{
  RBTree<size_t, Card> catalog;
  catalog.insert(1, Card{1, "Card1", "Magic", 3, 100, 10});

  SetCollection sets;
  Vector<size_t> ids;
  ids.push_back(1);
  sets.addSet("Set1", ids);

  Collection collection;
  collection.addCard(Card{1, "Card1", "Magic", 3, 100, 10});

  std::stringstream out;
  checkSet("Set1", collection, catalog, sets, out);

  BOOST_CHECK_EQUAL(out.str(), "Set 'Set1' is completed!\n");
}
