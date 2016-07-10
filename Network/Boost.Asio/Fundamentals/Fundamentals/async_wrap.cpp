#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

io_service service;

void dispatched_func_1() {
    std::cout << "dispatched 1" << std::endl;
}
void dispatched_func_2() {
    std::cout << "dispatched 2" << std::endl;
}
void test(boost::function<void()> func) {
    std::cout << "test" << std::endl;
    service.dispatch(dispatched_func_1);
    func();
}
void service_run() {
    service.run();
}
int main(int argc, char* argv[]) {
    test( service.wrap(dispatched_func_2));
    boost::thread th(service_run);
    boost::this_thread::sleep( boost::posix_time::millisec(500));
    th.join();
}
