#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

io_service service;

struct connection : boost::enable_shared_from_this<connection> {
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<connection> ptr;
    connection() : sock_(service), started_(true) {}
    void start(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, boost::bind(&connection::on_connect, shared_from_this(),_1));
    }
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
private:
    void on_connect(const error_code & err) {
        std::cout << "calling on_connect: " << std::endl;
    // here you decide what to do with the connection: read or write
        if ( !err)
            do_read();
        else
            stop();
    }
    void on_read(const error_code & err, size_t bytes) {
        std::cout << "calling on_read: " << std::endl;
        if ( !started() ) return;
        std::string msg(read_buffer_, bytes);
        if ( msg == "can_login")
            do_write("access_data");
        else if ( msg.find("data ") == 0)
            process_data(msg);
        else if ( msg == "login_fail")
            stop();
    }
    void on_write(const error_code & err, size_t bytes) {
        std::cout << "on_write finished, calling do_read: " << std::endl;
        do_read();
    }
    void do_read() {
        sock_.async_read_some(buffer(read_buffer_),
              boost::bind(&connection::on_read, shared_from_this(),_1, _2));
    }
    void do_write(const std::string & msg) {
        if ( !started() ) return;
        // note: in case you want to send several messages before
        // doing another async_read, you'll need several write buffers!
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
              boost::bind(&connection::on_write, shared_from_this(), _1, _2));
    }
    void process_data(const std::string & msg) {
        // process what comes from server, and then perform another write
        std::cout << "process_data: " << msg << std::endl;
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

int main(int argc, char* argv[]) {
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 21234);
    connection::ptr(new connection)->start(ep);
    while(1) {sleep(1);}
}
