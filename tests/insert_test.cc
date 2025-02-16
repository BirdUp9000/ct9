#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Trie Insert") {
  Trie trie;

  SECTION("Insert a single word") {
    trie.insert("hello");
    REQUIRE(trie.getRoot()->children.find('h') != trie.getRoot()->children.end());
  }

  SECTION("Insert multiple words") {
    trie.insert("hello");
    trie.insert("world");
    REQUIRE(trie.getRoot()->children.find('h') != trie.getRoot()->children.end());
    REQUIRE(trie.getRoot()->children.find('w') != trie.getRoot()->children.end());
  }

  SECTION("Insert overlapping words") {
    trie.insert("cat");
    trie.insert("car");

    const auto* root = trie.getRoot();
    auto c_node = root->children.find('c');
    REQUIRE(c_node != root->children.end());

    auto a_node = c_node->second->children.find('a');
    REQUIRE(a_node != c_node->second->children.end());

    REQUIRE(a_node->second->children.find('t') != a_node->second->children.end());
    REQUIRE(a_node->second->children.find('r') != a_node->second->children.end());
  }

  SECTION("Insert duplicate words") {
    trie.insert("hello");
    trie.insert("hello");
    REQUIRE(trie.getRoot()->children.find('h') != trie.getRoot()->children.end());
  }
}