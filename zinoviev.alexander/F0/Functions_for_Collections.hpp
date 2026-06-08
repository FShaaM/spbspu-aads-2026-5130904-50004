#ifndef FUNCTIONS_FOR_COLLECTIONS_HPP
#define FUNCTIONS_FOR_COLLECTIONS_HPP

#include <iostream>
#include <string>
#include "Vector.hpp"
#include "RBTree.hpp"
#include "Card.hpp"

namespace zinoviev
{
  std::istream& loadCatalog(std::istream& input, RBTree< size_t, Card >& catalog);
  std::pair< size_t, Vector< size_t > > knapsack(const RBTree< size_t, Card >& catalog, size_t budget, const std::string& criterion);
}

#endif
