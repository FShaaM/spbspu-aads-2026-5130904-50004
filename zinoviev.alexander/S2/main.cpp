#include <iostream>
#include <fstream>
#include <string>
#include "evaluate.hpp"
#include "Stack.hpp"

int main(int argc, char* argv[])
{
  using namespace zinoviev;

  std::ifstream input;
  std::istream& in = (argc == 2) ? input : std::cin;

  if (argc == 2)
  {
    input.open(argv[1]);
    if (!input)
    {
      std::cerr << "Cannot open file\n";
      return 1;
    }
  }
  else if (argc > 2)
  {
    std::cerr << "Usage: " << argv[0] << " [filename]\n";
    return 1;
  }

  Stack< long long > results;
  std::string line;

  while (std::getline(in, line))
  {
    if (!line.empty())
    {
      try
      {
        results.push(evaluate(line));
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
      }
    }
  }

  if (!results.empty())
  {
    std::cout << results.top();
    results.pop();
    while (!results.empty())
    {
      std::cout << ' ' << results.top();
      results.pop();
    }
  }
  std::cout << '\n';
}
