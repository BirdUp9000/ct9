#include <algorithm>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
class Trie final {

private:
  struct Node final{
    Node() = default;
    std::map<char,std::unique_ptr<Node>> children;
    const std::queue<std::string>& return_all_words() const;
    void insert(const char c);

    bool end_of_word;
  } root;

public:
  Node& getRoot() {
    return root;
  }
  Trie() = default;
  Trie(const Trie& trie);
  Trie(const std::string& str);
  Trie(const std::vector<std::string>& vec_str);
  Trie(Trie&& trie);
  Trie operator+(const Trie& trie) const;
  Trie operator-(const Trie& trie) const;
  Trie& operator=(const Trie& trie);
  bool operator<(const Trie& trie);
  bool operator==(const Trie& trie);
  bool operator>(const Trie& trie);
  bool operator!=(const Trie& trie);
  bool operator>=(const Trie& trie);
  bool operator<=(const Trie& trie);
  void insert(const std::string& text);
  void del(const std::string& text);
  const std::queue<std::string>& find(char ch) const;
  // TO DO operator <=>
};

inline void Trie::Node::insert(const char _c) {
  if (children.contains(_c)) {
    children[_c].get()->insert(_c);
  } else {
    children[_c] = std::make_unique<Node>();
  }
}

inline void Trie::insert(const std::string& text) {
  for (auto c = text.begin(); c != text.end(); ++c) {
    if (std::isalpha(std::static_cast<unsigned char>(*c))) {
      root.insert(*c);
    }
  }
}


// possible errors with non ASCII chars
std::string to_lowercase(std::string& word) {
  for (char& c : word) {
    c = std::tolower(c);
  }
  return word;
}
std::vector<std::string> text_to_words(const std::string& text) {
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

// inline void Trie::insert(const std::string& text) {
//   std::vector<std::string> all_words = text_to_words(text);
//   for(std::string& word : all_words) {
//     Node* tmp = &root;
//     for(char c : word) {
//    if (tmp->children.find(c) == tmp->children.end()) {
//             tmp->children[c] = std::make_unique<Node>(); 
//             tmp->children[c]->c = c;
//         }
      
//     }
//   }
// }
// //    std::map<char,std::unique_ptr<Node>> children;
