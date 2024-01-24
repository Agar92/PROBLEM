#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace boost::asio;

//$g++ client.cpp -o client -lboost_system -lpthread

int main()  {
  try {
    io_context io_context;
    boost::asio::ip::tcp::socket socket{io_context};
    socket.connect(ip::tcp::endpoint(ip::tcp::v4(),1895));
    while(true)
    {
      std::string input;
      std::cin>>input;
      std::string msg=input;
      msg+="\n";
      boost::system::error_code error;
      /*
      boost::asio::async_write(socket,
                               boost::asio::buffer(msg),
                               [](boost::system::error_code ec,
                                  size_t transferred){
                                 std::cout<<"async_write Completion!"<<std::endl;
                                 if(!ec)
                                   std::cout<<"Client sent message!"<<std::endl;
                                 else
                                   std::cout<<"Client did not send the message!"<<std::endl;
                               });
      */
      //*
      boost::asio::write(socket, boost::asio::buffer(msg), error);
      if(!error)
        std::cout<<"Client sent message="<<msg<<std::endl;
      else
        std::cout<<"send failed:"<<error.message()<<std::endl;
      //*/
      std::string response;
      std::cout<<"BEFORE"<<std::endl;
      std::cout<<"socket.local_endpoint():   "<<socket.local_endpoint()<<std::endl;
      std::cout<<"socket.remote_endpoint():   "<<socket.remote_endpoint()<<std::endl;
      boost::system::error_code err;
      boost::asio::streambuf buf;
      boost::asio::read_until(socket, buf,"\n");
      /*
      boost::asio::async_read_until(socket,
                                   dynamic_buffer(response), "\n",
                                    [] (boost::system::error_code ec,
                                        std::size_t length) {
                                      if(ec) std::cout<<"Error!!!"<<std::endl;
                                      
                                    });
      */
      std::cout<<"AFTER"<<std::endl;
      if(error && error != boost::asio::error::eof)
      {
        std::cout<<"Read failed!"<<error.message()<<std::endl;
      }
      else
      {
        std::cout<<"Reading..."<<std::endl;
        std::string data(boost::asio::buffer_cast<const char*>(buf.data()));
        std::cout<<data<<"  "
                 <<data.length()<<"  "
                 <<std::endl;
        for(int i=0; i<15; ++i) std::cout<<unsigned(response[i])<<" ";
        std::cout<<std::endl;
        response.clear();
      }
    }
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
