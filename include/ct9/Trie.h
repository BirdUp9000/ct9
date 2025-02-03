#pragma once
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


class Trie final {

  struct Node final{
    Node() = default;
    ~Node() {
      for (auto &child : children) {
        delete child.second;
      }
    }

    [[nodiscard]] std::queue<std::string> autocompleteNode(const std::string& prefix) const;
    template <std::unsigned_integral COUNT>
    [[nodiscard]] std::queue<std::string> autocompleteNode(const std::string& prefix, COUNT count) const;

    void insert(const std::string& text, size_t index, Node* root);

    [[nodiscard]] std::queue<std::string> find(char character) const;
    [[nodiscard]] std::string find() const;

    std::map<char, Node*> children;
    bool end_of_word = false;
  };

  Node* root = new Node();

public:
  const Node* getRoot() {
    return root;
  }
  Trie() = default;
  ~Trie() {
    delete root;
  }

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

  [[nodiscard]] std::queue<std::string> autocomplete(const std::string &prefix) const;
  template <std::integral COUNT>
  [[nodiscard]] std::queue<std::string> autocomplete(const std::string &prefix, COUNT count) const;

  void insert(const std::string& text);
};

/**
 * @brief Retrieves all autocomplete suggestions that start with the specified prefix.
 *
 * This function performs a depth-first search (DFS) starting from the current node to collect
 * all words that begin with the provided prefix. Whenever a node marked as the end of a word
 * is encountered, the accumulated text is added to the result queue.
 *
 * @param prefix The prefix string used as the starting point for generating autocomplete suggestions.
 * @return std::queue<std::string> A queue containing all the autocomplete suggestions found.
 *
 * @note The suggestions are collected in the order determined by the DFS traversal.
 *       A recursive lambda function is used for the DFS implementation.
 */
[[nodiscard]] inline std::queue<std::string> Trie::Node::autocompleteNode(const std::string& prefix) const {
  std::queue<std::string> results;
  std::function<void(const Node*, std::string)> dfs = [&](const Node* node, const std::string& current) {
    if (node->end_of_word) { results.push(current); };
    for (const auto& [key, child] : node->children) {
      dfs(child, current + key);
    }
  };
  dfs(this, prefix);
  return results;
}

/**
 * @brief Returns up to a given number of autocomplete suggestions starting with the given prefix.
 *
 * This version limits the number of suggestions to the value specified by `count`.
 * @attention This function has unsigned_integral template because this function
 * will be called only from autocomplete function (Trie class) who checks count to be positive.
 *
 * @tparam COUNT An unsigned integral type representing the maximum number of suggestions.
 * @param prefix The starting prefix.
 * @param count The maximum number of suggestions to collect.
 * @return std::queue<std::string> A queue containing up to `count` suggestions.
 */
template <std::unsigned_integral COUNT>
[[nodiscard]] std::queue<std::string> Trie::Node::autocompleteNode(const std::string& prefix, const COUNT count) const {
  std::queue<std::string> results;

  // TODO: COUNT not working properly
  std::function<void(const Node*, std::string)> dfs = [&](const Node* node, const std::string& current) {
    if (node->end_of_word) { results.push(current);}
    if (results.size() == count) { return; }
    for (const auto& [key, child] : node->children) {
      if (results.size() == count) { break; }
      dfs(child, current + key);
    }
  };
  dfs(this, prefix);
  return results;
}

/**
 * @brief Retrieves all autocomplete suggestions for the given prefix.
 *
 * Finds the node corresponding to the prefix and returns all suggestions.
 *
 * @param prefix The prefix string.
 * @return std::queue<std::string> A queue containing all suggestions.
 */
[[nodiscard]] inline std::queue<std::string> Trie::autocomplete(const std::string &prefix) const {
  Node* tmp_node = root;
  for (const auto &character : prefix) {
    if (!tmp_node->children.contains(character)) {
      return {};
    }
    tmp_node = tmp_node->children[character];
  }
  return tmp_node->autocompleteNode(prefix);
}

