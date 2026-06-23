#include "dictionary_options.hpp"

size_t zinoviev::findDict(zinoviev::DictionaryEntry** dicts, size_t count, const std::string& name)
{
  for (size_t i = 0; i < count; ++i)
    if (dicts[i]->getName() == name)
      return i;
  return count;
}

void zinoviev::addDict(zinoviev::DictionaryEntry**& dicts, size_t& count, const std::string& name)
{
  auto newArr = new zinoviev::DictionaryEntry * [count + 1];
  for (size_t i = 0; i < count; ++i)
    newArr[i] = dicts[i];
  try
  {
    newArr[count] = new zinoviev::DictionaryEntry(name);
  }
  catch (...)
  {
    delete[] newArr;
    throw;
  }
  delete[] dicts;
  dicts = newArr;
  ++count;
}
