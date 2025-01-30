#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <ostream>
#include <sstream>

class Trie final {

private:
  struct Node final{
    Node() = default;
    std::map<char, std::unique_ptr<Node>> children;
    const std::queue<std::string>& return_all_words() const;

    void insert(const std::string& text, size_t index, Node & root);


    std::queue<std::string> find(char ch) const;
    std::string find() const;



    char c;

    bool end_of_word = false;
  } root;

public:
  Node& getRoot() {
    return root;
  }
  Trie() = default;
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
  void insert(const std::string& text);
  void del(const std::string& text);
  std::queue<std::string> find(char ch) const;
  // TO DO operator <=>

  std::string DEBUG(const Node* node, int x, int y, int level, int parent_x, int parent_y, const char letter) const;
};

inline std::queue<std::string> Trie::find(const char ch) const {
  return root.find(ch);
}

inline std::string Trie::Node::find() const {
  std::string result;
  if (children.begin() == children.end()) {return result;}
  result += children.begin()->first;
  result += children.begin()->second->find();
  return result;
}

inline std::queue<std::string> Trie::Node::find(char ch) const {
  /////// TEST FUNCTION ///////
  std::queue<std::string> result;
  std::string tmp_string;
  tmp_string += ch;

  if (children.contains(ch)) {
    tmp_string += children.at(ch).get()->find();
  }

  result.push(tmp_string);
  return result;
}



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

/////////////////////////////////////////////////////////////////////////////////

inline void Trie::insert(const std::string& text) {
  root.insert(text, 0, root);
}

inline void Trie::Node::insert(const std::string& text, const size_t index, Node & root) {
  if (index == text.size()) {
    end_of_word = true;
    return;
  }

  const char c = text[index];
  if (!isalpha(c)) {
    end_of_word = true;
    root.insert(text, index + 1, root);
  } else {
    if (!children.contains(c)) {
      children[c] = std::make_unique<Node>();
    }

    children[c]->insert(text, index + 1, root);
  }
}

/////////////////////////////////////////////////////////////////////////////////

// possible errors with non ASCII chars
inline std::string to_lowercase(std::string& word) {
  for (char& c : word) {
    c = std::tolower(c);
  }
  return word;
}

inline std::vector<std::string> text_to_words(const std::string& text) {
  std::vector<char> delimiters = {' ', '.', '!', '?', '-', ':', ';',','};
  std::vector<std::string> words;
  bool is_word = false;
  auto start = text.begin();
  for (auto end = text.begin(); end != text.end(); ++end) {
    if (std::find(delimiters.begin(), delimiters.end(), *end) == delimiters.end() && !is_word) {
      start = end;
      is_word = true;
    } else if (std::find(delimiters.begin(), delimiters.end(), *end) != delimiters.end() && is_word) {
      std::string word(start, end);
      words.push_back(to_lowercase(word));
      is_word = false;
    }
  }
  if (is_word) {
    std::string word(start, text.end());
    words.push_back(to_lowercase(word));
  }
  return words;
}

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
