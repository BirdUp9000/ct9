#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Trie Deletion") {

  SECTION("Delete a word that exists in the Trie") {
    Trie trie;
    trie.insert("apple");
    trie.insert("bat");

    trie.del("apple");

    REQUIRE(trie.autocomplete("apple", 1).empty());
    REQUIRE_FALSE(trie.getRoot()->children['a']);
    REQUIRE(trie.autocomplete("bat", 1).size() == 1);
  }

  SECTION("Delete a non-existent word") {
    Trie trie;
    trie.insert("apple");
    trie.insert("bat");

    trie.del("cat");

    REQUIRE(trie.autocomplete("apple", 1).size() == 1);
    REQUIRE(trie.autocomplete("bat", 1).size() == 1);
  }

  SECTION("Delete a word that is a prefix of another word") {
    Trie trie;
    trie.insert("app");
    trie.insert("apple");

    trie.del("app");

    REQUIRE(trie.autocomplete("app", 2).size() == 1);
    REQUIRE(trie.autocomplete("apple", 1).size() == 1);
    REQUIRE(trie.getRoot()->children['a']->children['p']->children['p']->end_of_word == false);
  }

  SECTION("Delete a word that has a common prefix with another word") {
    Trie trie;
    trie.insert("bat");
    trie.insert("batch");

    trie.del("bat");

    REQUIRE(trie.autocomplete("batch", 1).size() == 1);
    REQUIRE(trie.getRoot()->children['b']->children['a']->children['t']->end_of_word == false);
  }

  SECTION("Delete all words in the Trie") {
    Trie trie;
    trie.insert("apple");
    trie.insert("bat");

    trie.del("apple");
    trie.del("bat");

    REQUIRE(trie.autocomplete("apple", 1).empty());
    REQUIRE(trie.autocomplete("bat", 1).empty());
    REQUIRE(trie.getRoot()->children.empty());
  }

  SECTION("Delete word in a complex Trie structure") {
    Trie trie;
    trie.insert("car");
    trie.insert("cart");
    trie.insert("care");
    trie.insert("careful");

    trie.del("care");

    REQUIRE(trie.autocomplete("care", 1).size() == 1);
    REQUIRE(trie.autocomplete("cart", 1).size() == 1);
    REQUIRE(trie.getRoot()->children['c']->children['a']->children['r']->children['e']->end_of_word == false);
  }
}