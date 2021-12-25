/*
  Copyright

  Open-Closed Principle Example

  g++ filter.cpp -o -std=c++17 filter
*/

#include <iostream>
#include <string>
#include <vector>

enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

struct Product {
  std::string name_;
  Color color_;
  Size size_;

  Product(std::string name, Color color, Size size) : name_(name), color_(color), size_(size) {}
};

struct ProductFilter {
  using Items = std::vector<Product*>;

  Items byColor(Items items, Color color);
  Items bySize(Items items, Size size);
  Items byColorAndSize(Items items, Color color, Size size);
};


ProductFilter::Items ProductFilter::byColor(Items items, Color color) {
  Items result;
  for (auto& i : items) {
    if (i->color_ == color) {
      result.push_back(i);
    }
  }
  return result;
}

ProductFilter::Items ProductFilter::bySize(Items items, Size size) {
  Items result;
  for (auto& i : items)
    if (i->size_ == size)
      result.push_back(i);
  return result;
}

ProductFilter::Items ProductFilter::byColorAndSize(Items items, Color color, Size size) {
  Items result;
  for (auto& i : items)
    if (i->size_ == size && i->color_ == color)
      result.push_back(i);
  return result;
}

/*
  Open-Closed Principle
*/

template <typename T>
struct Specification {
  virtual bool is_satisfied(T* item) = 0;
};

template <typename T> struct Filter {
  virtual std::vector<T*> filter(std::vector<T*> items,
                                 Specification<T>& spec) = 0;  // NOLINT non-const reference
};

struct BetterProductFilter : Filter<Product> {
  std::vector<Product*> filter(std::vector<Product*> items,
                               Specification<Product>& spec) override {
    std::vector<Product*> result;
    for (auto& p : items)
      if (spec.is_satisfied(p))

  result.push_back(p);
  return result;
  }
};

struct ColorSpecification : Specification<Product> {
  Color color;

  explicit ColorSpecification(const Color color) : color{color} {}

  bool is_satisfied(Product* item) override {
    return item->color_ == color;
  }
};

struct SizeSpecification : Specification<Product> {
  Size size;

  explicit SizeSpecification(const Size size) : size{size} {}

  bool is_satisfied(Product* item) override {
    return item->size_ == size;
  }
};

template <typename T> struct AndSpecification : Specification<T> {
  Specification<T>& first;
  Specification<T>& second;

  AndSpecification(Specification<T>& first,  // NOLINT non-const reference
                   Specification<T>& second)  // NOLINT non-const reference
                   : first{first}, second{second} {}

  bool is_satisfied(T* item) override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  }
};

void display_items(const ProductFilter::Items & items_filtered) {
  for (const auto & item : items_filtered) {
    std::cout << item->name_ << std::endl;
  }
}


int main() {
  Product p1("small_sofa", Color::Green, Size::Small);
  Product p2("big_sofa", Color::Red, Size::Large);
  Product p3("green_chair", Color::Green, Size::Medium);
  Product p4("red_chair", Color::Red, Size::Medium);
  Product p5("desk", Color::Blue, Size::Medium);
  Product p6("big_desk", Color::Green, Size::Large);

  ProductFilter::Items products = {&p1, &p2, &p3, &p4, &p5, &p6};

  ProductFilter product_filter;
  ProductFilter::Items items_filtered = product_filter.byColor(products, Color::Green);
  display_items(items_filtered);

  // Second and Best approach
  BetterProductFilter better_product_filter;
  ColorSpecification green(Color::Green);
  auto green_things = better_product_filter.filter(products, green);
  display_items(green_things);


  Product apple{ "Apple", Color::Green, Size::Small };
  Product tree{ "Tree", Color::Green, Size::Large };
  Product house{ "House", Color::Blue, Size::Large };

  std::vector<Product*> all{ &apple, &tree, &house };

  BetterProductFilter bf;

  SizeSpecification large(Size::Large);
  AndSpecification<Product> green_and_large{ large, green };

  auto big_green_things = bf.filter(all, green_and_large);

  // Just "Tree"
  display_items(big_green_things);
}
