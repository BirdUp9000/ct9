#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Trie Constructors") {

  SECTION("Default Constructor") {
    Trie trie;
    REQUIRE(trie.getRoot() != nullptr);
    REQUIRE(trie.getRoot()->children.empty());
  }

  SECTION("Constructor with Single String") {
    Trie trie("apple");
    REQUIRE(trie.getRoot()->children.find('a') != trie.getRoot()->children.end());
    REQUIRE(trie.getRoot()->children['a']->children['p']->children['p']->children['l']->children['e']->end_of_word ==
            true);
  }

  SECTION("Constructor with Vector of Strings") {
    std::vector<std::string> words = {"apple", "app", "bat"};
    Trie trie(words);

    // Check 'apple's
    REQUIRE(trie.getRoot()->children['a']->children['p']->children['p']->children['l']->children['e']->end_of_word ==
            true);
    // Check 'app'
    REQUIRE(trie.getRoot()->children['a']->children['p']->children['p']->end_of_word == true);
    // Check 'bat'
    REQUIRE(trie.getRoot()->children['b']->children['a']->children['t']->end_of_word == true);
  }

  SECTION("Copy Constructor") {
    Trie originalTrie(std::vector<std::string>{"apple", "bat"});
    Trie copiedTrie(originalTrie);

    REQUIRE(
        copiedTrie.getRoot()->children['a']->children['p']->children['p']->children['l']->children['e']->end_of_word ==
        true);
    REQUIRE(copiedTrie.getRoot()->children['b']->children['a']->children['t']->end_of_word == true);

    originalTrie.insert("ball");
    REQUIRE(originalTrie.getRoot()->children['b']->children['a']->children['l']->children['l']->end_of_word == true);
    REQUIRE(copiedTrie.getRoot()->children['b']->children.find('l') ==
            copiedTrie.getRoot()->children['b']->children.end());
  }

  SECTION("Move Constructor") {
    std::vector<std::string> words = {"apple", "bat"};
    Trie originalTrie(words);  // Create trie from vector
    Trie movedTrie(std::move(originalTrie));
    // Check that the moved trie has the data
    REQUIRE(
        movedTrie.getRoot()->children['a']->children['p']->children['p']->children['l']->children['e']->end_of_word ==
        true);
    REQUIRE(movedTrie.getRoot()->children['b']->children['a']->children['t']->end_of_word == true);
  }
}
