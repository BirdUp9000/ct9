#pragma once
#include <limits.h>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
class Trie final {

  struct Node final {
    Node() = default;
    ~Node() {
      for (auto& child : children) {
        delete child.second;
      }
    }
    /**
     * @brief Recursively clears all child nodes of this node.
     *
     * This method deletes all child nodes of the current node and clears the
     * children map. It effectively resets the subtree rooted at this node.
     *
     * @note This does not delete the current node itself, only its children.
     *       The node remains valid for further insertions or operations.
     *
     * @warning Ensure that no other references to the deleted child nodes exist,
     *          as this will lead to dangling pointers.
     *
     * @example
     * Node* root = new Node();
     * root->insert("apple", 0, root);
     * root->insert("banana", 0, root);
     * root->clear();  // The subtree is now empty, but root is still valid.
     */
    void clearNode() {
      for (auto& child : children) {
        delete child.second;
      }
      children.clear();
    }
    [[nodiscard]] std::queue<std::string> autocompleteNode(const std::string& prefix, size_t count) const;

    void insert(const std::string& text, size_t index, Node* root);

    [[nodiscard]] std::queue<std::string> find(char character) const;
    [[nodiscard]] std::string find() const;

    std::map<char, Node*> children;
    bool end_of_word = false;
  };

  Node* root = new Node();

public:
  const Node* getRoot() const { return root; }
  Node* getRoot() { return root; }
  void setRoot(Node* root) { this->root = root; }
  Trie() = default;
  ~Trie() { delete root; }
  Trie(const Trie& trie);
  explicit Trie(const std::string& str);
  explicit Trie(const std::vector<std::string>& vec_str);
  Trie(Trie&& trie) noexcept;
  Trie operator+(const Trie& trie) const;
  Trie operator-(const Trie& trie) const;
  Trie& operator=(const Trie& trie);
  Trie& operator=(Trie&& trie) noexcept;
  [[nodiscard]] bool operator<(const Trie& trie);
  [[nodiscard]] bool operator==(const Trie& trie);
  [[nodiscard]] bool operator>(const Trie& trie);
  [[nodiscard]] bool operator!=(const Trie& trie);
  [[nodiscard]] bool operator>=(const Trie& trie);
  [[nodiscard]] bool operator<=(const Trie& trie);
  void del(const std::string& word);
  std::string DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, char letter) const;
  [[nodiscard]] std::queue<std::string> autocomplete(const std::string& prefix, size_t count = INT_MAX) const;

  void insert(const std::string& text);
  /**
   * @brief Copies all nodes from a source trie to a destination trie.
   *
   * This function performs a breadth-first traversal (BFS) of the source trie
   * and duplicates its structure and data into the destination trie.
   * It ensures that each node and its respective children are copied correctly.
   *
   * @param dstRoot Pointer to the root node of the destination trie, where the copied structure will be stored.
   * @param srcRoot Pointer to the root node of the source trie, from which the structure and data will be copied.
   *
   * @note The destination trie should be initialized before calling this function.
   *       Existing data in the destination trie may be overwritten.
   *
   * @warning This function dynamically allocates memory for each new node in the destination trie.
   *          Ensure to properly manage memory to avoid leaks.
   */
  static void copyNodes(Node* dstRoot, const Node* srcRoot) {
    dstRoot->clearNode();
    std::queue<const Node*> srcQueue;
    std::queue<Node*> dstQueue;

    srcQueue.push(srcRoot);
    dstQueue.push(dstRoot);

    while (!srcQueue.empty()) {
      auto currentSrcNode = srcQueue.front();
      srcQueue.pop();

      auto currentDstNode = dstQueue.front();
      dstQueue.pop();

      for (auto& child : currentSrcNode->children) {
        srcQueue.push(child.second);

        char childChar = child.first;
        Node* newDstNode = new Node;
        newDstNode->end_of_word = child.second->end_of_word;

        currentDstNode->children[childChar] = newDstNode;
        dstQueue.push(newDstNode);
      }
    }
  }
};
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Deletes a word from the Trie.
 * 
 * This function removes a word from the Trie while ensuring that:
 * - It only deletes nodes that are no longer part of another word.
 * - It does not mistakenly remove prefixes that are still valid words.
 * 
 * @param word The word to be removed from the Trie.
 */
