#include <iostream>
#include <boost/asio.hpp>
#include "server.hpp"
#include "Common.hpp"
#include <thread>
#include <chrono>
#include <mutex>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;
        static Core core;
        //std::mutex mtx;

        server s(io_service);
        
        std::thread th2([&io_service/*, &mtx*/](){
            unsigned int load = 10;

            while(true) {
                if(core.GetRecentFlag()) {
                    load = 10;
                    core.SetRecentFlag(false);
                } else {
                    if (load < 100) {
                        load++;
                    }
                }
                //mtx.lock();
                io_service.post(consCall);
                //mtx.unlock();

                std::this_thread::sleep_for(std::chrono::milliseconds(load));
            }
            
        });
        
        std::thread th1([&io_service]() {
            io_service.run();
        });
        
        th1.join();
        th2.join();

        /*
        std::thread th2([](){
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            boost::asio::io_service io_service;

            tcp::resolver resolver(io_service);
            tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
            tcp::resolver::iterator iterator = resolver.resolve(query);

            
            
            nlohmann::json rq;
            rq["ReqType"] = Requests::Consume;

            std::string request = rq.dump();
            while(true) {
                tcp::socket s(io_service);
                s.connect(*iterator);
                boost::asio::write(s, boost::asio::buffer(request, request.size()));
            }
            //boost::asio::write(s, boost::asio::buffer(request, request.size()));
            
            /*
            while(true) {
                core.Consume();
            }
        });
        
    */
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}