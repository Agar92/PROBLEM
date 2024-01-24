#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/chrono.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
using namespace boost::asio;
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Client_Server
{
 
io_context service;
 
std::ostream& operator << (std::ostream& stream, const boost::system::error_code& err)
{
    return stream << "Category: " << err.category().name() << " message: " << err.message() << std::endl;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Client
{
private:
    static const int message_length=1024;
    ip::tcp::socket cl;  // write_sock_
    char write_buf[message_length];
public:
    Client():cl(service){}
 
    void start_client()
    {
        std::cout<<"Hello from client"<<std::endl;
        cl.async_connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 25000),boost::bind(&Client::handle_connect, this, _1));
    }
 
    void handle_connect(const boost::system::error_code& err)
    {
        if (err)
        {
            std::cout << "Error in connect: " << err << std::endl;
            do_close();
        }
        do_write();
    }
 
    void do_close()
    {
        std::cout << "Do close socket" << std::endl;
        if (cl.is_open()) cl.close();
        throw std::logic_error("timeout");
    }
 
    void do_write()
    {
        std::cout<<"client, input message: "<<std::endl;
        std::cin.getline(write_buf,message_length);
        async_write(cl,buffer(write_buf,message_length),boost::bind(&Client::handle_write, this,_1));
    }
 
    void handle_write(const boost::system::error_code& err)
    {
        if (err)
        {
            std::cout << "Write error: " << err << std::endl;
            do_close();
        }
        do_read_answer();
    }
 
    void do_read_answer()
    {
        async_read(cl, buffer(write_buf,message_length),boost::bind(&Client::handle_read_answer, this,_1));
    }
 
    void handle_read_answer(const boost::system::error_code& err)
    {
        if (err)
        {
            std::cout << "Error in read answer: " << err << std::endl;
            do_close();
        }
        std::cout << "From server: " << std::string(write_buf) << std::endl;
        do_write();
    }
};
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int main(int argc, char *argv[])
{
    boost::shared_ptr<Client_Server::Client> c=boost::make_shared<Client_Server::Client>();
    c->start_client();
    Client_Server::service.run();
}