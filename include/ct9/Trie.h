#include <memory>
#include <string>
#include <vector>
#include <queue>
class Trie final {

private:
  struct Node final {
    char c;
    std::vector<std::unique_ptr<Node>> vec;
    const std::queue<std::string>& return_all_words() const;
  } root;

public:
  Trie() = default;
  Trie(const Trie& trie);
  Trie(const std::string& str);
  Trie(const std::vector<std::string>& vec_str);
  Trie(Trie&& trie);
  Trie operator+(const Trie& trie) const;
  Trie operator-(const Trie& trie) const;
  Trie& operator=(const Trie& trie);
  bool operator< (const Trie& trie);
  bool operator== (const Trie& trie);
  bool operator> (const Trie& trie);
  bool operator!= (const Trie& trie);
  bool operator>= (const Trie& trie);
  bool operator<= (const Trie& trie);
  void insert(const std::string& text);
  void del(const std::string& text);
  const std::queue<std::string>& find(char ch) const;
  // TO DO operator <=>
};
