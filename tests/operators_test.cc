#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Trie Operators", "[Trie]") {

  SECTION("Assignment Operator (=)") {
    Trie trie1(std::vector<std::string>{"apple", "banana", "cherry"});
    Trie trie2;

    trie2 = trie1;

    REQUIRE(trie2.autocomplete("", INT_MAX).size() == 3);
    REQUIRE(trie2.autocomplete("", INT_MAX) == trie1.autocomplete("", INT_MAX));
  }
  SECTION("Addition Operator (+)") {
    Trie trie1(std::vector<std::string>{"hello", "world"});
    Trie trie2(std::vector<std::string>{"test", "case"});

    Trie result = trie1 + trie2;

    REQUIRE(result.autocomplete("", INT_MAX).size() == 4);
    REQUIRE(result.autocomplete("", INT_MAX) == std::queue<std::string>({"case", "hello", "test", "world"}));
  }

  SECTION("Subtraction Operator (-)") {
    Trie trie1(std::vector<std::string>{"hello", "world", "test"});
    Trie trie2(std::vector<std::string>{"test"});

    Trie result = trie1 - trie2;

    REQUIRE(result.autocomplete("", INT_MAX).size() == 2);
    REQUIRE(result.autocomplete("", INT_MAX) == std::queue<std::string>({"hello", "world"}));
  }
  SECTION("Equality and Inequality Operators (==, !=)") {
    Trie trie1(std::vector<std::string>{"apple", "banana", "cherry"});
    Trie trie2(std::vector<std::string>{"apple", "banana", "cherry"});
    Trie trie3(std::vector<std::string>{"apple", "banana"});
    Trie trie4(std::vector<std::string>{"apple", "banana", "date"});

    REQUIRE(trie1 == trie2);
    REQUIRE_FALSE(trie1 != trie2);
    REQUIRE(trie1 != trie3);
    REQUIRE(trie1 != trie4);
  }

  SECTION("Comparison Operators (<, <=, >, >=)") {
    Trie trie1(std::vector<std::string>{"apple", "banana", "cherry"});
    Trie trie2(std::vector<std::string>{"apple", "banana", "cherry"});
    Trie trie3(std::vector<std::string>{"apple", "banana"});
    Trie trie4(std::vector<std::string>{"apple", "banana", "date"});

    REQUIRE(trie3 < trie1);
    REQUIRE(trie3 <= trie1);
    REQUIRE(trie1 > trie3);
    REQUIRE(trie1 >= trie3);

    REQUIRE(trie1 < trie4);
    REQUIRE(trie1 <= trie4);
    REQUIRE(trie4 > trie1);
    REQUIRE(trie4 >= trie1);

    REQUIRE(trie1 <= trie2);
    REQUIRE(trie1 >= trie2);
  }
}