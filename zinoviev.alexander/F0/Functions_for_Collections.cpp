#include "Functions_for_Collections.hpp"

namespace zinoviev
{

  IOGuard::IOGuard(std::basic_ios< char >& s):
    s_(s),
    precision_(s.precision()),
    width_(s.width()),
    flags_(s.flags()),
    fill_(s.fill())
  {
  }

  IOGuard::~IOGuard()
  {
    s_.precision(precision_);
    s_.width(width_);
    s_.flags(flags_);
    s_.fill(fill_);
  }

  std::istream& loadCatalog(std::istream& input, RBTree< size_t, Card >& catalog)
  {
    if (!input)
    {
      return input;
    }

    RBTree< size_t, Card > tmp_catalog;
    Card card;
    while (input >> card)
    {
      tmp_catalog.insert(card.id, card);
    }

    catalog.swap(tmp_catalog);
    return input;
  }

  std::pair< size_t, Vector< size_t > > knapsack(const RBTree< size_t, Card >& catalog,
    size_t budget,
    const std::string& criterion)
  {
    Vector< Card > items;
    for (auto it = catalog.cbegin(); it != catalog.cend(); ++it)
    {
      items.push_back(it->second);
    }

    Vector< size_t > dp(budget + 1, 0);
    Vector< const Card* > last_id(budget + 1, nullptr);
    Vector< size_t > old_budget(budget + 1, 0);

    auto get_weight = [&criterion](const Card& card) noexcept -> size_t
    {
      return (criterion == "power") ? card.power : card.rarity;
    };

    for (size_t i = 0; i < items.size(); ++i)
    {
      const size_t p = items[i].price;
      const size_t v = get_weight(items[i]);

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
