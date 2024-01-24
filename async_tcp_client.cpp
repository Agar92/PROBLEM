#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class TCPClient {
public:
    TCPClient(boost::asio::io_context& io_service,
              const std::string& server, const std::string& message)
      : socket_(io_service), message_(message + "\n"),
        resolver(io_service), query(server, "1234"),
        endpoint_iterator( resolver.resolve(query) ) {
    }

  void run(){
    boost::asio::async_connect(socket_, endpoint_iterator,
            [this](const boost::system::error_code& error, tcp::resolver::iterator){
                if (!error) {
                    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                        [this](const boost::system::error_code& error, std::size_t) {
                            if (!error) {
                                boost::asio::async_read_until(socket_, reply_, "\n",
                                    [this](const boost::system::error_code& error, std::size_t) {
                                        if (!error) {
                                            std::cout << &reply_;
                                        }
                                    });
                            }
                        });
                }
            });
  }

private:
  tcp::socket socket_;
  std::string message_;
  boost::asio::streambuf reply_;
  //
  tcp::resolver resolver;
  tcp::resolver::query query;
  tcp::resolver::iterator endpoint_iterator;
};

int main() {
    boost::asio::io_context io_context;
    TCPClient client(io_context, "localhost", "Hello, Server!");
    client.run();
    io_context.run();
}
