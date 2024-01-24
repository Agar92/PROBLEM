#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "unistd.h"

using boost::asio::ip::tcp;

class TCPClient {
public:
    TCPClient(boost::asio::io_context& io_service,
              const std::string& server)
      : socket_(io_service),
        resolver(io_service), query(server, "1895"),
        endpoint_iterator( resolver.resolve(query) ) {
    }

  void SetMessage(const std::string & msg){message=msg;}

  void run(const std::string & msg, std::promise<bool> && promise){
    message=msg;
    message+="\n";
    boost::asio::async_connect(socket_, endpoint_iterator,
            [this](const boost::system::error_code& error, tcp::resolver::iterator){
                if (!error) {
                    std::cout<<"socket_.local_endpoint():   "<<socket_.local_endpoint()<<std::endl;
                    std::cout<<"socket_.remote_endpoint():   "<<socket_.remote_endpoint()<<std::endl;
                    boost::asio::async_write(socket_, boost::asio::buffer(message),
                        [this](const boost::system::error_code& error, std::size_t) {
                            if (!error) {
                                boost::asio::async_read_until(socket_, reply_, "\n",
                                    [this](const boost::system::error_code& error, std::size_t) {
                                        if (!error) {
                                            std::string data(boost::asio::buffer_cast<const char*>(reply_.data()));
                                            std::cout << data << std::endl;
                                        }
                                        else
                                          std::cout<<"error"<<std::endl;
                                    });
                            }
                            else std::cout<<"error*#*"<<std::endl;
                        });
                }
                else std::cout<<"ERROR async_connect"<<std::endl;
            });
    promise.set_value(true);
  }

private:
  tcp::socket socket_;
  std::string message="WERT";
  boost::asio::streambuf reply_;
  //
  tcp::resolver resolver;
  tcp::resolver::query query;
  tcp::resolver::iterator endpoint_iterator;
};

int main() {
    using namespace std;
    try {
      boost::asio::io_context io_context;
      TCPClient client(io_context, "localhost");
      std::string msg="";
      //while(true)
      {
        std::promise<bool> promise;
        std::cin>>msg;
        cout<<"msg="<<msg<<endl;
        client.run(msg, std::move(promise));
        auto fut=promise.get_future();
        fut.get();
      }
      io_context.run();
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    
}
