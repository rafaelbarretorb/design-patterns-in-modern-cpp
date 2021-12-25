/*
  Copyright

  g++ Builder.cpp -std=c++17 -o Builder
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>


struct HtmlBuilder;

struct HtmlElement {
  std::string name;
  std::string text;
  std::vector<HtmlElement> elements;
  const size_t indent_size = 2;

  HtmlElement() {}
  HtmlElement(const std::string &name, const std::string &text)
    : name(name),
    text(text) {}

  std::string str(int indent = 0) const {
    std::ostringstream oss;
    std::string i(indent_size*indent, ' ');
    oss << i << "<" << name << ">" << std::endl;
    if (text.size() > 0)
      oss << std::string(indent_size*(indent + 1), ' ') << text << std::endl;

    for (const auto& e : elements)
      oss << e.str(indent + 1);

    oss << i << "</" << name << ">" << std::endl;
    return oss.str();
  }

  static std::unique_ptr<HtmlBuilder> build(std::string root_name) {
    return std::make_unique<HtmlBuilder>(root_name);
  }
};

struct HtmlBuilder {
  explicit HtmlBuilder(std::string root_name) {
    root.name = root_name;
  }

  // Fluent Builder reference based
  HtmlBuilder& add_child(std::string child_name, std::string child_text) {
    HtmlElement e{ child_name, child_text };
    root.elements.emplace_back(e);
    return *this;
  }

  // Fluent Builder pointer based
  HtmlBuilder* add_child_2(std::string child_name, std::string child_text) {
    HtmlElement e{ child_name, child_text };
    root.elements.emplace_back(e);
    return this;
  }

  std::string str() { return root.str(); }

  operator HtmlElement() const { return root; }

  HtmlElement root;
};

int main() {
  // <p>hello</p>
  auto text = "hello";
  std::string output;
  output += "<p>";
  output += text;
  output += "</p>";
  printf("<p>%s</p>", text);

  // <ul><li>hello</li><li>world</li></ul>
  std::string words[] = { "hello", "world" };
  std::ostringstream oss;
  oss << "<ul>";
  for (auto w : words)
    oss << "  <li>" << w << "</li>";

  oss << "</ul>";
  printf("\n\n%s", oss.str().c_str());

  // Approach 1
  std::cout << "\n\nApproach 1:" << std::endl;
  HtmlBuilder builder{ "ul" };
  builder.add_child("li", "hello").add_child("li", "world");
  std::cout << builder.str() << std::endl;

  // Approach 2
  std::cout << "\nApproach 2:" << std::endl;
  auto builder2 = HtmlElement::build("ul");
  builder2->add_child_2("li", "hello")->add_child_2("li", "world");
  std::cout << builder2->str() << std::endl;


  // ERROR if I do this way
  // Segmentation fault (core dumped)
  // auto builder2 = HtmlElement::build("ul")->add_child_2("li", "hello")->add_child_2("li", "world");
  // std::cout << builder2->str() << std::endl;

  return 0;
}
