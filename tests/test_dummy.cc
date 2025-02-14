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
//   SECTION("Move Constructor") {
//     std::vector<std::string> words = {"apple", "bat"};
//     Trie originalTrie(words);  // Create trie from vector
//     Trie movedTrie(std::move(originalTrie));

//     // Check that the moved trie has the data
//     REQUIRE(
//         movedTrie.getRoot()->children['a']->children['p']->children['p']->children['l']->children['e']->end_of_word ==
//         true);
//     REQUIRE(movedTrie.getRoot()->children['b']->children['a']->children['t']->end_of_word == true);

//     //Ensure the original trie is empty
//     REQUIRE(originalTrie.getRoot() == nullptr);
//  }
}

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
