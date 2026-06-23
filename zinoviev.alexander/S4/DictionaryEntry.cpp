#include "DictionaryEntry.hpp"

const std::string& zinoviev::DictionaryEntry::getName() const
{
  return name_;
}

zinoviev::BSTree<int, std::string>& zinoviev::DictionaryEntry::getTree()
{
  return tree_;
}

const zinoviev::BSTree<int, std::string>& zinoviev::DictionaryEntry::getTree() const
{
  return tree_;
}
