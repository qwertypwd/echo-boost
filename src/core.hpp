#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include "json.hpp"
#include "Common.hpp"
#include "models.hpp"
#include "testcases.hpp"

using boost::asio::ip::tcp;

class Core
{
    friend struct ::testSuiteCore::testCore;
    friend struct ::testSuiteCore::testCoreCreateApp;
    friend struct ::testSuiteCore::testCoreQueue;
    friend struct ::testSuiteCore::testCoreQueueEquals;
    friend struct ::testSuiteCore::testCoreConsumer;
public:
    // "Регистрирует" нового пользователя и возвращает его ID.
    size_t RegisterNewUser(const std::string& aUserName)
    {
        size_t newUserId = mUsers.size();
        Models::User* aUser = new Models::User(newUserId, aUserName);
        mUsers[newUserId] = aUser;
        //std::cout << newUserId << " create userId" << std::endl;
        return newUserId;
    }

    // Запрос имени клиента по ID
    Models::User* GetUser(size_t aUserId)
    {
        std::cout << aUserId << " get userId" << std::endl;
        const auto userIt = mUsers.find(aUserId);
        if (userIt == mUsers.cend())
        {   
            std::cout << "Error! Unknown User" << std::endl;
            return new Models::User(0, ""); 
        }
        else
        {
            return userIt->second;
        }
    }
    
    // Application create
    void CreateApplication(const nlohmann::json& j)
    {
        this->SetRecentFlag(true);
        Models::Application* app = new Models::Application(j);
        if(j["ReqType"] == Requests::Sale) 
        {
            sq.push(app);
        }
        else if (j["ReqType"] == Requests::Purchase)
        {
            pq.push(app);
        }
        else 
        {
            std::cerr << "undefined action" << std::endl;
        }
    }

    void Consume() {
        std::cout << "consumer call" << std::endl;
        if (sq.empty() or pq.empty()) 
        {
            return;
        }

        Models::User* buyer = GetUser(pq.top()->getAppCliendId());
        Models::User* seller = GetUser(sq.top()->getAppCliendId());

        int qty;
        int cost;

        if (pq.top()->getAppCost() >= sq.top()->getAppCost()) 
        {
            cost = pq.top()->getAppCost();
            if (pq.top()->getAppQty() == sq.top()->getAppQty()) 
            {
                qty = pq.top()->getAppQty();
                sq.pop();
                pq.pop();
            }
            else if (pq.top()->getAppQty() > sq.top()->getAppQty())
            {
                qty = sq.top()->getAppQty();
                pq.top()->setAppQty(pq.top()->getAppQty() - qty);
                sq.pop();
            }
            else
            {
                qty = pq.top()->getAppQty();
                sq.top()->setAppQty(sq.top()->getAppQty() - qty);
                pq.pop();
            }

            std::cout << "start exchanging" << '\n';
            std::cout << seller->getUserUsd() << '\t' << seller->getUserRur() << '\t' << seller->getUserName() << " (seller)" << '\n';
            std::cout << buyer->getUserUsd() << '\t' << buyer->getUserRur() << '\t' << buyer->getUserName() << " (buyer)" << '\n';

            buyer->exchange(qty, cost);
            seller->exchange(-qty, cost);

            std::cout << "exchanged " << qty << " usd " << cost << " rur" << '\n';
            std::cout << seller->getUserUsd() << '\t' << seller->getUserRur() << '\t' << seller->getUserName() << " (seller)" << '\n';
            std::cout << buyer->getUserUsd() << '\t' << buyer->getUserRur() << '\t' << buyer->getUserName() << " (buyer)" << '\n';
            std::cout << std::endl;
        }
    }

    bool GetRecentFlag() {
        return recentApp;
    }

    void SetRecentFlag(bool b) {
        recentApp = b;
    }
private:
    // <UserId, UserName> // User
    std::map<size_t, Models::User*> mUsers;
    // Sales app queue
    std::priority_queue<Models::Application*, std::vector<Models::Application*>, Models::Application::greater> sq;
    // Purchase app queue
    std::priority_queue<Models::Application*, std::vector<Models::Application*>, Models::Application::less> pq;
    // Kostil))0
    bool recentApp = false;
};

Core& GetCore()
{
    static Core core;
    return core;
}