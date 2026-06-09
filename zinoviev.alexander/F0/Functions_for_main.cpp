#include <fstream>
#include <cctype>
#include "Vector.hpp"
#include "Functions_for_main.hpp"
#include "Set_Operations.hpp"

namespace zinoviev
{
  Vector< std::string > splitLine(const std::string& line)
  {
    Vector< std::string > tokens;
    size_t pos = 0;
    while (pos < line.size())
    {
      while (pos < line.size() && std::isspace(line[pos]))
        ++pos;
      if (pos >= line.size())
        break;
      size_t start = pos;
      while (pos < line.size() && !std::isspace(line[pos]))
        ++pos;
      tokens.push_back(line.substr(start, pos - start));
    }
    return tokens;
  }

  void printHelp(std::ostream& out)
  {
    out << "Available commands:\n";
    out << "  create <name>                                     - create a new collection\n";
    out << "  use <name>                                        - switch to an existing collection\n";
    out << "  list                                              - list all collections (active marked with *)\n";
    out << "  drop <name>                                       - delete a collection\n";
    out << "  add <id> <name> <type> <rarity> <price> <power>   - add a card to current collection\n";
    out << "  remove <id>                                       - remove a card by id from current collection\n";
    out << "  card <id>                                         - show card info\n";
    out << "  filter <type>                                     - show cards of given type\n";
    out << "  cards                                             - show all cards in current collection\n";
    out << "  save <filename>                                   - save current collection to file\n";
    out << "  load <filename>                                   - load cards from file into current collection\n";
    out << "  create_set_to_id <name> <id1> <id2> ...           - create a set by explicit ids\n";
    out << "  create_set_to_type <name> <type>                  - create a set by card type\n";
    out << "  check_set <name>                                  - check if the set is completed in current collection\n";
    out << "  upgrade_set <name> <budget> [rarity|power]        - buy missing cards to complete the set\n";
    out << "  completable_sets <budget> [rarity|power]          - list sets that can be completed within budget\n";
    out << "  min_cost_for_sets <set1> <set2> ...               - minimum cost to complete several sets together\n";
    out << "  max_sets <budget> [count|rarity|power]            - maximize sets completed within budget\n";
    out << "  save_sets <filename>                              - save all sets to file\n";
    out << "  load_sets <filename>                              - load sets from file\n";
    out << "  save_all                                          - save both sets and collections to startup files\n";
    out << "  quit / exit                                       - exit the program\n";
  }

