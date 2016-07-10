#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

io_service service;

void func(int i) {
    std::cout << "func called, i= " << i << std::endl;
}

void run_dispatch_and_post() {
    for ( int i = 0; i < 10; i += 2) {
        service.dispatch(boost::bind(func, i));
        service.post(boost::bind(func, i + 1));
    }
}

int main(int argc, char* argv[]) {
    service.post(run_dispatch_and_post);
    service.run();
}
