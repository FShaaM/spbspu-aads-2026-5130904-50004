#ifndef FUNCTIONS_FOR_COLLECTIONS_HPP
#define FUNCTIONS_FOR_COLLECTIONS_HPP

#include <iostream>
#include <string>
#include <ios>
#include "Vector.hpp"
#include "RBTree.hpp"
#include "Card.hpp"

namespace zinoviev
{

  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >& s);
    ~IOGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::ios_base::fmtflags flags_;
    char fill_;
  };

  std::istream& loadCatalog(std::istream& input, RBTree< size_t, Card >& catalog);
  std::pair< size_t, Vector< size_t > > knapsack(const RBTree< size_t, Card >& catalog, size_t budget, const std::string& criterion);

}

#endif