  void processCreate(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: create <name>\n";
      return;
    }
    const std::string& name = tokens[1];
    if (collections.find(name) == collections.end())
    {
      collections.insert(name, Collection());
      out << "Collection '" << name << "' created.\n";
    }
    else
      out << "Collection already exists.\n";
  }

  void processUse(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::string& current,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: use <name>\n";
      return;
    }
    const std::string& name = tokens[1];
    if (collections.find(name) != collections.end())
    {
      current = name;
      out << "Switched to collection '" << name << "'.\n";
    }
    else
      out << "Collection not found.\n";
  }

  void processList(const RBTree< std::string, Collection >& collections,
    const std::string& current,
    std::ostream& out)
  {
    for (auto it = collections.cbegin(); it != collections.cend(); ++it)
    {
      out << "  " << it->first;
      if (it->first == current)
        out << " *";
      out << std::endl;
    }
  }

  void processDrop(const Vector< std::string >& tokens,
    RBTree< std::string, Collection >& collections,
    std::string& current,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: drop <name>\n";
      return;
    }
    const std::string& name = tokens[1];
    if (collections.find(name) != collections.end())
    {
      collections.erase(name);
      if (current == name) current.clear();
      out << "Collection '" << name << "' removed.\n";
    }
    else
      out << "Collection not found.\n";
  }

  void processAdd(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 7)
    {
      out << "Usage: add <id> <name> <type> <rarity> <price> <power>\n";
      return;
    }
    Card card;
    card.id = std::stoull(tokens[1]);
    card.name = tokens[2];
    card.type = tokens[3];
    card.rarity = std::stoull(tokens[4]);
    card.price = std::stoull(tokens[5]);
    card.power = std::stoull(tokens[6]);
    auto it = collections.find(current);
    if (it != collections.end())
      it->second.addCard(card);
    out << "Card added.\n";
  }

  void processRemove(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 2)
    {
      out << "Usage: remove <id>\n";
      return;
    }
    size_t id = std::stoull(tokens[1]);
    auto it = collections.find(current);
    if (it != collections.end())
      it->second.removeCard(id);
    out << "Card removed (if existed).\n";
  }

  void processCard(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 2)
    {
      out << "Usage: card <id>\n";
      return;
    }
    size_t id = std::stoull(tokens[1]);
    auto it = collections.find(current);
    if (it != collections.cend())
    {
      const Card* card = it->second.findCard(id);
      if (card)
      {
        out << card->id << " " << card->name << " " << card->type << " "
          << card->rarity << " " << card->price << " " << card->power << std::endl;
      }
      else
        out << "Card not found.\n";
    }
    else
      out << "Collection not found.\n";
  }

  void processFilter(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 2)
    {
      out << "Usage: filter <type>\n";
      return;
    }
    const std::string& type = tokens[1];
    auto it = collections.find(current);
    if (it != collections.cend())
    {
      Vector< Card > cards = it->second.filterByType(type);
      for (size_t i = 0; i < cards.size(); ++i)
        out << cards[i].id << " " << cards[i].name << std::endl;
    }
    else
      out << "Collection not found.\n";
  }

  void processCards(const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    auto it = collections.find(current);
    if (it != collections.cend())
    {
      Vector< Card > all = it->second.getAllCards();
      for (size_t i = 0; i < all.size(); ++i)
        out << all[i].id << " " << all[i].name << " " << all[i].type << std::endl;
    }
    else
      out << "Collection not found.\n";
  }

  void processSave(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 2)
    {
      out << "Usage: save <filename>\n";
      return;
    }
    const std::string& filename = tokens[1];
    std::ofstream file(filename);
    if (file)
    {
      auto it = collections.find(current);
      if (it != collections.cend())
        it->second.saveToStream(file);
      out << "Collection saved to " << filename << std::endl;
    }
    else
      out << "Cannot create file.\n";
  }

  void processLoad(const Vector< std::string >& tokens,
    const std::string& current,
    RBTree< std::string, Collection >& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      out << "No active collection.\n";
      return;
    }
    if (tokens.size() < 2)
    {
      out << "Usage: load <filename>\n";
      return;
    }
    const std::string& filename = tokens[1];
    std::ifstream file(filename);
    if (file)
    {
      auto it = collections.find(current);
      if (it != collections.end())
        it->second.loadFromStream(file);
      out << "Collection loaded from " << filename << std::endl;
    }
    else
      out << "Cannot open file.\n";
  }

  void processCreateSetToId(const Vector< std::string >& tokens,
    const RBTree< size_t, Card >& catalog,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 3)
    {
      out << "Usage: create_set_to_id <name> <id1> <id2> ...\n";
      return;
    }
    std::string name = tokens[1];
    Vector< size_t > ids;
    for (size_t i = 2; i < tokens.size(); ++i)
      ids.push_back(std::stoull(tokens[i]));
    Vector< size_t > validIds;
    for (size_t j = 0; j < ids.size(); ++j)
    {
      size_t id = ids[j];
      if (catalog.find(id) != catalog.cend())
        validIds.push_back(id);
      else
        out << "Card with ID " << id << " not found in global catalog\n";
    }
    if (!validIds.empty())
    {
      if (sets.hasSet(name))
        out << "Set already exists.\n";
      else
      {
        sets.addSet(name, validIds);
        out << "Set '" << name << "' created with " << validIds.size() << " cards.\n";
      }
    }
    else
      out << "No valid IDs provided.\n";
  }

  void processCreateSetToType(const Vector< std::string >& tokens,
    const RBTree< size_t, Card >& catalog,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() != 3)
    {
      out << "Usage: create_set_to_type <name> <type>\n";
      return;
    }
    std::string name = tokens[1];
    std::string type = tokens[2];
    if (sets.hasSet(name))
    {
      out << "Set already exists.\n";
      return;
    }
    Vector< size_t > ids;
    for (auto it = catalog.cbegin(); it != catalog.cend(); ++it)
    {
      if (it->second.type == type)
        ids.push_back(it->second.id);
    }
    if (ids.empty())
      out << "No cards of type '" << type << "' in catalog.\n";
    else
    {
      sets.addSet(name, ids);
      out << "Set '" << name << "' created with " << ids.size() << " cards.\n";
    }
  }

  void processCheckSet(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: check_set <name>\n";
      return;
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      out << "No active collection.\n";
      return;
    }
    checkSet(tokens[1], it->second, catalog, sets, out);
  }

  void processUpgradeSet(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 3)
    {
      out << "Usage: upgrade_set <name> <budget> [rarity|power]\n";
      return;
    }
    std::string name = tokens[1];
    size_t budget = std::stoull(tokens[2]);
    std::string criterion = (tokens.size() >= 4) ? tokens[3] : "rarity";
    if (criterion != "rarity" && criterion != "power")
      criterion = "rarity";
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      out << "No active collection.\n";
      return;
    }
    upgradeSet(name, budget, criterion, it->second, catalog, sets, out);
  }

  void processCompletableSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: completable_sets <budget> [rarity|power]\n";
      return;
    }
    size_t budget = std::stoull(tokens[1]);
    std::string criterion = (tokens.size() >= 3) ? tokens[2] : "rarity";
    if (criterion != "rarity" && criterion != "power")
      criterion = "rarity";
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      out << "No active collection.\n";
      return;
    }
    completableSets(budget, criterion, it->second, catalog, sets, out);
  }

  void processMinCostForSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: min_cost_for_sets <set1> <set2> ...\n";
      return;
    }
    Vector< std::string > setNames;
    for (size_t i = 1; i < tokens.size(); ++i)
      setNames.push_back(tokens[i]);
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      out << "No active collection.\n";
      return;
    }
    minCostForSets(setNames, it->second, catalog, sets, out);
  }

  void processMaxSets(const Vector< std::string >& tokens,
    const std::string& current,
    const RBTree< std::string, Collection >& collections,
    const RBTree< size_t, Card >& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: max_sets <budget> [count|rarity|power]\n";
      return;
    }
    size_t budget = std::stoull(tokens[1]);
    std::string mode = (tokens.size() >= 3) ? tokens[2] : "count";
    if (mode != "count" && mode != "rarity" && mode != "power")
      mode = "count";
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      out << "No active collection.\n";
      return;
    }
    maxSets(budget, mode, it->second, catalog, sets, out);
  }

  void processSaveSets(const Vector< std::string >& tokens,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: save_sets <filename>\n";
      return;
    }
    const std::string& filename = tokens[1];
    std::ofstream file(filename);
    if (file)
    {
      sets.saveToStream(file);
      out << "Sets saved to " << filename << std::endl;
    }
    else
      out << "Cannot create file.\n";
  }

  void processLoadSets(const Vector< std::string >& tokens,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      out << "Usage: load_sets <filename>\n";
      return;
    }
    const std::string& filename = tokens[1];
    std::ifstream file(filename);
    if (file)
    {
      sets.loadFromStream(file);
      out << "Sets loaded from " << filename << std::endl;
    }
    else
      out << "Cannot open file.\n";
  }

  void processSaveAll(const Vector< std::string >& tokens,
    const RBTree< std::string, Collection >& collections,
    const SetCollection& sets,
    const std::string& setsFile,
    const std::string& collFile,
    std::ostream& out)
  {
    if (!setsFile.empty())
    {
      std::ofstream sf(setsFile);
      if (sf)
      {
        sets.saveToStream(sf);
        out << "Sets saved to " << setsFile << std::endl;
      }
      else
        out << "Cannot write sets file.\n";
    }
    if (!collFile.empty())
    {
      std::ofstream cf(collFile);
      if (cf)
      {
        for (auto it = collections.cbegin(); it != collections.cend(); ++it)
        {
          cf << it->first;
          Vector< Card > cards = it->second.getAllCards();
          for (size_t i = 0; i < cards.size(); ++i)
          {
            const Card& c = cards[i];
            cf << " " << c.id << " " << c.name << " " << c.type
              << " " << c.rarity << " " << c.price << " " << c.power;
          }
          cf << "\n";
        }
        out << "Collections saved to " << collFile << std::endl;
      }
      else
        out << "Cannot write collections file.\n";
    }
  }

}
