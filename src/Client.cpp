#include <iostream>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket& aSocket,
    nlohmann::json& rq)
{
    std::string request = rq.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string ReadMessage(tcp::socket& aSocket)
{
    boost::asio::streambuf b;
    boost::asio::read_until(aSocket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

class MenuManager {
public:
    MenuManager(tcp::socket& s) 
        : aSocket(s)
    {
        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам ID.
        // Этот ID далее используется при отправке запросов.
        my_id = ProcessRegistration();
    }

    // Choose menu option
    void Command();
    
    // "Создаём" пользователя, получаем его ID.
    size_t ProcessRegistration();

    // Для примера того, как может выглядить взаимодействие с сервером
    // реализован один единственный метод - Hello.
    // Этот метод получает от сервера приветствие с именем клиента,
    // отправляя серверу id, полученный при регистрации.
    void Hello();
    
    // Send sale or purchase application
    void SendApplication(const std::string& reqType);
    
private:
    tcp::socket& aSocket;
    size_t my_id;
};

size_t MenuManager::ProcessRegistration()
{
    std::string name;
    std::cout << "Hello! Enter your name: ";
    std::cin >> name;

    Serialization::Message msg;
    // Для регистрации Id не нужен, заполним его нулём
    msg.setHeader(0, Requests::Registration);
    msg.setBody(name);
    auto j = msg.get();
    
    SendMessage(aSocket, j);

    return std::stoi(ReadMessage(aSocket));
}

void MenuManager::Hello() 
{
    Serialization::Message msg;
    msg.setHeader(my_id, Requests::Hello);
    msg.setBody("");
    auto j = msg.get();
    SendMessage(aSocket, j);
    std::cout << ReadMessage(aSocket);
}

void MenuManager::SendApplication(const std::string& reqType) 
{
    unsigned int qty;
    int cost;
    std::cout << "Enter (<quantity>, <cost>) the data: ";
    std::cin >> qty >> cost;

    Serialization::Application app;
    app.setHeader(my_id, reqType);
    app.setBody(qty, cost);
    auto j = app.get();
    SendMessage(aSocket, j);
    std::cout << ReadMessage(aSocket);
}

void MenuManager::Command()
{
    // Тут реализовано "бесконечное" меню.
    std::cout << "Menu:\n"
                    "1) Hello Request\n"
                    "2) Send Sale Application\n"
                    "3) Send Purchase Application\n"
                    "4) Exit\n"
                    << std::endl;

    short menu_option_num;
    std::cin >> menu_option_num;
    switch (menu_option_num)
    {
        case 1:
        {
            Hello();
            break;
        }
        case 2:
        {
            SendApplication(Requests::Sale);
            break;
        }
        case 3:
        {
            SendApplication(Requests::Purchase);
            break;
        }
        case 4:
        {
            exit(0);
            break;
        }
        default:
        {
            std::cout << "Unknown menu option\n" << std::endl;
        }
    }
}

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        MenuManager menu(s);
        while(true) {
            menu.Command();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}