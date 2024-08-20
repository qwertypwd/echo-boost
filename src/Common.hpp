#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>
#include "json.hpp"

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Sale = "Sal";
    static std::string Purchase = "Pur";
    static std::string Consume = "Con";
}

namespace Serialization
{
    class ISerializer {
    public:
        ISerializer() = default;
        void setHeader(size_t id, const std::string& reqType) {
            rq["UserId"] = id;
            rq["ReqType"] = reqType;
        }
        virtual void setBody() = 0;
        nlohmann::json get() {
            nlohmann::json j = std::move(rq);
            this->~ISerializer();
            return std::move(j);
        }
    protected:
        nlohmann::json rq;
    };

    class Message : public ISerializer {
    public:
        void setBody() override {}
        void setBody(const std::string& msg) {
            rq["Message"] = msg;
        }
    };
    
    class Application : public ISerializer {
    public:
        void setBody() override {}
        void setBody(unsigned int qty, int cost) {
            rq["qty"] = qty;
            rq["cost"] = cost;
        }
    };
}
#endif //CLIENSERVERECN_COMMON_HPP
