#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include <functional>
#include "vector.hpp"
#include "hash_table.hpp"
#include "Graph.hpp"

namespace zinoviev
{
  using GraphTable = HashTable< std::string,
                                Graph,
                                StringHasher,
                                std::equal_to< std::string > >;

  Vector< std::string > split(const std::string& line);
  std::ostream& print_name_graphs(std::ostream& out,
                                  GraphTable::ConstIterator cbegin,
                                  GraphTable::ConstIterator cend);
}

#endif
