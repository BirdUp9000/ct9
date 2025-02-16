// size_test.cpp

#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Size test on empty tree.") {
  Trie t{};
  REQUIRE(t.size() == 0);
}

TEST_CASE("Size test on trie with one word.") {
  Trie t{};
  t.insert("hello");
  REQUIRE(t.size() == 5);
}

TEST_CASE("Size test on trie with many words.") {
  Trie t{};
  t.insert("hello");
  t.insert("world");
  REQUIRE(t.size() == 10);
}

TEST_CASE("Size test on trie after deletion.") {
  Trie t{};
  t.insert("hello");
  t.del("hello");
  REQUIRE(t.size() == 0);
}

TEST_CASE("Size test on trie after copy.") {
  Trie t1{};
  t1.insert("hello");

  Trie t2(t1);
  REQUIRE(t2.size() == 5);
}

TEST_CASE("Size test on trie after assignment.") {
  Trie t1{};
  t1.insert("hello");

  Trie t2 = t1;
  REQUIRE(t2.size() == 5);
}

TEST_CASE("Size test on trie after move.") {
  Trie t1{};
  t1.insert("hello");

  Trie t2(std::move(t1));
  REQUIRE(t2.size() == 5);
}








