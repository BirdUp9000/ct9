#include <filesystem>
#include <fstream>
#include <iostream>

#include "../include/ct9/Trie.h"

int main() {

  std::cout << "Hello, World" << "!\n";


  std::ifstream file("../tests/words.txt");

  if (!file.is_open()) {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }

  std::string line;
  // Чтение файла построчно
  while (std::getline(file, line)) {
    std::cout << line << std::endl;
  }

  Trie t(line);
  auto res = t.autocomplete("");

  //for (;;) {
  //  if (res.empty()) break;
  //  std::cout << res.front() << std::endl;
  //  res.pop();
  //}

  Trie t2(std::move(t));

  auto res2 = t2.autocomplete("");
  for (;;) {
    if (res2.empty()) break;
    std::cout << res2.front() << std::endl;
    res2.pop();
  }

  t.insert("fuck");

  t2 = t;

  return 0;
}
