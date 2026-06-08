#include "Functions_for_Collections.hpp"

namespace zinoviev
{
  std::istream& loadCatalog(std::istream& input, RBTree< size_t, Card >& catalog)
  {
    if (!input)
      return input;

    Card card;
    while (input >> card.id >> card.name >> card.type >> card.rarity >> card.power >> card.price)
    {
      catalog.insert(card.id, card);
    }
    return input;
  }

  std::pair< size_t, Vector< size_t > > knapsack(const RBTree<size_t, Card>& catalog, size_t budget, const std::string& criterion)
  {
    Vector< Card > items;
    for (auto it = catalog.cbegin(); it != catalog.cend(); ++it)
      items.push_back(it->second);

    Vector< size_t > dp(budget + 1, 0);
    Vector< const Card* > last_id(budget + 1, nullptr);
    Vector< size_t > old_budget(budget + 1, 0);

    for (size_t i = 0; i < items.size(); ++i)
    {
      size_t p = items[i].price;
      size_t v = (criterion == "power") ? items[i].power : items[i].rarity;
      for (size_t b = budget; b >= p; --b)
      {
        if (dp[b - p] + v > dp[b])
        {
          dp[b] = dp[b - p] + v;
          last_id[b] = &items[i];
          old_budget[b] = b - p;
        }
      }
    }

    Vector< size_t > selected_ids;
    size_t b = budget;

    while (b > 0 && last_id[b])
    {
      selected_ids.push_back(last_id[b]->id);
      b = old_budget[b];
    }

    return std::make_pair(dp[budget], selected_ids);
  }
}
