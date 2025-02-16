
#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"


TEST_CASE("Trie Contain Method") {

    SECTION("Contain existing words") {
      Trie trie(std::vector<std::string>{"apple", "banana", "cherry"});
  
      REQUIRE(trie.contain(std::string("apple")) == true);
      REQUIRE(trie.contain(std::string("banana")) == true);
      REQUIRE(trie.contain(std::string("cherry")) == true);
    }
  
    SECTION("Contain non-existing words") {
      Trie trie(std::vector<std::string>{"apple", "banana", "cherry"});
  
      REQUIRE(trie.contain(std::string("grape")) == false);
      REQUIRE(trie.contain(std::string("pear")) == false);
      REQUIRE(trie.contain(std::string("pineapple")) == false);
    }
  
    SECTION("Contain on empty Trie") {
      Trie trie;
  
      REQUIRE(trie.contain(std::string("apple")) == false);
    }
  }
  