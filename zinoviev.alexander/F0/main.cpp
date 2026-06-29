#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <functional>

#include "Vector.hpp"
#include "Collection.hpp"
#include "Set_Collection.hpp"
#include "RBTree.hpp"
#include "Hash_Table.hpp"
#include "Set_Operations.hpp"
#include "Card.hpp"

namespace zinoviev
{

  Vector<std::string> splitLine(const std::string& line);

  void printHelp(std::ostream& out);

  void processCreate(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processUse(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::string& current,
    std::ostream& out);

  void processList(const RBTree<std::string, Collection>& collections,
    const std::string& current,
    std::ostream& out);

  void processDrop(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::string& current,
    std::ostream& out);

  void processAdd(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processRemove(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processCard(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processFilter(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processCards(const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processSave(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processLoad(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out);

  void processCreateSetToId(const Vector<std::string>& tokens,
    const RBTree<size_t, Card>& catalog,
    SetCollection& sets,
    std::ostream& out);

  void processCreateSetToType(const Vector<std::string>& tokens,
    const RBTree<size_t, Card>& catalog,
    SetCollection& sets,
    std::ostream& out);

  void processCheckSet(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processUpgradeSet(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processCompletableSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processMinCostForSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processMaxSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out);

  void processSaveSets(const Vector<std::string>& tokens,
    const SetCollection& sets,
    std::ostream& out);

  void processLoadSets(const Vector<std::string>& tokens,
    SetCollection& sets,
    std::ostream& out);

  void processSaveAll(const Vector<std::string>& tokens,
    const RBTree<std::string, Collection>& collections,
    const SetCollection& sets,
    const std::string& setsFile,
    const std::string& collFile,
    std::ostream& out);

}

int main(int argc, char* argv[])
{
  using namespace zinoviev;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0]
      << " <catalog_file> [sets_file] [collections_file]\n";
    return 1;
  }

  RBTree<size_t, Card> catalog;
  RBTree<std::string, Collection> collections;
  std::string current;
  SetCollection sets;

  std::string setsFileName;
  std::string collFileName;
  if (argc >= 3)
  {
    setsFileName = argv[2];
  }
  if (argc >= 4)
  {
    collFileName = argv[3];
  }

  std::ifstream catalogFile(argv[1]);
  if (!catalogFile)
  {
    std::cout << "Cannot open catalog file.\n";
    return 1;
  }
  loadCatalog(catalogFile, catalog);

  if (!setsFileName.empty())
  {
    std::ifstream sf(setsFileName);
    if (sf)
    {
      sets.loadFromStream(sf);
    }
  }

  if (!collFileName.empty())
  {
    std::ifstream cf(collFileName);
    if (cf)
    {
      std::string line;
      while (std::getline(cf, line))
      {
        if (line.empty())
        {
          continue;
        }
        Vector<std::string> tokens = splitLine(line);
        if (tokens.empty())
        {
          continue;
        }
        std::string colName = tokens[0];
        Collection col;
        for (size_t i = 1; i + 5 < tokens.size(); i += 6)
        {
          size_t cardId = std::stoull(tokens[i]);
          std::string cardName = tokens[i + 1];
          std::string cardType = tokens[i + 2];
          size_t cardRarity = std::stoull(tokens[i + 3]);
          size_t cardPrice = std::stoull(tokens[i + 4]);
          size_t cardPower = std::stoull(tokens[i + 5]);

          col.addCard(Card{ cardId, cardName, cardType, cardRarity, cardPrice, cardPower });
        }
        collections.insert(colName, col);
      }
    }
  }

  using CommandHandler = std::function<void(const Vector<std::string>&)>;
  HashTable<std::string, CommandHandler, std::hash<std::string>, std::equal_to<std::string>> cmdTable;

  cmdTable.insert({ "help", [&](const Vector<std::string>&) { printHelp(std::cout); } });
  cmdTable.insert({ "create", [&](const Vector<std::string>& t) { processCreate(t, collections, std::cout); } });
  cmdTable.insert({ "use", [&](const Vector<std::string>& t) { processUse(t, collections, current, std::cout); } });
  cmdTable.insert({ "list", [&](const Vector<std::string>&) { processList(collections, current, std::cout); } });
  cmdTable.insert({ "drop", [&](const Vector<std::string>& t) { processDrop(t, collections, current, std::cout); } });
  cmdTable.insert({ "add", [&](const Vector<std::string>& t) { processAdd(t, current, collections, std::cout); } });
  cmdTable.insert({ "remove", [&](const Vector<std::string>& t) { processRemove(t, current, collections, std::cout); } });
  cmdTable.insert({ "card", [&](const Vector<std::string>& t) { processCard(t, current, collections, std::cout); } });
  cmdTable.insert({ "filter", [&](const Vector<std::string>& t) { processFilter(t, current, collections, std::cout); } });
  cmdTable.insert({ "cards", [&](const Vector<std::string>&) { processCards(current, collections, std::cout); } });
  cmdTable.insert({ "save", [&](const Vector<std::string>& t) { processSave(t, current, collections, std::cout); } });
  cmdTable.insert({ "load", [&](const Vector<std::string>& t) { processLoad(t, current, collections, std::cout); } });

  cmdTable.insert(
    { "create_set_to_id",
      [&](const Vector<std::string>& t)
      {
        processCreateSetToId(t, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "create_set_to_type",
      [&](const Vector<std::string>& t)
      {
        processCreateSetToType(t, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "check_set",
      [&](const Vector<std::string>& t)
      {
        processCheckSet(t, current, collections, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "upgrade_set",
      [&](const Vector<std::string>& t)
      {
        processUpgradeSet(t, current, collections, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "completable_sets",
      [&](const Vector<std::string>& t)
      {
        processCompletableSets(t, current, collections, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "min_cost_for_sets",
      [&](const Vector<std::string>& t)
      {
        processMinCostForSets(t, current, collections, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "max_sets",
      [&](const Vector<std::string>& t)
      {
        processMaxSets(t, current, collections, catalog, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "save_sets",
      [&](const Vector<std::string>& t)
      {
        processSaveSets(t, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "load_sets",
      [&](const Vector<std::string>& t)
      {
        processLoadSets(t, sets, std::cout);
      }
    });

  cmdTable.insert(
    { "save_all",
      [&](const Vector<std::string>& t)
      {
        processSaveAll(t, collections, sets, setsFileName, collFileName, std::cout);
      }
    });

  std::cout << "write 'help' to view the commands\n";

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    Vector<std::string> tokens = splitLine(line);
    if (tokens.empty())
    {
      continue;
    }
    const std::string& cmd = tokens[0];

    if (cmd == "quit" || cmd == "exit")
    {
      break;
    }

    try
    {
      auto it = cmdTable.find(cmd);
      if (it != cmdTable.end())
      {
        it->second(tokens);
      }
      else
      {
        std::cout << "Unknown command.\n";
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }
  }

  if (!setsFileName.empty())
  {
    std::ofstream sf(setsFileName);
    if (sf)
    {
      sets.saveToStream(sf);
    }
  }
  if (!collFileName.empty())
  {
    std::ofstream cf(collFileName);
    if (cf)
    {
      for (auto it = collections.cbegin(); it != collections.cend(); ++it)
      {
        cf << it->first;
        Vector<Card> cards = it->second.getAllCards();
        for (size_t i = 0; i < cards.size(); ++i)
        {
          cf << " " << cards[i];
        }
        cf << "\n";
      }
    }
  }
  return 0;
}

namespace zinoviev
{

  std::string getValidMode(const Vector<std::string>& tokens)
  {
    if (tokens.size() < 3)
    {
      return "count";
    }
    const std::string& m = tokens[2];
    return (m == "count" || m == "rarity" || m == "power") ? m : "count";
  }

  Vector<std::string> splitLine(const std::string& line)
  {
    Vector<std::string> tokens;
    size_t pos = 0;
    while (pos < line.size())
    {
      while (pos < line.size() && std::isspace(line[pos]))
      {
        ++pos;
      }
      if (pos >= line.size())
      {
        break;
      }
      size_t start = pos;
      while (pos < line.size() && !std::isspace(line[pos]))
      {
        ++pos;
      }
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

  void processCreate(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: create <name>");
    }
    const std::string& name = tokens[1];
    if (collections.find(name) == collections.end())
    {
      collections.insert(name, Collection());
      out << "Collection '" << name << "' created.\n";
    }
    else
    {
      throw std::runtime_error("Collection already exists.");
    }
  }

  void processUse(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::string& current,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: use <name>");
    }
    const std::string& name = tokens[1];
    if (collections.find(name) != collections.end())
    {
      current = name;
      out << "Switched to collection '" << name << "'.\n";
    }
    else
    {
      throw std::runtime_error("Collection not found.");
    }
  }

  void processList(const RBTree<std::string, Collection>& collections,
    const std::string& current,
    std::ostream& out)
  {
    for (auto it = collections.cbegin(); it != collections.cend(); ++it)
    {
      out << "  " << it->first;
      if (it->first == current)
      {
        out << " *";
      }
      out << std::endl;
    }
  }

  void processDrop(const Vector<std::string>& tokens,
    RBTree<std::string, Collection>& collections,
    std::string& current,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: drop <name>");
    }
    const std::string& name = tokens[1];
    if (collections.find(name) != collections.end())
    {
      collections.erase(name);
      if (current == name)
      {
        current.clear();
      }
      out << "Collection '" << name << "' removed.\n";
    }
    else
    {
      throw std::runtime_error("Collection not found.");
    }
  }

  void processAdd(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 7)
    {
      throw std::runtime_error("Usage: add <id> <name> <type> <rarity> <price> <power>");
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
    {
      it->second.addCard(card);
    }
    out << "Card added.\n";
  }

  void processRemove(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: remove <id>");
    }
    size_t id = std::stoull(tokens[1]);
    auto it = collections.find(current);
    if (it != collections.end())
    {
      it->second.removeCard(id);
    }
    out << "Card removed (if existed).\n";
  }

  void processCard(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: card <id>");
    }
    size_t id = std::stoull(tokens[1]);
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("Collection not found.");
    }
    const Card* card = it->second.findCard(id);
    if (card)
    {
      out << card->id << " " << card->name << " " << card->type << " "
        << card->rarity << " " << card->price << " " << card->power << std::endl;
    }
    else
    {
      throw std::runtime_error("Card not found.");
    }
  }

  void processFilter(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: filter <type>");
    }
    const std::string& type = tokens[1];
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("Collection not found.");
    }
    Vector<Card> cards = it->second.filterByType(type);
    for (size_t i = 0; i < cards.size(); ++i)
    {
      out << cards[i].id << " " << cards[i].name << std::endl;
    }
  }

  void processCards(const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("Collection not found.");
    }
    Vector<Card> all = it->second.getAllCards();
    for (size_t i = 0; i < all.size(); ++i)
    {
      out << all[i].id << " " << all[i].name << " " << all[i].type << std::endl;
    }
  }

  void processSave(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: save <filename>");
    }
    const std::string& filename = tokens[1];
    std::ofstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot create file.");
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("Collection not found.");
    }
    it->second.saveToStream(file);
    out << "Collection saved to " << filename << std::endl;
  }

  void processLoad(const Vector<std::string>& tokens,
    const std::string& current,
    RBTree<std::string, Collection>& collections,
    std::ostream& out)
  {
    if (current.empty())
    {
      throw std::runtime_error("No active collection.");
    }
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: load <filename>");
    }
    const std::string& filename = tokens[1];
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot open file.");
    }
    auto it = collections.find(current);
    if (it == collections.end())
    {
      throw std::runtime_error("Collection not found.");
    }
    it->second.loadFromStream(file);
    out << "Collection loaded from " << filename << std::endl;
  }

  void processCreateSetToId(const Vector<std::string>& tokens,
    const RBTree<size_t, Card>& catalog,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 3)
    {
      throw std::runtime_error("Usage: create_set_to_id <name> <id1> <id2> ...");
    }
    std::string name = tokens[1];
    Vector<size_t> ids;
    for (size_t i = 2; i < tokens.size(); ++i)
    {
      ids.push_back(std::stoull(tokens[i]));
    }
    Vector<size_t> validIds;
    for (size_t j = 0; j < ids.size(); ++j)
    {
      size_t id = ids[j];
      if (catalog.find(id) != catalog.cend())
      {
        validIds.push_back(id);
      }
      else
      {
        throw std::runtime_error("Card with ID " + std::to_string(id) +
          " not found in global catalog");
      }
    }
    if (validIds.empty())
    {
      throw std::runtime_error("No valid IDs provided.");
    }
    if (sets.hasSet(name))
    {
      throw std::runtime_error("Set already exists.");
    }
    sets.addSet(name, validIds);
    out << "Set '" << name << "' created with " << validIds.size() << " cards.\n";
  }

  void processCreateSetToType(const Vector<std::string>& tokens,
    const RBTree<size_t, Card>& catalog,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() != 3)
    {
      throw std::runtime_error("Usage: create_set_to_type <name> <type>");
    }
    std::string name = tokens[1];
    std::string type = tokens[2];
    if (sets.hasSet(name))
    {
      throw std::runtime_error("Set already exists.");
    }
    Vector<size_t> ids;
    for (auto it = catalog.cbegin(); it != catalog.cend(); ++it)
    {
      if (it->second.type == type)
      {
        ids.push_back(it->second.id);
      }
    }
    if (ids.empty())
    {
      throw std::runtime_error("No cards of type '" + type + "' in catalog.");
    }
    sets.addSet(name, ids);
    out << "Set '" << name << "' created with " << ids.size() << " cards.\n";
  }

  void processCheckSet(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: check_set <name>");
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("No active collection.");
    }
    checkSet(tokens[1], it->second, catalog, sets, out);
  }

  void processUpgradeSet(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 3)
    {
      throw std::runtime_error("Usage: upgrade_set <name> <budget> [rarity|power]");
    }
    std::string name = tokens[1];
    size_t budget = std::stoull(tokens[2]);
    std::string criterion = (tokens.size() >= 4) ? tokens[3] : "rarity";
    if (criterion != "rarity" && criterion != "power")
    {
      criterion = "rarity";
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("No active collection.");
    }
    upgradeSet(name, budget, criterion, it->second, catalog, sets, out);
  }

  void processCompletableSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: completable_sets <budget> [rarity|power]");
    }
    size_t budget = std::stoull(tokens[1]);
    std::string criterion = (tokens.size() >= 3) ? tokens[2] : "rarity";
    if (criterion != "rarity" && criterion != "power")
    {
      criterion = "rarity";
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("No active collection.");
    }
    completableSets(budget, criterion, it->second, catalog, sets, out);
  }

  void processMinCostForSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: min_cost_for_sets <set1> <set2> ...");
    }
    Vector<std::string> setNames;
    for (size_t i = 1; i < tokens.size(); ++i)
    {
      setNames.push_back(tokens[i]);
    }
    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("No active collection.");
    }
    minCostForSets(setNames, it->second, catalog, sets, out);
  }

  void processMaxSets(const Vector<std::string>& tokens,
    const std::string& current,
    const RBTree<std::string, Collection>& collections,
    const RBTree<size_t, Card>& catalog,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: max_sets <budget> [count|rarity|power]");
    }

    size_t budget = std::stoull(tokens[1]);
    std::string mode = getValidMode(tokens);

    auto it = collections.find(current);
    if (it == collections.cend())
    {
      throw std::runtime_error("No active collection.");
    }
    maxSets(budget, mode, it->second, catalog, sets, out);
  }

  void processSaveSets(const Vector<std::string>& tokens,
    const SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: save_sets <filename>");
    }
    const std::string& filename = tokens[1];
    std::ofstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot create file.");
    }
    sets.saveToStream(file);
    out << "Sets saved to " << filename << std::endl;
  }

  void processLoadSets(const Vector<std::string>& tokens,
    SetCollection& sets,
    std::ostream& out)
  {
    if (tokens.size() < 2)
    {
      throw std::runtime_error("Usage: load_sets <filename>");
    }
    const std::string& filename = tokens[1];
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot open file.");
    }
    sets.loadFromStream(file);
    out << "Sets loaded from " << filename << std::endl;
  }

  void processSaveAll(const Vector<std::string>&,
    const RBTree<std::string, Collection>& collections,
    const SetCollection& sets,
    const std::string& setsFile,
    const std::string& collFile,
    std::ostream& out)
  {
    if (!setsFile.empty())
    {
      std::ofstream sf(setsFile);
      if (!sf)
      {
        throw std::runtime_error("Cannot write sets file.");
      }
      sets.saveToStream(sf);
      out << "Sets saved to " << setsFile << std::endl;
    }
    if (!collFile.empty())
    {
      std::ofstream cf(collFile);
      if (!cf)
      {
        throw std::runtime_error("Cannot write collections file.");
      }
      for (auto it = collections.cbegin(); it != collections.cend(); ++it)
      {
        cf << it->first;
        Vector<Card> cards = it->second.getAllCards();
        for (size_t i = 0; i < cards.size(); ++i)
        {
          cf << " " << cards[i];
        }
        cf << "\n";
      }
      out << "Collections saved to " << collFile << std::endl;
    }
  }

}
