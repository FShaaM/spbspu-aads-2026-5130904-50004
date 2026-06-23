#ifndef DICTIONARYENTRY_HPP
#define DICTIONARYENTRY_HPP

#include <string>
#include "BSTree.hpp"

namespace zinoviev
{
  class DictionaryEntry
  {
    std::string name_;
    BSTree<int, std::string> tree_;

  public:
    explicit DictionaryEntry(std::string name) :
      name_(name)
    {}

    const std::string& getName() const;
    BSTree<int, std::string>& getTree();
    const BSTree<int, std::string>& getTree() const;
  };
}

#endif
