#include "Set_Operations.hpp"

#include <iostream>
#include <string>

#include "Collection.hpp"
#include "Vector.hpp"

namespace zinoviev
{
  std::istream& assemble_set_by_id(std::istream& input, std::ostream& out,
      const RBTree< size_t, Card >& catalog, SetCollection& sets)
  {
    if (!input)
    {
      return input;
    }

    std::string name;
    if (!(input >> name) || sets.hasSet(name))
    {
      out << "<ERROR> : BAD NAME\n";
      return input;
    }

    Vector< size_t > v_id;
    size_t id = 0;
    while (input >> id)
    {
      if (catalog.find(id) != catalog.cend())
      {
        v_id.push_back(id);
      }
      else
      {
        out << "Card with ID " << id << " not found in the global catalog\n";
      }
    }

    if (!v_id.empty())
    {
      sets.addSet(name, v_id);
    }
    else
    {
      out << "Сouldn't collect cards into the set\n";
    }

    return input;
  }

  std::istream& assemble_set_by_type(std::istream& input, std::ostream& out,
      const RBTree< size_t, Card >& catalog, SetCollection& sets)
  {
    std::string name, type;
    if (!(input >> name) || sets.hasSet(name))
    {
      out << "<ERROR> : BAD NAME\n";
      return input;
    }

    if (!(input >> type))
    {
      out << "<ERROR> : BAD TYPE\n";
      return input;
    }

    Vector< size_t > v_id;
    for (auto it = catalog.cbegin(); it != catalog.cend(); ++it)
    {
      if (it->second.type == type)
      {
        v_id.push_back(it->second.id);
      }
    }

    if (!v_id.empty())
    {
      sets.addSet(name, v_id);
    }
    else
    {
      out << "Cards with TYPE " << type << " not found in the global catalog\n";
    }

    return input;
  }

  void upgradeSet(const std::string& setName, size_t budget, const std::string& criterion,
      const Collection& collection, const RBTree< size_t, Card >& catalog,
      const SetCollection& sets, std::ostream& out)
  {
    const Vector< size_t >* set_ids = sets.getSet(setName);
    if (!set_ids)
    {
      out << "Set not found\n";
      return;
    }

    RBTree< size_t, Card > missingCards;
    for (size_t i = 0; i < set_ids->size(); ++i)
    {
      size_t id = (*set_ids)[i];
      if (!collection.findCard(id))
      {
        auto card = catalog.find(id);
        if (card != catalog.cend())
        {
          missingCards.insert(card->first, card->second);
        }
      }
    }

    if (missingCards.empty())
    {
      out << "Set already completed\n";
      return;
    }

    auto result = knapsack(missingCards, budget, criterion);
    size_t total_value = result.first;
    const Vector< size_t >& selected_ids = result.second;

    if (selected_ids.empty())
    {
      out << "Cannot upgrade set within budget : " << budget << "\n";
      return;
    }

    out << "Upgrade set '" << setName << "' with budget " << budget << " (" << criterion << ")\n";

    size_t totalCost = 0;
    for (size_t i = 0; i < selected_ids.size(); ++i)
    {
      auto card = catalog.find(selected_ids[i]);
      if (card != catalog.cend())
      {
        size_t val = (criterion == "rarity") ? card->second.rarity : card->second.power;
        out << "   " << card->second.name << " (price=" << card->second.price
            << ", " << criterion << "=" << val << ")\n";
        totalCost += card->second.price;
      }
    }
    out << "Total cost: " << totalCost << ", total " << criterion << ": " << total_value << "\n";
  }

  void completableSets(size_t budget, const std::string& criterion,
      const Collection& collection, const RBTree< size_t, Card >& catalog,
      const SetCollection& sets, std::ostream& out)
  {
    Vector< std::string > names;
    Vector< size_t > costs;
    Vector< size_t > powers;
    size_t bestPower = 0;
    std::string bestSet;

    for (auto it = sets.cbegin(); it != sets.cend(); ++it)
    {
      const std::string& name_set = it->first;
      const Vector< size_t >& ids = it->second;

      size_t missingCost = 0;
      size_t totalPower = 0;
      bool valid = true;
      for (size_t i = 0; i < ids.size(); ++i)
      {
        size_t id = ids[i];
        auto card = catalog.find(id);
        if (card != catalog.cend())
        {
          totalPower += (criterion == "rarity") ? card->second.rarity : card->second.power;
          if (collection.findCard(id) == nullptr)
          {
            missingCost += card->second.price;
          }
        }
        else
        {
          valid = false;
          break;
        }
      }

      if (!valid || missingCost > budget)
      {
        continue;
      }

      names.push_back(name_set);
      costs.push_back(missingCost);
      powers.push_back(totalPower);
      if (totalPower > bestPower)
      {
        bestPower = totalPower;
        bestSet = name_set;
      }
    }

    if (names.empty())
    {
      out << "No completable sets within budget " << budget << "\n";
      return;
    }

    out << "Completable sets (budget=" << budget << ", criterion=" << criterion << "):\n";
    for (size_t i = 0; i < names.size(); ++i)
    {
      out << "  " << names[i] << ": cost=" << costs[i] << ", total " << criterion << "=" << powers[i] << "\n";
    }
    out << "Strongest completable set: " << bestSet << " (total " << criterion << "=" << bestPower << ")\n";
  }

