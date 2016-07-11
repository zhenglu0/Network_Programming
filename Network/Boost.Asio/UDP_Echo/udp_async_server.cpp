#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(),y,z)

typedef boost::system::error_code error_code;

io_service service;

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>
, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service, ip::udp::endpoint(ip::udp::v4(),8001)), started_(false) {}
public:
    typedef boost::shared_ptr<talk_to_client> ptr;
    void start() {
        started_ = true;
        do_read();
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
    ip::udp::socket & sock() { return sock_;}
    void do_read() {
        sock_.async_receive_from(buffer(read_buffer_), sender_ep, MEM_FN2(on_read,_1,_2));
    }
    void do_write(const std::string & msg) {
        if ( !started() ) return;
        std::cout << "do_write:" << msg;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_send_to(buffer(write_buffer_, msg.size()), sender_ep, MEM_FN2(on_write,_1,_2));
    }
    void on_read(const error_code & err, size_t bytes) {
        if ( !err) {
            std::string msg(read_buffer_, bytes);
            do_write(msg);
        }
    }
    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }
private:
    ip::udp::socket sock_;
    ip::udp::endpoint sender_ep;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    client->start();
    service.run();
}
