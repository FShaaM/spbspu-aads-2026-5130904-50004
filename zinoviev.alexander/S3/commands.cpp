#include "commands.hpp"

namespace zinoviev
{
  Vector< std::string > split(const std::string& line)
  {
    Vector< std::string > res;

    if (line.empty())
    {
      return res;
    }

    std::string word;
    const size_t len = line.size();

    for (size_t i = 0; i < len; ++i)
    {
      const char ch = line[i];

      if (ch == ' ')
      {
        if (!word.empty())
        {
          res.push_back(word);
          word.clear();
        }
      }
      else
      {
        word.push_back(ch);
      }
    }

    if (!word.empty())
    {
      res.push_back(word);
    }
    return res;
  }

  std::ostream& print_name_graphs(std::ostream& out,
                                  GraphTable::ConstIterator cbegin,
                                  GraphTable::ConstIterator cend)
  {
    Vector< std::string > names;

    for (; cbegin != cend; ++cbegin)
    {
      names.push_back((*cbegin).first);
    }

    sortVector(names);

    const size_t count = names.size();
    for (size_t i = 0; i < count; ++i)
    {
      out << names[i] << '\n';
    }

    return out;
  }
}
