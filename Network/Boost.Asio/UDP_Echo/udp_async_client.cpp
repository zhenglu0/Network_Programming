#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(),y,z)

io_service service;

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
, boost::noncopyable {
    typedef talk_to_svr self_type;
    talk_to_svr(const std::string & message)
    : sock_(service, ip::udp::endpoint(ip::udp::v4(), 0)), started_(true), message_(message) {}
    void start(ip::udp::endpoint ep) {
        do_write(message_ + "\n", ep);
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_svr> ptr;
    static ptr start(ip::udp::endpoint ep, const std::string &message) {
        ptr new_(new talk_to_svr(message));
        new_->start(ep);
        return new_;
    }
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
    void do_read() {
        ip::udp::endpoint sender_ep;
        sock_.async_receive_from(buffer(read_buffer_), sender_ep, MEM_FN2(on_read,_1,_2));
    }
    void do_write(const std::string & msg, ip::udp::endpoint ep) {
        if ( !started() ) return;
        std::cout << "do_write:" << msg;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_send_to(buffer(write_buffer_, msg.size()), ep, MEM_FN2(on_write,_1,_2));
    }
    void on_read(const error_code & err, size_t bytes) {
        if ( !err) {
            std::string copy(read_buffer_, bytes - 1);
            std::cout << "server echoed our " << message_ << ": "
                      << (copy == message_ ? "OK" : "FAIL") << std::endl;
        }
        stop();
    }
    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }
private:
    ip::udp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string message_;
};

int main(int argc, char* argv[]) {
    ip::udp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
    const char* messages[] = { "John says hi", "so does James", "Lucy got home", 0 };
    for (const char ** message = messages; *message; ++message) {
        talk_to_svr::start(ep, *message);
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    service.run();
}
