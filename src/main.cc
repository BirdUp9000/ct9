#include <fstream>
#include <iostream>

#include "../include/ct9/Trie.h"

int main() {

  std::cout << "Hello, World" << "!\n";


  Trie t;
  t.insert("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Nullam dapibus fermentum ipsum. Aliquam id dolor. Fusce tellus odio, dapibus id fermentum quis, suscipit id erat. Sed convallis magna eu sem. Pellentesque ipsum. Nulla non lectus sed nisl molestie malesuada. Nullam sapien sem, ornare ac, nonummy non, lobortis a enim. Quisque porta. Aliquam erat volutpat. Etiam quis quam. Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?");
  //t.insert("Lorem");
  //std::string svg_tree = "<svg width=\"8000\" height=\"6000\" xmlns=\"http://www.w3.org/2000/svg\">\n";

  //std::cout << "[DEBUG] Root node's childrens count: " << t.getRoot().children.size() << std::endl;
  //std::cout << "[DEBUG] Next to root node's childrens count: " << t.getRoot().children.find('a')->second.get()->children.size() << std::endl;

  //svg_tree += t.DEBUG(&t.getRoot(), 3500, 200, 0, 0, 0, ' ');
  //svg_tree += "</svg>";

  //std::ofstream file("tree.svg");
  //file << "<html><body>" << svg_tree << "</body></html>";
  //file.close();

  //std::cout << "[DEBUG] First word starting with 'v' : " << t.find('v').front() << std::endl;

  std::queue<std::string> A = t.autocomplete("");

  while (!A.empty()) {
    std::cout << A.front() << "\n";
    A.pop();
  }

  return 0;
}
