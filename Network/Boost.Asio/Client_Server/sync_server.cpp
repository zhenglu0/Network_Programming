#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <vector>

using namespace boost::asio;

io_service service;

struct talk_to_client;
typedef boost::system::error_code error_code;
typedef boost::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
array clients;
boost::recursive_mutex cs; // thread-safe access to clients array

struct talk_to_client : boost::enable_shared_from_this<talk_to_client>
{
    talk_to_client() : sock_(service) {}
    std::string username() const { return username_; }
    void answer_to_client() {
        try {
            read_request();
            process_request();
        } catch ( boost::system::system_error&) {
            stop();
        }
        if ( timed_out()) {
            std::cout << "time out: " << username_ << std::endl;
            stop();
        }
    }
    void update_clients_changed() { clients_changed_ = true; }
    ip::tcp::socket & sock() { return sock_; }
    bool timed_out() const {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        long long ms = (now - last_ping).total_milliseconds();
        return ms > 5000 ;
    }
    void stop() {
        boost::system::error_code err;
        sock_.close(err);
    }
    void read_request() {
        if ( sock_.available())
            already_read_ += sock_.read_some(
                buffer(buff_ + already_read_, max_msg - already_read_));
    }
    void process_request() {
        bool found_enter = std::find(buff_, buff_ + already_read_, '\n')
                            < buff_ + already_read_;
        if ( !found_enter)
            return; // message is not full
        // process the msg
        last_ping = boost::posix_time::microsec_clock::local_time();
        size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
        std::string msg(buff_, pos);
        std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
        already_read_ -= pos + 1;

        std::cout << "msg received: " << msg << ", from: " << username_ << std::endl;
        if (msg.find("login ") == 0) on_login(msg);
        else if ( msg.find("ping") == 0) on_ping();
        else if ( msg.find("ask_clients") == 0) on_clients();
        else std::cerr << "invalid msg " << msg << std::endl;
    }
    void on_login(const std::string & msg) {
        std::istringstream in(msg);
        in >> username_ >> username_;
        write("login ok\n");
        update_clients_changed();
    }
    void on_ping() {
        write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
        clients_changed_ = false;
    }
    void on_clients() {
        std::string msg;
        {
            boost::recursive_mutex::scoped_lock lk(cs);
            for( array::const_iterator b = clients.begin(), e = clients.end(); b != e; ++b)
                msg += (*b)->username() + " ";
        }
        write("clients " + msg + "\n");
    }
    void write(const std::string & msg) {
        std::string msg_no_endl(msg, 0, msg.size()-1);
        std::cout << "msg written: " << msg_no_endl << ", to: " << username_ << std::endl;;
        sock_.write_some(buffer(msg));
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool clients_changed_;
    boost::posix_time::ptime last_ping;
    std::string username_;
};

void accept_thread() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    while ( true) {
        client_ptr new_( new talk_to_client);
        acceptor.accept(new_->sock());
        boost::recursive_mutex::scoped_lock lk(cs);
        std::cout << "connection established" << std::endl;
        clients.push_back(new_);
    }
}

void handle_clients_thread() {
    while ( true) {
        boost::this_thread::sleep(boost::posix_time::millisec(1));
        boost::recursive_mutex::scoped_lock lk(cs);
        for(array::iterator b = clients.begin(),e = clients.end(); b!= e; ++b)
            (*b)->answer_to_client();
         // erase clients that timed out
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            boost::bind(&talk_to_client::timed_out,_1)),
        clients.end());
    }
}

int main(int argc, char* argv[]) {
    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(handle_clients_thread);
    threads.join_all();
}
