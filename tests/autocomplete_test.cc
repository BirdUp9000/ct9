#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

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
