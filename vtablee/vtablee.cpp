#include <iostream>


class Animal {
public:
    virtual void speak() {
        std::cout << "Animal speaks" << std::endl;
    }
    virtual void eat() {
        std::cout << "Animal eats" << std::endl;
    }
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void speak() override {
        std::cout << "Dog barks" << std::endl;
    }
    // 不重写eat()函数
};

class Cat : public Animal {
public:
    void speak() override {
        std::cout << "Cat meows" << std::endl;
    }
    void eat() override {
        std::cout << "Cat eats fish" << std::endl;
    }
};


int main()
{
    //  多态：父类的接口指针，指向了子类实现
    Animal* animal1 = new Dog();
    Animal* animal2 = new Cat();

    animal1->speak();  // 输出: Dog barks
    animal2->speak();  // 输出: Cat meows
}


