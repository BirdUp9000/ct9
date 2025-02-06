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

    [[nodiscard]] std::queue<std::string> autocompleteNode(const std::string& prefix, size_t count) const;

    void insert(const std::string& text, size_t index, Node* root);

    [[nodiscard]] std::queue<std::string> find(char character) const;
    [[nodiscard]] std::string find() const;

    std::map<char, Node*> children;
    bool end_of_word = false;
  };

  Node* root = new Node();

public:
  const Node* getRoot() { return root; }
  Trie() = default;
  ~Trie() { delete root; }

  Trie(const Trie& trie);
  explicit Trie(const std::string& str);
  explicit Trie(const std::vector<std::string>& vec_str);
  Trie(Trie&& trie) noexcept;
  Trie operator+(const Trie& trie) const;
  Trie operator-(const Trie& trie) const;
  Trie& operator=(const Trie& trie);
  [[nodiscard]] bool operator<(const Trie& trie);
  [[nodiscard]] bool operator==(const Trie& trie);
  [[nodiscard]] bool operator>(const Trie& trie);
  [[nodiscard]] bool operator!=(const Trie& trie);
  [[nodiscard]] bool operator>=(const Trie& trie);
  [[nodiscard]] bool operator<=(const Trie& trie);
  void del(const std::string& text);
  std::string DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, char letter) const;
  [[nodiscard]] std::queue<std::string> autocomplete(const std::string& prefix, size_t count = INT_MAX) const;
  void insert(const std::string& text);
};

//////////////////////////////////////////////////////////////////////////////////////////

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

[[nodiscard]] std::queue<std::string> Trie::Node::autocompleteNode(const std::string& prefix, size_t count) const {
  std::stack<std::pair<const Node*, std::string>> stack;
  std::queue<std::string> results;
  stack.push({this, prefix});
  while (!stack.empty() && results.size() < count) {
    auto [current_children, word] = stack.top();
    stack.pop();
    if (current_children->end_of_word) {
      results.push(word);
    }
    for (auto it = current_children->children.rbegin(); it != current_children->children.rend(); ++it) {
      stack.push({it->second, word + it->first});
    }
  }
  return results;
}

//////////////////////////////////////////////////////////////////////////////////////////

inline void Trie::insert(const std::string& text) { root->insert(text, 0, root); }

inline void Trie::Node::insert(const std::string& text, const size_t index, Node* root) {
  if (index == text.size()) {
    end_of_word = true;
    return;
  }

  // If our char is hot an alpha char the word has ended
  if (isalpha(text[index]) == 0) {  // TODO: Should we add end_of_word = true if we have last char in string?
    end_of_word = true;
    root->insert(text, index + 1, root);
  } else {
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