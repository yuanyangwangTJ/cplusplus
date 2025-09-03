#include <boost/fiber/all.hpp>
#include <iostream>

using namespace std;
using namespace boost;
 
void callMe(fibers::buffered_channel<string>& pipe) {
    pipe.push("hello world");
}
 
 
int main() {
    fibers::buffered_channel<string> pipe(2);
    fibers::fiber f([&]() {callMe(pipe); });
    f.detach();
    string str;
    std::cout<<"start pop"<<std::endl;
    pipe.pop(str); //切换协程运行
    std::cout<<"get str:"<<str<<std::endl;
    return 0;
}
