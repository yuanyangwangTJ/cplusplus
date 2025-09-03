#include <iostream>

class Parent {
public:
    virtual void Intro() {
        std::cout << "Parent" << std::endl;
        Hobby();
    }
    virtual void Hobby() {
        std::cout << "Parent::Hobby" << std::endl;
    }
};

class Child : public Parent {
    virtual void Intro() {
        std::cout << "Child" << std::endl;
        Hobby();
    }
    virtual void Hobby() {
        std::cout << "Child::Hobby" << std::endl;
    }
};

int main() {
    Child* c = new Child();
    Parent* p = (Parent*)c;
    p->Intro();
    p->Parent::Intro();
    Parent pp = *c;
    pp.Intro();
    
    return 0;
}