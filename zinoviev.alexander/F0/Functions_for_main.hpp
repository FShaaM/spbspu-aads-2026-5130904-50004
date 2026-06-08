#ifndef FUNCTIONS_FOR_MAIN_HPP
#define FUNCTIONS_FOR_MAIN_HPP

#include <iostream>
#include <string>
#include "Vector.hpp"
#include "Card.hpp"
#include "Collection.hpp"
#include "Set_Collection.hpp"
#include "RBTree.hpp"

namespace zinoviev
{
  Vector<std::string> splitLine(const std::string& line);

  void processCreate(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processUse(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::string& current,
    std::ostream& out);

  void processList(const RBTree<std::string, Collection>& collections,
    const std::string& current,
    std::ostream& out);

  void processDrop(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::string& current,
    std::ostream& out);

  void processAdd(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processRemove(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processCard(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processFilter(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processCards(const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processSave(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processLoad(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out);

  void processCreateSetToId(const Vector< std::string >& tokens,
    const RBTree< size_t, Card >& catalog,
    SetCollection& sets,
    std::ostream& out);

  void processCreateSetToType(const Vector< std::string >& tokens,
    const RBTree< size_t, Card >& catalog,
    SetCollection& sets,
    std::ostream& out);

  void processCheckSet(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processUpgradeSet(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processCompletableSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processMinCostForSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processMaxSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processSaveSets(const Vector< std::string >& tokens,
    const SetCollection& sets,
    std::ostream& out);

  void processLoadSets(const Vector< std::string >& tokens,
    SetCollection& sets,
    std::ostream& out);

  void processSaveAll(const Vector< std::string >& tokens,
    const RBTree< std::string, Collection >& collections,
    const SetCollection& sets,
    const std::string& setsFile,
    const std::string& collFile,
    std::ostream& out);
}

#endif