/**
 * @brief Retrieves up to a specified number of autocomplete suggestions for the given prefix.
 *
 * This function locates the node corresponding to the provided prefix and then returns at most
 * `count` suggestions gathered from that node using a depth-first search (DFS) traversal of the Trie.
 * If a negative value is provided for `count`, the function throws a `std::invalid_argument` exception.
 *
 * @tparam COUNT An integral type representing the maximum number of suggestions. Negative values are not allowed.
 * @param prefix The prefix string used to search within the Trie.
 * @param count The maximum number of suggestions to return. Must be non-negative.
 * @return std::queue<std::string> A queue containing up to `count` autocomplete suggestions.
 * @throws std::invalid_argument if `count` is negative.
 *
 * @note The expression:
 *       @code
 *       static_cast<std::make_unsigned_t<COUNT>>(count)
 *       @endcode
 *       converts the `count` parameter to its corresponding unsigned type. This ensures that if
 *       `COUNT` is a signed integral type (e.g., int), it is converted to the appropriate unsigned
 *       type (e.g., unsigned int) before being passed to the underlying `autocompleteNode` function.
 *       The conversion is safe because the function throws an exception if `count` is negative.
 */
template <std::integral COUNT>
[[nodiscard]] std::queue<std::string> Trie::autocomplete(const std::string &prefix, const COUNT count) const {
  if (count < 0) { throw std::invalid_argument("Trie::autocomplete(): negative count"); }
  Node* tmp_node = root;
  for (const auto &character : prefix) {
    if (!tmp_node->children.contains(character)) {
      return {};
    }
    tmp_node = tmp_node->children[character];
  }
  return tmp_node->autocompleteNode(prefix, static_cast<std::make_unsigned_t<COUNT>>(count));
}

inline void Trie::insert(const std::string& text){
  root->insert(text, 0, root);
}

inline void Trie::Node::insert(const std::string& text, const size_t index, Node* root) {
  if (index == text.size()) {
    end_of_word = true;
    return;
  }

  // If our char is hot an alpha char the word has ended
  if (isalpha(text[index]) == 0) { // TODO: Should we add end_of_word = true if we have last char in string?
    end_of_word = true;
    root->insert(text, index + 1, root);
  } else {
    if (!children.contains(text[index])) {
      children[text[index]] = new Node();
    }
    children[text[index]]->insert(text, index + 1, root);
  }
}
// TODO: constexpr set with supportable chars

// possible errors with non ASCII chars
// inline std::string to_lowercase(std::string& word) {
//   for (char& c : word) {
//     c = std::tolower(c);
//   }
//   return word;
// }
//
// inline std::vector<std::string> text_to_words(const std::string& text) {
//   std::vector<char> delimiters = {' ', '.', '!', '?', '-', ':', ';',','};
//   std::vector<std::string> words;
//   bool is_word = false;
//   auto start = text.begin();
//   for (auto end = text.begin(); end != text.end(); ++end) {
//     if (std::find(delimiters.begin(), delimiters.end(), *end) == delimiters.end() && !is_word) {
//       start = end;
//       is_word = true;
//     } else if (std::find(delimiters.begin(), delimiters.end(), *end) != delimiters.end() && is_word) {
//       std::string word(start, end);
//       words.push_back(to_lowercase(word));
//       is_word = false;
//     }
//   }
//   if (is_word) {
//     std::string word(start, text.end());
//     words.push_back(to_lowercase(word));
//   }
//   return words;
// }

/*
inline void Trie::insert(const std::string& text) {
  std::vector<std::string> all_words = text_to_words(text);
  for (const std::string& word : all_words) {
    Node* tmp = &root;
    for (char c : word) {
      if (tmp->children.find(c) == tmp->children.end()) {
        tmp->children[c] = std::make_unique<Node>();
        tmp->children[c]->c = c;
      }
      tmp = tmp->children[c].get();
    }
    tmp->end_of_word = true;
  }
}
*/


/*
inline std::string Trie::DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, const char letter) const {
  if (!node) return "";

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