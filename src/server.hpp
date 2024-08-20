#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include "json.hpp"
#include "Common.hpp"
#include "core.hpp"

class session
{
public:
    session(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    // Обработка полученного сообщения.
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            data_[bytes_transferred] = '\0';

            // Парсим json, который пришёл нам в сообщении.
            auto j = nlohmann::json::parse(data_);
    
            std::string reply = manage_req(j);

            boost::asio::async_write(socket_,
                boost::asio::buffer(reply, reply.size()),
                boost::bind(&session::handle_write, this,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                boost::bind(&session::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

    std::string manage_req(const nlohmann::json& j) {
        std::string reply;
        std::string reqType = j["ReqType"];

        if (reqType == Requests::Registration)
        {
            // Это реквест на регистрацию пользователя.
            // Добавляем нового пользователя и возвращаем его ID.
            std::cout << "Register request" << std::endl;
            reply = std::to_string(GetCore().RegisterNewUser(j["Message"]));
        }
        else if (reqType == Requests::Hello)
        {
            // Это реквест на приветствие.
            // Находим имя пользователя по ID и приветствуем его по имени.
            std::cout << "Hello request" << std::endl;
            reply = "Hello, " + GetCore().GetUser(j["UserId"])->getUserName() + "!\n";
        }
        else if (reqType == Requests::Sale or reqType == Requests::Purchase)
        {
            std::cout << "New application" << std::endl;
            GetCore().CreateApplication(j);
            reply = "New application created\n";
        }
        /*
        else if (reqType == Requests::Consume)
        {
            GetCore().Consume();
            reply = "";
        } */
        else
        {
            reply = "Error! Unknown request type";
        }
        return reply;
    }

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_service& io_service)
        : io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        std::cout << "Server started! Listen " << port << " port" << std::endl;

        session* new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
    }

    void handle_accept(session* new_session,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
            new_session = new session(io_service_);
            acceptor_.async_accept(new_session->socket(),
                boost::bind(&server::handle_accept, this, new_session,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete new_session;
        }
    }

    tcp::acceptor& get_acceptor() 
    {
        return acceptor_;
    }

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

void consCall() {
    GetCore().Consume();
}