  void minCostForSets(const Vector< std::string >& setNames,
      const Collection& collection, const RBTree< size_t, Card >& catalog,
      const SetCollection& sets, std::ostream& out)
  {
    RBTree< size_t, bool > uniqueIds;

    for (size_t s = 0; s < setNames.size(); ++s)
    {
      const Vector< size_t >* ids = sets.getSet(setNames[s]);
      if (!ids)
      {
        out << "Set '" << setNames[s] << "' not found\n";
        return;
      }

      for (size_t i = 0; i < ids->size(); ++i)
      {
        size_t id = (*ids)[i];
        if (collection.findCard(id) == nullptr)
        {
          uniqueIds.insert(id, true);
        }
      }
    }

    if (uniqueIds.empty())
    {
      out << "All sets already completed, no cost\n";
      return;
    }

    Vector< size_t > missingIds;
    for (auto it = uniqueIds.cbegin(); it != uniqueIds.cend(); ++it)
    {
      missingIds.push_back(it->first);
    }

    size_t totalCost = 0;
    for (size_t i = 0; i < missingIds.size(); ++i)
    {
      auto card = catalog.find(missingIds[i]);
      if (card != catalog.cend())
      {
        totalCost += card->second.price;
      }
    }

    out << "Minimum cost to complete all given sets: " << totalCost << "\n";
    out << "Cards to buy: ";
    for (size_t i = 0; i < missingIds.size(); ++i)
    {
      auto card = catalog.find(missingIds[i]);
      if (card != catalog.cend())
      {
        out << card->second.name << " (price=" << card->second.price << ") ";
      }
    }
    out << "\n";
  }

  void maxSets(size_t budget, const std::string& mode,
      const Collection& collection, const RBTree< size_t, Card >& catalog,
      const SetCollection& sets, std::ostream& out)
  {
    Vector< std::string > names;
    Vector< size_t > costs;
    Vector< size_t > values;

    for (auto it = sets.cbegin(); it != sets.cend(); ++it)
    {
      const std::string& setName = it->first;
      const Vector< size_t >& ids = it->second;

      size_t cost = 0;
      bool canComplete = true;
      for (size_t i = 0; i < ids.size(); ++i)
      {
        size_t id = ids[i];
        if (collection.findCard(id) == nullptr)
        {
          auto card = catalog.find(id);
          if (card != catalog.cend())
          {
            cost += card->second.price;
          }
          else
          {
            canComplete = false;
            break;
          }
        }
      }
      if (!canComplete || cost > budget)
      {
        continue;
      }

      size_t value = 0;
      if (mode == "count")
      {
        value = 1;
      }
      else if (mode == "rarity")
      {
        for (size_t i = 0; i < ids.size(); ++i)
        {
          auto card = catalog.find(ids[i]);
          if (card != catalog.cend())
          {
            value += card->second.rarity;
          }
        }
      }
      else if (mode == "power")
      {
        for (size_t i = 0; i < ids.size(); ++i)
        {
          auto card = catalog.find(ids[i]);
          if (card != catalog.cend())
          {
            value += card->second.power;
          }
        }
      }
      else
      {
        out << "Mode must be 'count', 'rarity', or 'power'.\n";
        return;
      }

      names.push_back(setName);
      costs.push_back(cost);
      values.push_back(value);
    }

    if (names.empty())
    {
      out << "No sets can be completed within budget " << budget << "\n";
      return;
    }

    size_t n = names.size();
    Vector< size_t > dp(budget + 1, 0);
    Vector< Vector< bool > > taken(n, Vector< bool >(budget + 1, false));

    for (size_t i = 0; i < n; ++i)
    {
      size_t c = costs[i];
      size_t v = values[i];
      for (size_t b = budget; b >= c; --b)
      {
        if (dp[b - c] + v > dp[b])
        {
          dp[b] = dp[b - c] + v;
          taken[i][b] = true;
        }
      }
    }

    Vector< std::string > selected;
    size_t b = budget;
    for (int i = n - 1; i >= 0; --i)
    {
      if (taken[i][b])
      {
        selected.push_back(names[i]);
        b -= costs[i];
      }
    }

    out << "Selected sets (mode=" << mode << ", total value=" << dp[budget] << "):\n";
    for (size_t i = 0; i < selected.size(); ++i)
    {
      out << "  " << selected[i] << "\n";
    }
  }

  void checkSet(const std::string& setName, const Collection& collection,
      const RBTree< size_t, Card >& catalog, const SetCollection& sets,
      std::ostream& out)
  {
    const Vector< size_t >* ids = sets.getSet(setName);
    if (!ids)
    {
      out << "Set not found.\n";
      return;
    }
    Vector< size_t > missing;
    for (size_t i = 0; i < ids->size(); ++i)
    {
      size_t id = (*ids)[i];
      if (collection.findCard(id) == nullptr)
      {
        missing.push_back(id);
      }
    }
    if (missing.empty())
    {
      out << "Set '" << setName << "' is completed!\n";
    }
    else
    {
      out << "Set '" << setName << "' missing cards: ";
      for (size_t i = 0; i < missing.size(); ++i)
      {
        size_t id = missing[i];
        auto card = catalog.find(id);
        if (card != catalog.cend())
        {
          out << card->second.name << " (id=" << id << ") ";
        }
        else
        {
          out << id << " ";
        }
      }
      out << std::endl;
    }
  }

}
