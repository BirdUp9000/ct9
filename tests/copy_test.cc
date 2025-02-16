#include <catch2/catch_all.hpp>
#include <iostream>

#include "../include/ct9/Trie.h"

TEST_CASE("Trie Copy Nodes") {

  Trie sourceTrie;

  sourceTrie.insert("apple");
  sourceTrie.insert("app");
  sourceTrie.insert("bat");
  sourceTrie.insert("ball");

  Trie destinationTrie;
  destinationTrie.insert("temp");

  SECTION("Copy nodes from source to destination") {
    Trie::copyNodes(destinationTrie.getRoot(), sourceTrie.getRoot());

    // Check that the words have been copied correctly to the destination
    REQUIRE(destinationTrie.getRoot()->children.find('a') != destinationTrie.getRoot()->children.end());
    REQUIRE(destinationTrie.getRoot()->children['a']->children.find('p') !=
            destinationTrie.getRoot()->children['a']->children.end());
    REQUIRE(destinationTrie.getRoot()->children['a']->children['p']->children.find('p') !=
            destinationTrie.getRoot()->children['a']->children['p']->children.end());
    REQUIRE(destinationTrie.getRoot()->children['a']->children['p']->children['p']->children.find('l') !=
            destinationTrie.getRoot()->children['a']->children['p']->children['p']->children.end());

    // Check 'end_of_word' for one word
    REQUIRE(destinationTrie.getRoot()->children['a']->children['p']->children['p']->end_of_word == true);  // For "app"
    REQUIRE(destinationTrie.getRoot()->children['b']->children['a']->children['t']->end_of_word == true);  // For "bat"
    REQUIRE(destinationTrie.getRoot()->children['b']->children['a']->children['l']->children['l']->end_of_word ==
            true);  // For "ball"
  }

  SECTION("Ensure copied trie has same structure") {
    // Copy the nodes from source to destination
    Trie::copyNodes(destinationTrie.getRoot(), sourceTrie.getRoot());

    // Check the structure and ensure it's identical
    REQUIRE(destinationTrie.getRoot()->children.size() == sourceTrie.getRoot()->children.size());

    // Check children of specific nodes
    auto& rootChildren = destinationTrie.getRoot()->children;
    auto& sourceRootChildren = sourceTrie.getRoot()->children;

    // Check 'a' node structure
    REQUIRE(rootChildren['a']->children.size() == sourceRootChildren['a']->children.size());
    REQUIRE(rootChildren['b']->children.size() == sourceRootChildren['b']->children.size());
  }

  SECTION("Copying an empty trie") {
    Trie emptySourceTrie;
    Trie emptyDestinationTrie;

    Trie::copyNodes(emptyDestinationTrie.getRoot(), emptySourceTrie.getRoot());

    // Ensure the destination trie is still empty
    REQUIRE(emptyDestinationTrie.getRoot()->children.empty());
  }
}
