#pragma once
#include <climits>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#ifdef UNIT_TEST
  #define PRIVATE public
#else
  #define PRIVATE private
#endif

class Trie final {
  struct Node final {
    Node();
    ~Node();
    Node(const Node&);
    void clearNode();
    [[nodiscard]] std::queue<std::string> autocompleteNode(const std::string& prefix, size_t count) const;
    void insert(const std::string& text, size_t index, Node* root);
    [[nodiscard]] std::queue<std::string> find(char character) const;
    [[nodiscard]] std::string find() const;
    std::map<char, Node*> children;
    bool end_of_word{false};
  };

  PRIVATE:
    static void copyNodes(Node* dstRoot, const Node* srcRoot);
    Node* root{nullptr};

 public:
  /// Functions for testing
  const Node* getRoot() const { return root; }
  Node* getRoot() { return root; }
  void setRoot(Node* root) { this->root = root; }
  ///

  Trie();
  ~Trie();
  Trie(const std::string& str);
  Trie(const std::vector<std::string>& vec_str);

  Trie(const Trie& trie);
  Trie& operator=(const Trie& trie);
  Trie(Trie&& trie) noexcept;

  Trie operator+(const Trie& trie) const;
  Trie operator-(const Trie& trie) const;

  Trie& operator=(Trie&& trie) noexcept;
  void del(const std::string& text) const;
  std::string DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, char letter) const;
  [[nodiscard]] std::queue<std::string> autocomplete(const std::string& prefix, size_t count = INT_MAX) const;

  void insert(const std::string& text);
};

/**
 * @brief Trie copy constructor.
 * @param trie source of copy.
 */
inline Trie::Trie(const Trie& trie) : root(new Node()) {
  copyNodes(root, trie.root);
}

/**
 * @brief Assignment operator.
 * @param trie source.
 * @return New Trie.
 */
inline Trie& Trie::operator=(const Trie& trie) {
  if (this != &trie) {
    copyNodes(root, trie.root);
  }
  return *this;
}

/**
 * Move constructor.
 * @param trie source of move.
 */
inline Trie::Trie(Trie&& trie) noexcept {
  root = trie.root;
  trie.root = new Node();
}

/**
 * @brief Basic Node constructor.
 */
inline Trie::Node::Node() = default;

/**
 * @brief Node destructor.
 * Deletes all nodes.
 * @warning Add a pointer check.
 */
inline Trie::Node::~Node() {
  for (const auto & [key, value] : children) {
    delete value;
  }
}

/**
 * @brief Basic Trie constructor.
 * Creating new root node.
 */
inline Trie::Trie() : root(new Node()) {}

/**
 * @brief Basic Trie destructor.
 * Deletes root node.
 */
inline Trie::~Trie() {
  delete root;
}

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
inline void Trie::copyNodes(Node* dstRoot, const Node* srcRoot) {
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

/**
 * @brief Deletes a word from the Trie.
 *
 * This function removes a word from the Trie while ensuring that:
 * - It only deletes nodes that are no longer part of another word.
 * - It does not mistakenly remove prefixes that are still valid words.
 *
 * @param word The word to be removed from the Trie.
 */
inline void Trie::del(const std::string& word) const {
  bool is_word = this->autocomplete(word, 1).size() == 1;
  if (!is_word) {return;}
  Node* tmp = root;
  std::vector<Node*> path;
  for (char c : word) {
    path.push_back(tmp);
    if (tmp->children.find(c) == tmp->children.end()) {return;}
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
inline void Trie::Node::clearNode() {
  for (auto& child : children) {
    delete child.second;
  }
  children.clear();
}

inline Trie::Trie(const std::string& str) : root(new Node()){
  this->insert(str);
}

inline Trie::Trie(const std::vector<std::string>& vec_str) : root(new Node()) {
  for (const std::string& word : vec_str) {
    this->insert(word);
  }
}

inline Trie& Trie::operator=(Trie&& trie) noexcept {
  if(this != &trie) {
    delete root;
    root = trie.getRoot();
    trie.setRoot(nullptr);
  }
  return *this;
}

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
 * @brief Retrieves autocomplete suggestions starting with a specified prefix.
 *
 * This function performs a depth-first search (DFS) from the current trie node to collect words
 * that begin with the given prefix. A recursive lambda function is used for traversal. The
 * `current` string serves as a mutable buffer that holds the current path in the trie. When a
 * node marking the end of a valid word is encountered, the accumulated string in `current` is
 * added to the `results` queue. The DFS terminates early once the number of collected suggestions
 * reaches the specified limit.
 *
 * @param prefix The prefix string used as the starting point for generating autocomplete suggestions.
 * @param count The maximum number of autocomplete suggestions to retrieve.
 * @return std::queue<std::string> A queue containing the autocomplete suggestions in DFS traversal order.
 *
 * @note The implementation uses backtracking to avoid unnecessary string copying. The recursive DFS
 *       function returns a boolean value to indicate whether the search should be terminated early.
 */
[[nodiscard]] inline std::queue<std::string> Trie::Node::autocompleteNode(const std::string& prefix, const size_t count) const {
  std::queue<std::string> results;
  std::string current = prefix;

  std::function<bool(const Node*)> dfs = [&](const Node* node) -> bool {
    if (results.size() >= count) {return true;}

    if (node->end_of_word) {
      results.push(current);
      if (results.size() >= count) {return true;}
    }

    for (const auto& [key, child] : node->children) {
      current.push_back(key);
      if (dfs(child)) {return true;}
      current.pop_back();
    }
    return false;
  };

  static_cast<void>(dfs(this));
  return results;
}

/**
 * @brief Inserts a word into the trie.
 *
 * This function inserts the provided string into the trie, character by character.
 * It uses the root node to start the insertion process.
 *
 * @param text The word to be inserted into the trie.
 */
inline void Trie::insert(const std::string& text) {
  root->insert(text, 0, root);
}

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

