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

class Server:public boost::enable_shared_from_this<Server>
{
private:
 
    static const int message_length=1024;
    char read_buf[message_length];
    ip::tcp::socket ser;  //read_sock_
    boost::scoped_ptr<ip::tcp::acceptor> acc;
 
public:
    Server():ser(service){}
 
    void start_server()
    {
        std::cout<<"Hellow from server"<<std::endl;
        acc.reset(new ip::tcp::acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 25000)));
        acc->async_accept(ser, boost::bind(&Server::handle_accept,shared_from_this(),_1));
    }
 
    void handle_accept(const boost::system::error_code& err)
    {
        if (err)
        {
            std::cout << "Error in accept: " << err << std::endl;
            do_close();
        }
        do_server_read();
    }
 
    void do_close()
    {
        std::cout << "Do close socket" << std::endl;
        if (ser.is_open()) ser.close();
        if (acc&& acc->is_open()) acc->close();
        throw std::logic_error("timeout");
    }
 
    void do_server_read()
    {
        async_read(ser, buffer(read_buf,message_length),boost::bind(&Server::handle_read,shared_from_this(), _1));
    }
 
    void handle_read(const boost::system::error_code& err)
    {
        if (err)
        {
            std::cout << "Error in read: " << err;
            do_close();
        }
        std::cout << "From client: " << std::string(read_buf) << std::endl;
        do_write_answer();
    }
 
    void do_write_answer()
    {
        std::string s(read_buf);
        boost::to_upper(s);
 /*
        if(s=="hi")
        {
            strcpy(read_buf,"Hello");
        }
        else if(s=="how are u?")
        {
            strcpy(read_buf,"Fine, ty");
        }
        else if(s=="Do u really work?")
        {
            strcpy(read_buf,"Yes, i do");
        }
        else strcpy(read_buf,"idk");
*/
        strcpy(read_buf,s.data());
        async_write(ser, buffer(read_buf),boost::bind(&Server::handle_write_answer,shared_from_this(),_1,_2));
    }
 
    void handle_write_answer(const boost::system::error_code& err, size_t bytes)
    {
        if (err)
        {
            std::cout << "Error in write: " << err << std::endl;
            do_close();
        }
        do_server_read();
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int main(int argc, char *argv[])
{
    boost::shared_ptr<Client_Server::Server> s=boost::make_shared<Client_Server::Server>();
    s->start_server();
    Client_Server::service.run();
}