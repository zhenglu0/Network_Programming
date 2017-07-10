// https://www.gamedev.net/blogs/entry/2249317-a-guide-to-getting-started-with-boostasio/

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>

// Example 5a
//
/*
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    try
    {
        io_service->run();
    }
    catch( std::exception & ex )
    {
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
            << "] Exception: " << ex.what() << std::endl;
        global_stream_lock.unlock();
    }

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] " << __FUNCTION__ << std::endl;
    global_stream_lock.unlock();

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(
        new boost::asio::io_service
        );
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( *io_service )
        );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}
*/

// Example 5b
//
/*
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    boost::system::error_code ec;
    io_service->run( ec );

    if( ec )
    {
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
            << "] Exception: " << ec << std::endl;
        global_stream_lock.unlock();
    }

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] " << __FUNCTION__ << std::endl;
    global_stream_lock.unlock();

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(
        new boost::asio::io_service
        );
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( *io_service )
        );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}
*/

// Example 5c
//
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service->run( ec );
            if( ec )
            {
                global_stream_lock.lock();
                std::cout << "[" << boost::this_thread::get_id()
                    << "] Error: " << ec << std::endl;
                global_stream_lock.unlock();
            }
            break;
        }
        catch( std::exception & ex )
        {
            global_stream_lock.lock();
            std::cout << "[" << boost::this_thread::get_id()
                << "] Exception: " << ex.what() << std::endl;
            global_stream_lock.unlock();
        }
    }

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] " << __FUNCTION__ << std::endl;
    global_stream_lock.unlock();

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(
        new boost::asio::io_service
        );
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( *io_service )
        );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}
