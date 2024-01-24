#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <memory>

//g++ server.cpp -o server -lboost_system -lpthread

using namespace boost::asio;

struct Session : std::enable_shared_from_this<Session> {
  explicit Session(ip::tcp::socket socket) : socket{ std::move(socket) } { }
  void read() {
    async_read_until(socket, dynamic_buffer(message), "\n",
            [self=shared_from_this()] (boost::system::error_code ec,
                                       std::size_t length) {
              //std::cout<<"#1#:   "<<self->message<<std::endl;
              if (ec || self->message == "\n") return;
              if(!ec) std::cout<<"!ec"<<std::endl;
              //std::cout<<"#2#:   "<<self->message<<std::endl;
              boost::algorithm::to_upper(self->message);
              //std::cout<<"#3#:   "<<self->message<<std::endl;
              self->write();
              //std::cout<<"#4#:   "<<self->message<<std::endl;
            });
  }
  void write() {
    //std::cout<<"@1@:   "<<message<<std::endl;
    message+="\n";
    async_write(socket, buffer(message),
                         [self=shared_from_this()] (boost::system::error_code ec,
                                           std::size_t length) {
                  //std::cout<<"***A***:   "<<self->message
                  //         <<" length="<<length
                  //         <<std::endl;
                  if (ec) return;
                  self->message.clear();
                  //std::cout<<"***B***:   "<<self->message
                  //         <<" length="<<length
                  //         <<std::endl;
                  self->read();
                });
    //std::cout<<"@2@:   "<<message<<std::endl;
  }
private:
  ip::tcp::socket socket;
  std::string message;
};

void serve(ip::tcp::acceptor& acceptor) {
  acceptor.async_accept([&acceptor](boost::system::error_code ec,
                                    ip::tcp::socket socket) {
    std::cout<<"socket.local_endpoint():   "<<socket.local_endpoint()<<std::endl;
    std::cout<<"socket.remote_endpoint():   "<<socket.remote_endpoint()<<std::endl;
    serve(acceptor);
    if (ec) return;
    else std::cout<<"GOOD!"<<std::endl;
    auto session = std::make_shared<Session>(std::move(socket));
    session->read();
  });
}

int main()  {
  try {
    io_context io_context;
    ip::tcp::acceptor acceptor{ io_context,
                                ip::tcp::endpoint(ip::tcp::v4(), 1895) };
    serve(acceptor);
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
