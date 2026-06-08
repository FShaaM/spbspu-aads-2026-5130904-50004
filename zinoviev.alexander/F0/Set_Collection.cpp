#include "Set_Collection.hpp"

namespace zinoviev
{
  void SetCollection::addSet(const std::string& name, const Vector< size_t >& ids)
  {
    auto it = sets_.insert(name, ids);

    if (!it.second)
    {
      sets_.erase(name);
      sets_.insert(name, ids);
    }
  }

  bool SetCollection::hasSet(const std::string& name) const
  {
    return sets_.find(name) != sets_.cend();
  }

  const Vector< size_t >* SetCollection::getSet(const std::string& name) const
  {
    auto it = sets_.find(name);
    if (it != sets_.cend())
      return &(it->second);

    return nullptr;
  }

  void SetCollection::removeSet(const std::string& name)
  {
    sets_.erase(name);
  }

  std::istream& SetCollection::loadFromStream(std::istream& in)
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
        continue;

      size_t pos = 0;
      while (pos < line.size() && line[pos] == ' ')
        ++pos;

      if (pos == line.size())
        continue;

      size_t start = pos;
      while (pos < line.size() && line[pos] != ' ')
        ++pos;

      std::string name = line.substr(start, pos - start);

      Vector< size_t > ids;
      while (pos < line.size())
      {
        while (pos < line.size() && line[pos] == ' ')
          ++pos;

        if (pos == line.size())
          break;

        size_t num_start = pos;
        while (pos < line.size() && std::isdigit(line[pos]))
          ++pos;

        std::string num_str = line.substr(num_start, pos - num_start);
        ids.push_back(std::stoull(num_str));
      }

      if (!ids.empty())
        addSet(name, ids);
    }

    return in;
  }

  void SetCollection::saveToStream(std::ostream& out) const
  {
    for (auto it = sets_.cbegin(); it != sets_.cend(); ++it)
    {
      out << it->first;
      const Vector< size_t >& ids = it->second;
      for (size_t i = 0; i < ids.size(); ++i)
      {
        out << " " << ids[i];
      }
      out << "\n";
    }
  }

  CIterator< std::string, Vector< size_t > > SetCollection::cbegin() const
  {
    return sets_.cbegin();
  }

  CIterator< std::string, Vector< size_t > > SetCollection::cend() const
  {
    return sets_.cend();
  }

  Iterator< std::string, Vector< size_t > > SetCollection::begin()
  {
    return sets_.begin();
  }

  Iterator< std::string, Vector< size_t > > SetCollection::end()
  {
    return sets_.end();
  }
}
