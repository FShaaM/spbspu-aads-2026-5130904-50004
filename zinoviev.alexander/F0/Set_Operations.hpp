#ifndef SET_OPERATIONS_HPP
#define SET_OPERATIONS_HPP

#include <string>
#include <iostream>
#include "Vector.hpp"
#include "Set_Collection.hpp"
#include "Functions_for_Collections.hpp"
#include "Collection.hpp"

namespace zinoviev
{

  std::istream& assemble_set_by_id(std::istream& input, std::ostream& out, const RBTree< size_t, Card >& catalog, SetCollection& sets);
  std::istream& assemble_set_by_type(std::istream& input, std::ostream& out, const RBTree< size_t, Card >& catalog, SetCollection& sets);

  void upgradeSet(const std::string& setName, size_t budget, const std::string& criterion,
    const Collection& collection, const RBTree< size_t, Card >& catalog,
    const SetCollection& sets, std::ostream& out);

  void completableSets(size_t budget, const std::string& criterion,
    const Collection& collection, const RBTree< size_t, Card >& catalog,
    const SetCollection& sets, std::ostream& out);

  void minCostForSets(const Vector< std::string >& setNames,
    const Collection& collection, const RBTree< size_t, Card >& catalog,
    const SetCollection& sets, std::ostream& out);

  void maxSets(size_t budget, const std::string& mode,
    const Collection& collection, const RBTree< size_t, Card >& catalog,
    const SetCollection& sets, std::ostream& out);

  void checkSet(const std::string& setName,
    const Collection& collection, const RBTree< size_t, Card >& catalog,
    const SetCollection& sets, std::ostream& out);

}

#endif
