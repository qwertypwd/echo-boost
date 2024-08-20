#include <iostream>
#include "json.hpp"
#include "Common.hpp"

namespace Models
{
	// User
    class User {
    public:
        User(size_t id, const std::string& name)
            : id_(id), name_(name)
        {
            usdBalance = 0;
            rurBalance = 0;
            //std::cout << name << " registered " << std::endl;
        }

        const std::string& getUserName() {
            return name_;
        }

        int getUserRur() {
            return rurBalance;
        }
        void setUserRur(int val) {
            rurBalance = val;
        }
        int getUserUsd() {
            return usdBalance;
        }
        void setUserUsd(int val) {
            usdBalance = val;
        }
        // (usd params)
        void exchange(int qty, int cost) {
            usdBalance += qty;
            rurBalance += qty * (-1) * cost;
        }

    private:
        size_t id_;
        std::string name_;
        int usdBalance;
        int rurBalance;
    };

    class Application {
    public:
        Application(const nlohmann::json& j)
        {
            this->type = j["ReqType"];
            this->clientId = j["UserId"];
            this->qty = j["qty"];
            this->cost = j["cost"];
            this->timestamp = std::time(0);
        }

        unsigned int getAppQty() {
            return qty;
        }
        void setAppQty(unsigned int val) {
            qty = val;
        }
        int getAppCost() {
            return cost;
        }
        void setAppCost(int val) {
            cost = val;
        }
        size_t getAppCliendId() {
            return clientId;
        }

        struct less;
        struct greater;
    private:
        std::string type;
        size_t clientId;
        unsigned int qty;    // >=0?? uint
        int cost;
        std::time_t timestamp;
    };

    // applicationcomparation cond
    struct Application::less
    {
        bool operator()(Application* l, Application* r)
        {   
            return l->cost == r->cost
                ? l->timestamp >= r->timestamp
                : l->cost < r->cost;
        }
    };
    struct Application::greater
    {
        bool operator()(Application* l, Application* r)
        {
            return l->cost == r->cost
                ? l->timestamp >= r->timestamp
                : l->cost > r->cost;
        }
    };
}
