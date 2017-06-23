// https://www.gamedev.net/blogs/entry/2249317-a-guide-to-getting-started-with-boostasio/
//
// 1. The basics of io_service
//

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>

// ##### Example 1a #####
//
/*
int main( int argc, char * argv[] )
{
    boost::asio::io_service io_service;

    io_service.run();

    std::cout << "Do you reckon this line displays?" << std::endl;

    return 0;
}
*/

// ##### Example 1b #####
//
/*
int main( int argc, char * argv[] )
{
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );

    io_service.run();

    std::cout << "Do you reckon this line displays?" << std::endl;

    return 0;
}
*/

// ##### Example 1c #####
//
/*
int main( int argc, char * argv[] )
{
    boost::asio::io_service io_service;

    for( int x = 0; x < 42; ++x )
    {
        io_service.poll();
        std::cout << "Counter: " << x << std::endl;
    }

    return 0;
}
*/

// ##### Example 1d #####
//
/*
int main( int argc, char * argv[] )
{
    boost::asio::io_service io_service;
    boost::asio::io_service::work work( io_service );

    for( int x = 0; x < 42; ++x )
    {
        io_service.poll();
        std::cout << "Counter: " << x << std::endl;
    }

    return 0;
}
*/

// ##### Example 1e #####
//
/*
int main( int argc, char * argv[] )
{
    boost::asio::io_service io_service;
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( io_service )
    );

    work.reset();

    io_service.run();

    std::cout << "Do you reckon this line displays?" << std::endl;

    return 0;
}
*/

// ##### Example 1f #####
//
boost::asio::io_service io_service;

void WorkerThread()
{
    std::cout << "Thread Start\n";
    io_service.run();
    std::cout << "Thread Finish\n";
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( io_service )
    );

    std::cout << "Press [return] to exit." << std::endl;

    boost::thread_group worker_threads;
    for( int x = 0; x < 4; ++x )
    {
        worker_threads.create_thread( WorkerThread );
    }

    std::cin.get();

    io_service.stop();

    worker_threads.join_all();

    return 0;
}
