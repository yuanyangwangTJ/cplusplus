#include <iostream>
template <typename Derived>
class Animal {
public:
    void speak() {
        static_cast<Derived*>(this)->makeSound();
    }
};

class Dog : public Animal<Dog> {
public:
    void makeSound() { std::cout << "Woof!" << std::endl; }
};

class Cat : public Animal<Cat> {
public:
    void makeSound() { std::cout << "Meow!" << std::endl; }
};

int main() {
    Dog d;
    Cat c;
    d.speak();  // Woof!
    c.speak();  // Meow!
    return 0;
}
