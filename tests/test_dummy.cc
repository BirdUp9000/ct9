// test_dummy.cpp

#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"
TEST_CASE("Dummy test example") { REQUIRE(1 == 1); }

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

TEST_CASE("Trie Autocomplete") {
  Trie trie;

  trie.insert("apple");
  trie.insert("app");
  trie.insert("application");
  trie.insert("apricot");
  trie.insert("banana");
  trie.insert("band");
  trie.insert("bat");

  SECTION("Autocomplete with existing prefix") {
    auto results = trie.autocomplete("app");

    std::vector<std::string> expected = {"app", "apple", "application"};
    REQUIRE(results.size() == expected.size());

    for (const auto& expected_word : expected) {
      REQUIRE(results.front() == expected_word);
      results.pop();
    }
  }

  SECTION("Autocomplete with non-existing prefix") {
    auto results = trie.autocomplete("xyz");
    REQUIRE(results.empty());
  }

  SECTION("Autocomplete with partial match") {
    auto results = trie.autocomplete("ap");

    std::vector<std::string> expected = {"app", "apple", "application", "apricot"};
    REQUIRE(results.size() == expected.size());

    for (const auto& expected_word : expected) {
      REQUIRE(results.front() == expected_word);
      results.pop();
    }
  }

  SECTION("Autocomplete with full word match") {
    auto results = trie.autocomplete("apple");

    std::vector<std::string> expected = {"apple"};
    REQUIRE(results.size() == expected.size());

    REQUIRE(results.front() == expected[0]);
  }

  SECTION("Autocomplete with limited results") {
    auto results = trie.autocomplete("ap", 2);

    std::vector<std::string> expected = {"app", "apple"};
    REQUIRE(results.size() == expected.size());

    for (const auto& expected_word : expected) {
      REQUIRE(results.front() == expected_word);
      results.pop();
    }
  }

  SECTION("Autocomplete with single character prefix") {
    auto results = trie.autocomplete("b");

    std::vector<std::string> expected = {"banana", "band", "bat"};
    REQUIRE(results.size() == expected.size());

    for (const auto& expected_word : expected) {
      REQUIRE(results.front() == expected_word);
      results.pop();
    }
  }
}
