// test_dummy.cpp

#include <catch2/catch_all.hpp>
#include "../include/ct9/Trie.h"
TEST_CASE("Dummy test example") {
    REQUIRE(1 == 1);
}
TEST_CASE("text_to_words Functionality and Edge Cases") {
    SECTION("Basic sentence splitting") {
        std::string text = "Hello, world!";
        auto words = text_to_words(text);
        REQUIRE(words.size() == 2);
        REQUIRE(words[0] == "hello");
        REQUIRE(words[1] == "world");
    }

    SECTION("Multiple spaces between words") {
        std::string text = "Hello  ,   world!";
        auto words = text_to_words(text);
        REQUIRE(words.size() == 2);
        REQUIRE(words[0] == "hello");
        REQUIRE(words[1] == "world");
    }

    SECTION("Empty input") {
        std::string text = "";
        auto words = text_to_words(text);
        REQUIRE(words.empty());  // No words in an empty string
    }

    SECTION("Only delimiters") {
        std::string text = "!!!    ";
        auto words = text_to_words(text);
        REQUIRE(words.empty());  // Should return empty list
    }

    SECTION("Single word input") {
        std::string text = "Hello";
        auto words = text_to_words(text);
        REQUIRE(words.size() == 1);
        REQUIRE(words[0] == "hello");  // Word should be converted to lowercase
    }

    SECTION("Punctuation handling") {
        std::string text = "hello, world! This-is: a-test;";
        auto words = text_to_words(text);
        REQUIRE(words.size() == 6);
        REQUIRE(words[0] == "hello");
        REQUIRE(words[1] == "world");
        REQUIRE(words[2] == "this");
        REQUIRE(words[3] == "is");
        REQUIRE(words[4] == "a");
        REQUIRE(words[5] == "test");
    }

    SECTION("Performance test with large input") {
        std::string text = "word " + std::string(1000000, ' ') + "word";
        auto words = text_to_words(text);
        REQUIRE(words.size() == 2);  // Only two words in this large string
    }
}
TEST_CASE("Trie Insert") {
    Trie trie;

    SECTION("Insert a single word") {
        trie.insert("hello");
        REQUIRE(trie.getRoot().children.find('h') != trie.getRoot().children.end());
    }

    SECTION("Insert multiple words") {
        trie.insert("hello");
        trie.insert("world");
        REQUIRE(trie.getRoot().children.find('h') != trie.getRoot().children.end());
        REQUIRE(trie.getRoot().children.find('w') != trie.getRoot().children.end());
    }


    SECTION("Insert overlapping words") {
        trie.insert("cat");
        trie.insert("car");

        auto& root = trie.getRoot();
        auto c_node = root.children.find('c');
        REQUIRE(c_node != root.children.end());

        auto a_node = c_node->second->children.find('a');
        REQUIRE(a_node != c_node->second->children.end());

        REQUIRE(a_node->second->children.find('t') != a_node->second->children.end());
        REQUIRE(a_node->second->children.find('r') != a_node->second->children.end());
  }

    SECTION("Insert duplicate words") {
        trie.insert("hello");
        trie.insert("hello");
        REQUIRE(trie.getRoot().children.find('h') != trie.getRoot().children.end());
    }
}