void Trie::del(const std::string& word) {
  bool is_word = this->autocomplete(word, 1).size() == 1;
  if (!is_word) return;
  Node* tmp = root;
  std::vector<Node*> path;
  for (char c : word) {
    path.push_back(tmp);
    if (tmp->children.find(c) == tmp->children.end()) return;
    tmp = tmp->children[c];
  }

  tmp->end_of_word = false;

  for (int i = path.size() - 1; i >= 0; --i) {
    Node* parent = path[i];
    char c = word[i];

    if (parent->children[c]->children.empty() && !parent->children[c]->end_of_word) {
      delete parent->children[c];
      parent->children.erase(c);
    } else {
      break;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////

Trie::Trie(const Trie& trie) {
  const Node* new_root = trie.getRoot();
  Trie::copyNodes(root, new_root);
}

Trie::Trie(const std::string& str) { this->insert(str); }
Trie::Trie(const std::vector<std::string>& vec_str) {
  for (std::string word : vec_str) {
    this->insert(word);
  }
}
Trie::Trie(Trie&& trie) noexcept {
  this->root = trie.getRoot();
  trie.setRoot(nullptr);
}

Trie& Trie::operator=(Trie&& trie) noexcept {
  if (this != &trie) {
    delete root;
    root = trie.getRoot();
    trie.setRoot(nullptr);
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Provides autocomplete suggestions based on the given prefix.
 *
 * This function searches the trie for the provided prefix and returns up to
 * `count` autocomplete suggestions. If the prefix is not found, it returns an empty queue.
 *
 * @param prefix The prefix string to search for in the trie.
 * @param count The maximum number of autocomplete suggestions to return.
 * @return A queue of strings containing the autocomplete suggestions.
 *         The results are in lexicographical order.
 */
[[nodiscard]] std::queue<std::string> Trie::autocomplete(const std::string& prefix, size_t count) const {
  Node* tmp_node = root;
  for (const auto& character : prefix) {
    if (!tmp_node->children.contains(character)) {
      return {};
    }
    tmp_node = tmp_node->children[character];
  }
  return tmp_node->autocompleteNode(prefix, count);
}

/**
 * @brief Helper function to gather autocomplete suggestions from a specific node.
 *
 * Performs a depth-first search (DFS) starting from the current node to find all
 * possible word completions that begin with the given prefix. The search continues
 * until the specified count of results is collected.
 *
 * @param prefix The prefix to append to each found word.
 * @param count The maximum number of results to return.
 * @return A queue of strings containing the found words, ordered lexicographically.
 */
[[nodiscard]] std::queue<std::string> Trie::Node::autocompleteNode(const std::string& prefix, size_t count) const {
  std::stack<std::pair<const Node*, std::string>> stack;
  std::queue<std::string> results;
  stack.push({this, prefix});

  while (!stack.empty() && results.size() < count) {
    auto [current_node, word] = stack.top();
    stack.pop();

    if (current_node->end_of_word) {
      results.push(word);
    }

    for (auto it = current_node->children.rbegin(); it != current_node->children.rend(); ++it) {
      stack.push({it->second, word + it->first});
    }
  }

  return results;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Inserts a word into the trie.
 *
 * This function inserts the provided string into the trie, character by character.
 * It uses the root node to start the insertion process.
 *
 * @param text The word to be inserted into the trie.
 */
inline void Trie::insert(const std::string& text) { root->insert(text, 0, root); }

/**
 * @brief Recursively inserts characters of the string into the trie.
 *
 * Inserts characters starting from the specified index. Marks the node as
 * the end of a word if the string ends or encounters a non-alphabetic character.
 *
 * @param text The word being inserted.
 * @param index The current index of the character being processed in the word.
 * @param root Pointer to the root node of the trie for recursive insertion.
 */
inline void Trie::Node::insert(const std::string& text, const size_t index, Node* root) {
  if (index == text.size()) {
    end_of_word = true;
    return;
  }

  // Mark as end of word if a non-alphabetic character is encountered
  if (isalpha(text[index]) == 0) {
    end_of_word = true;
    root->insert(text, index + 1, root);
  } else {
    // Create a new node if the current character is not found
    if (!children.contains(text[index])) {
      children[text[index]] = new Node();
    }
    children[text[index]]->insert(text, index + 1, root);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

// TODO: constexpr set with supportable chars
/*
inline std::string Trie::DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, const char letter)
const { if (!node) return "";

  std::ostringstream svg;

  int box_width = 20;
  int box_height = 20;
  int vertical_spacing = 50;
  int horizontal_spacing = 10 * std::max(1, static_cast<int>(node->children.size()));

  if (level > 0) {
    svg << "<line x1=\"" << parent_x + box_width / 2 << "\" y1=\"" << parent_y + box_height
        << "\" x2=\"" << x + box_width / 2 << "\" y2=\"" << y
        << "\" stroke=\"black\" stroke-width=\"2\" />\n";
  }

  svg << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << box_width
      << "\" height=\"" << box_height << "\" rx=\"10\" ry=\"10\" fill=\"blue\" />\n";

  svg << "<text x=\"" << (x + box_width / 2) << "\" y=\"" << (y + box_height / 2)
      << "\" font-size=\"20\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"middle\">"
      << letter << "</text>\n";

  int num_children = node->children.size();
  int start_x = x - (num_children - 1) * horizontal_spacing / 2;
  int child_x = start_x;

  for (const auto& [key, child] : node->children) {
    svg << DEBUG(child.get(), child_x, y + vertical_spacing, level + 1, x, y, key);
    child_x += horizontal_spacing;
  }

  return svg.str();
}
*/