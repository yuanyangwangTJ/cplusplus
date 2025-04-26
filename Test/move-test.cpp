#include <iostream>

using namespace std;

class A
{
public:
	A() {};
	A(const A&) { cout << "copy construct\n"; }
	A& operator=(const A&) {
		cout << "copy assignment\n";
	}
	A(A&&) noexcept { cout << "move construct\n"; }
	A& operator=(A&&) noexcept {
		cout << "move assignment\n";
	}

private:
};

A foo() {
	A a{};
	//auto&& b = std::move(a);
	return a;
}

int main()
{
	auto a(foo());
	//A c = a;

}
