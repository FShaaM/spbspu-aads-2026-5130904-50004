#include <algorithm>
#include "Graph.hpp"

namespace zinoviev
{
  namespace
  {
    template<class T>
    void sort(T& v, size_t begin, size_t end)
    {
      if (end - begin <= 1)
      {
        return;
      }

      const size_t id = end - 1;
      T pivot = v[id];

      size_t i = begin;
      for (size_t j = i; j < end; ++j)
      {
        if (v[j] < pivot)
        {
          std::swap(v[i], v[j]);
          ++i;
        }
      }
      std::swap(v[i], v[id]);

      sort(v, begin, i);
      sort(v, i + 1, end);
    }

    void sort_pair(Vector< std::pair< std::string, Vector< unsigned long long > > >& p,
                   size_t begin,
                   size_t end)
    {
      if (end - begin <= 1)
      {
        return;
      }

      const size_t id = end - 1;
      auto pivot = p[id];

      size_t i = begin;
      for (size_t j = i; j < end; ++j)
      {
        if (p[j].first < pivot.first)
        {
          std::swap(p[i], p[j]);
          ++i;
        }
      }
      std::swap(p[i], p[id]);

      sort_pair(p, begin, i);
      sort_pair(p, i + 1, end);
    }

    template<class T>
    void sortVector(T& v)
    {
      sort(v, 0, v.getSize());
    }

    void sortPair(Vector< std::pair< std::string, Vector< unsigned long long > > >& p)
    {
      sort_pair(p, 0, p.getSize());
    }
  } // namespace

  std::size_t StringHasher::operator()(const std::string& p) const
  {
    boost::hash2::sha1_160 hasher;
    boost::hash2::hash_append(hasher, {}, p);
    return boost::hash2::get_integral_result(hasher.result());
  }

  std::size_t PairHasher::operator()(const std::pair< std::string, std::string >& p) const
  {
    boost::hash2::sha1_160 hasher;
    boost::hash2::hash_append(hasher, {}, p.first);
    boost::hash2::hash_append(hasher, {}, p.second);
    return boost::hash2::get_integral_result(hasher.result());
  }

  Graph::Graph(const std::string& name):
    name_(name),
    vertexes_(16, 6, 16),
    edges_(18, 6, 16)
  {
  }

  Graph::Graph(const Graph& g):
    name_(g.name_),
    vertexes_(g.vertexes_),
    edges_(g.edges_)
  {
  }

  void Graph::add_vertex(const std::string& v)
  {
    vertexes_.add(v, true);
  }

  void Graph::add_edge(const std::string& from, const std::string& to, unsigned long long weight)
  {
    auto vertices_copy = vertexes_;
    auto edges_copy = edges_;

    auto key = std::make_pair(from, to);
    Vector< unsigned long long >* existing = edges_copy.find(key);

    Vector< unsigned long long > new_weights;
    if (existing)
    {
      new_weights = *existing;
      new_weights.push_back(weight);
    }
    else
    {
      new_weights.push_back(weight);
    }

    vertices_copy.add(from, true);
    vertices_copy.add(to, true);

    edges_copy.add(key, new_weights);

    vertexes_.swap(vertices_copy);
    edges_.swap(edges_copy);
  }

  bool Graph::has_vertex(const std::string& v) const
  {
    return vertexes_.has(v);
  }

  Vector< std::string > Graph::get_vertices() const
  {
    Vector< std::string > tmp;

    auto slot_pt = vertexes_.cbegin();
    const auto end = vertexes_.cend();
    while (slot_pt != end)
    {
      tmp.push_back((*slot_pt).first);
      ++slot_pt;
    }

    sortVector(tmp);
    return tmp;
  }

  const Vector< unsigned long long >* Graph::get_weights(const std::string& from,
                                                         const std::string& to) const
  {
    return edges_.find({ from, to });
  }

  void Graph::swap(Graph& other) noexcept
  {
    std::swap(name_, other.name_);
    edges_.swap(other.edges_);
    vertexes_.swap(other.vertexes_);
  }

  std::string Graph::get_name() const
  {
    return name_;
  }

  Vector< std::pair< std::string, Vector< unsigned long long > > >
  Graph::get_outbound(const std::string& vertex) const
  {
    Vector< std::pair< std::string, Vector< unsigned long long > > > res;

    auto edge = edges_.cbegin();
    const auto end = edges_.cend();
    while (edge != end)
    {
      if ((*edge).first.first == vertex)
      {
        res.push_back({ (*edge).first.second, (*edge).second });
      }
      ++edge;
    }

    sortPair(res);
    const size_t count = res.getSize();
    for (size_t i = 0; i < count; ++i)
    {
      sortVector(res[i].second);
    }
    return res;
  }

  Vector< std::pair< std::string, Vector< unsigned long long > > >
  Graph::get_inbound(const std::string& vertex) const
  {
    Vector< std::pair< std::string, Vector< unsigned long long > > > res;

    auto edge = edges_.cbegin();
    const auto end = edges_.cend();
    while (edge != end)
    {
      if ((*edge).first.second == vertex)
      {
        res.push_back({ (*edge).first.first, (*edge).second });
      }
      ++edge;
    }

    sortPair(res);
    const size_t count = res.getSize();
    for (size_t i = 0; i < count; ++i)
    {
      sortVector(res[i].second);
    }
    return res;
  }

  void Graph::remove_edge(const std::pair< std::string, std::string >& p,
                          unsigned long long weight)
  {
    Vector< unsigned long long >* ptr = edges_.find(p);

    if (!ptr)
    {
      throw std::out_of_range("Can't find this edge");
    }

    const size_t size = ptr->getSize();
    size_t id = size;
    for (size_t i = 0; i < size; ++i)
    {
      if (ptr->operator[](i) == weight)
      {
        id = i;
        break;
      }
    }

    if (id == size)
    {
      throw std::out_of_range("Can't find this weight");
    }

    ptr->erase(id);

    if (ptr->is_empty())
    {
      edges_.erase(p);
    }
  }

  void Graph::add_graph(const Graph& other)
  {
    Graph tmp(*this);

    Vector< std::string > ver = other.get_vertices();
    const size_t ver_count = ver.getSize();
    for (size_t i = 0; i < ver_count; ++i)
    {
      tmp.add_vertex(ver[i]);
    }

    auto citer = other.edges_.cbegin();
    const auto cend = other.edges_.cend();
    while (citer != cend)
    {
      const Vector< unsigned long long >& weights = (*citer).second;
      const size_t w_count = weights.getSize();
      for (size_t i = 0; i < w_count; ++i)
      {
        tmp.add_edge((*citer).first.first, (*citer).first.second, weights[i]);
      }
      ++citer;
    }

    swap(tmp);
  }
}
