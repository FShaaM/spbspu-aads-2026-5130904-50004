#include <iostream>
#include <fstream>
#include <string>
#include "BSTree.hpp"
#include "DictionaryEntry.hpp"
#include "dictionary_options.hpp"

int main(int argc, const char** argv)
{
  if (argc < 2)
  {
    std::cout << "No filename provided\n";
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file.is_open())
  {
    std::cout << "Can't open file\n";
    return 1;
  }

  zinoviev::DictionaryEntry** dictionaries = nullptr;
  size_t dictCount = 0;

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
      continue;

    size_t pos = 0;
    size_t size = line.size();
    while (pos < size && line[pos] == ' ')
      ++pos;

    if (pos == size)
      continue;

    std::string name_dict;
    while (pos < size && line[pos] != ' ')
    {
      name_dict += line[pos];
      ++pos;
    }

    size_t id = zinoviev::findDict(dictionaries, dictCount, name_dict);

    try
    {
      if (id == dictCount)
        zinoviev::addDict(dictionaries, dictCount, name_dict);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    while (pos < size)
    {
      while (pos < size && line[pos] == ' ')
        ++pos;

      std::string key_str;
      while (pos < size && line[pos] != ' ')
      {
        key_str += line[pos];
        ++pos;
      }

      unsigned long long key = 0.;
      try
      {
        key = std::stoull(key_str);
      }
      catch (...)
      {
        continue;
      }

      while (pos < size && line[pos] == ' ')
        ++pos;

      std::string val_str;
      while (pos < size && line[pos] != ' ')
      {
        val_str += line[pos];
        ++pos;
      }

      dictionaries[id]->getDictionary().push(key, val_str);
    }
  }

  std::string cmdLine;
  while (std::getline(std::cin, cmdLine))
  {
    if (cmdLine.empty())
      continue;

    size_t pos = 0;
    size_t size = cmdLine.size();

    while (pos < size && cmdLine[pos] == ' ')
      ++pos;

    if (pos == size)
      continue;

    std::string command;
    while (pos < size && cmdLine[pos] != ' ')
    {
      command += cmdLine[pos];
      ++pos;
    }

    while (pos < size && cmdLine[pos] == ' ')
      ++pos;

    if (command == "print")
    {
      std::string argument_1;
      while (pos < size && cmdLine[pos] != ' ')
      {
        argument_1 += cmdLine[pos];
        ++pos;
      }

      while (pos < size && cmdLine[pos] == ' ')
        ++pos;

      if (argument_1.empty() || pos != size)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      size_t id = zinoviev::findDict(dictionaries, dictCount, argument_1);
      if (id == dictCount)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      auto& Dictionary = dictionaries[id]->getDictionary();
      auto it = Dictionary.begin();
      if (it == Dictionary.end())
      {
        std::cout << "<EMPTY>\n";
      }
      else
      {
        std::cout << argument_1;
        for (; it != Dictionary.end(); ++it)
        {
          std::cout << ' ' << it->first << ' ' << it->second;
        }
        std::cout << '\n';
      }
    }
    else if (command == "complement" || command == "intersect" || command == "union")
    {
      std::string argument_1, argument_2, argument_3;
      while (pos < size && cmdLine[pos] != ' ')
      {
        argument_1 += cmdLine[pos];
        ++pos;
      }

      if (argument_1.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      while (pos < size && cmdLine[pos] == ' ')
        ++pos;
      while (pos < size && cmdLine[pos] != ' ')
      {
        argument_2 += cmdLine[pos];
        ++pos;
      }

      if (argument_2.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      while (pos < size && cmdLine[pos] == ' ')
        ++pos;
      while (pos < size && cmdLine[pos] != ' ')
      {
        argument_3 += cmdLine[pos];
        ++pos;
      }

      if (argument_3.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      while (pos < size && cmdLine[pos] == ' ')
        ++pos;
      if (pos != size)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      if (command == "complement")
      {
        size_t id_1 = zinoviev::findDict(dictionaries, dictCount, argument_1);
        size_t id_2 = zinoviev::findDict(dictionaries, dictCount, argument_2);
        size_t id_3 = zinoviev::findDict(dictionaries, dictCount, argument_3);
        if (id_1 != dictCount || id_2 == dictCount || id_3 == dictCount)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        try
        {
          zinoviev::addDict(dictionaries, dictCount, argument_1);
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        auto& tree2 = dictionaries[id_2]->getDictionary();
        auto& tree3 = dictionaries[id_3]->getDictionary();
        auto& newTree1 = dictionaries[dictCount - 1]->getDictionary();

        auto citer_2 = tree2.cbegin();
        auto cend_2 = tree2.cend();

        while (citer_2 != cend_2)
        {
          if (!tree3.contains(citer_2->first))
            newTree1.push(citer_2->first, citer_2->second);

          ++citer_2;
        }
      }
      else if (command == "intersect")
      {
        size_t id_1 = zinoviev::findDict(dictionaries, dictCount, argument_1);
        size_t id_2 = zinoviev::findDict(dictionaries, dictCount, argument_2);
        size_t id_3 = zinoviev::findDict(dictionaries, dictCount, argument_3);
        if (id_1 != dictCount || id_2 == dictCount || id_3 == dictCount)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        try
        {
          zinoviev::addDict(dictionaries, dictCount, argument_1);
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        auto& tree2 = dictionaries[id_2]->getDictionary();
        auto& tree3 = dictionaries[id_3]->getDictionary();
        auto& newTree1 = dictionaries[dictCount - 1]->getDictionary();

        auto citer_2 = tree2.cbegin();
        auto cend_2 = tree2.cend();

        while (citer_2 != cend_2)
        {
          if (tree3.contains(citer_2->first))
            newTree1.push(citer_2->first, citer_2->second);

          ++citer_2;
        }
      }
      else if (command == "union")
      {
        size_t id_1 = zinoviev::findDict(dictionaries, dictCount, argument_1);
        size_t id_2 = zinoviev::findDict(dictionaries, dictCount, argument_2);
        size_t id_3 = zinoviev::findDict(dictionaries, dictCount, argument_3);
        if (id_1 != dictCount || id_2 == dictCount || id_3 == dictCount)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        try
        {
          zinoviev::addDict(dictionaries, dictCount, argument_1);
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        auto& tree2 = dictionaries[id_2]->getDictionary();
        auto& tree3 = dictionaries[id_3]->getDictionary();
        auto& newTree1 = dictionaries[dictCount - 1]->getDictionary();

        for (auto it = tree2.cbegin(); it != tree2.cend(); ++it)
          newTree1.push(it->first, it->second);

        auto citer_3 = tree3.cbegin();
        auto cend_3 = tree3.cend();

        while (citer_3 != cend_3)
        {
          if (!tree2.contains(citer_3->first))
            newTree1.push(citer_3->first, citer_3->second);

          ++citer_3;
        }
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  for (size_t i = 0; i < dictCount; ++i)
    delete dictionaries[i];
  delete[] dictionaries;
}
