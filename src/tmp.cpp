#include <cstdlib>
#include <iostream>
#include <string>
#include "json.hpp"

struct User {
    User(int id_, const std::string& name_)
        : id(id_), name(name_)
    {
        this->usdBalance = 0;
        this->rurBalance = 0;
    }
    int id;
    std::string name;
    int usdBalance;
    int rurBalance;
};

class ISerializer {
public:
    ISerializer(const std::string& id_, const std::string& reqType_) 
        : id(id_), reqType(reqType_)
    {
        req["UserId"] = id;
        req["ReqType"] = reqType;
    }
    virtual nlohmann::json get() {
        nlohmann::json j = std::move(req);
        this->~ISerializer();
        return j;
    }
private:
    nlohmann::json req;
    std::string id;
    std::string reqType;
    //req["UserId"] = aId;
    //req["ReqType"] = aRequestType;
};

int main() {
    ISerializer req("0", "");
    auto tmp = req.get();
    std::cout << tmp["UserId"] << std::endl;
    
    /*
    User aUser(123, "aboba");

    nlohmann::json req;
    req["UserId"] = 123;
    req["ReqType"] = "sale";
    req["Message"] = "msg";

    std::string request = req.dump();
    std::cout << request << std::endl;

    auto j = nlohmann::json::parse(request);
    int a = j["UserId"];
    std::cout << (a + 5) << std::endl;
    */
    return 0;
}
