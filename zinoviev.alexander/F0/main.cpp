#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "Vector.hpp"
#include "Collection.hpp"
#include "Set_Collection.hpp"
#include "Functions_for_main.hpp"
#include "RBTree.hpp"

int main(int argc, char* argv[])
{
  using namespace zinoviev;
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0]
      << " <catalog_file> [sets_file] [collections_file]\n";
    return 1;
  }

  RBTree< size_t, Card > catalog;
  RBTree< std::string, Collection > collections;
  std::string current;
  SetCollection sets;

  std::string setsFileName, collFileName;
  if (argc >= 3)
    setsFileName = argv[2];
  if (argc >= 4)
    collFileName = argv[3];

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
    if (sf) sets.loadFromStream(sf);
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
          continue;
        Vector<std::string> tokens = splitLine(line);
        if (tokens.empty())
          continue;
        std::string colName = tokens[0];
        Collection col;
        for (size_t i = 1; i + 5 < tokens.size(); i += 6)
        {
          Card card;
          card.id = std::stoull(tokens[i]);
          card.name = tokens[i + 1];
          card.type = tokens[i + 2];
          card.rarity = std::stoull(tokens[i + 3]);
          card.price = std::stoull(tokens[i + 4]);
          card.power = std::stoull(tokens[i + 5]);
          col.addCard(card);
        }
        collections.insert(colName, col);
      }
    }
  }

  std::cout << "write 'help' to view the commands\n";

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
      continue;
    Vector<std::string> tokens = splitLine(line);
    if (tokens.empty())
      continue;
    const std::string& cmd = tokens[0];

    if (cmd == "help")
      printHelp(std::cout);
    else if (cmd == "quit" || cmd == "exit")
      break;
    else if (cmd == "create")
      processCreate(tokens, collections, std::cout);
    else if (cmd == "use")
      processUse(tokens, collections, current, std::cout);
    else if (cmd == "list")
      processList(collections, current, std::cout);
    else if (cmd == "drop")
      processDrop(tokens, collections, current, std::cout);
    else if (cmd == "add")
      processAdd(tokens, current, collections, std::cout);
    else if (cmd == "remove")
      processRemove(tokens, current, collections, std::cout);
    else if (cmd == "card")
      processCard(tokens, current, collections, std::cout);
    else if (cmd == "filter")
      processFilter(tokens, current, collections, std::cout);
    else if (cmd == "cards")
      processCards(current, collections, std::cout);
    else if (cmd == "save")
      processSave(tokens, current, collections, std::cout);
    else if (cmd == "load")
      processLoad(tokens, current, collections, std::cout);
    else if (cmd == "create_set_to_id")
      processCreateSetToId(tokens, catalog, sets, std::cout);
    else if (cmd == "create_set_to_type")
      processCreateSetToType(tokens, catalog, sets, std::cout);
    else if (cmd == "check_set")
      processCheckSet(tokens, current, collections, catalog, sets, std::cout);
    else if (cmd == "upgrade_set")
      processUpgradeSet(tokens, current, collections, catalog, sets, std::cout);
    else if (cmd == "completable_sets")
      processCompletableSets(tokens, current, collections, catalog, sets, std::cout);
    else if (cmd == "min_cost_for_sets")
      processMinCostForSets(tokens, current, collections, catalog, sets, std::cout);
    else if (cmd == "max_sets")
      processMaxSets(tokens, current, collections, catalog, sets, std::cout);
    else if (cmd == "save_sets")
      processSaveSets(tokens, sets, std::cout);
    else if (cmd == "load_sets")
      processLoadSets(tokens, sets, std::cout);
    else if (cmd == "save_all")
      processSaveAll(tokens, collections, sets, setsFileName, collFileName, std::cout);
    else
      std::cout << "Unknown command.\n";
  }

  if (!setsFileName.empty())
  {
    std::ofstream sf(setsFileName);
    if (sf)
      sets.saveToStream(sf);
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
        for (const Card& c : cards)
        {
          cf << " " << c.id << " " << c.name << " " << c.type
            << " " << c.rarity << " " << c.price << " " << c.power;
        }
        cf << "\n";
      }
    }
  }
}

