#include "server.hpp"
#include "json.hpp"
#include "Common.hpp"
#include <chrono>
#include <thread>

#define BOOST_TEST_MODULE testCore

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(testSuiteCore)

struct Fixture
{
	Fixture()
		: core()
	{
		// Здесь тестовый объект можно настроить
		bibaId = core.RegisterNewUser("biba"); 
		bobaId = core.RegisterNewUser("boba"); 
	}
	~Fixture()
	{
		// А здесь корректно завершить с ним работу
	}
	// А вот и сам тестовый объект
	Core core;
	std::string bibaId;
	std::string bobaId;
};
/*
BOOST_FIXTURE_TEST_CASE(testCore, Fixture)
{
}
 
BOOST_FIXTURE_TEST_CASE(testCoreCreateApp, Fixture)
{
    nlohmann::json req;
    req["UserId"] = 0;
    req["ReqType"] = Requests::Sale;
    req["qty"] = 1;
    req["cost"] = 99;
    //timestamp;
	core.CreateApplication(req);

	auto head = core.sq.top();
	BOOST_CHECK_EQUAL(head->getAppCost(), 99);
}
 
BOOST_FIXTURE_TEST_CASE(testCoreQueue, Fixture)
{
    nlohmann::json req;
	nlohmann::json req2;
	nlohmann::json req3;

    req["UserId"] = 0;
    req["ReqType"] = Requests::Sale;
    req["qty"] = 1;
    req["cost"] = 99;

	req2["UserId"] = 0;
    req2["ReqType"] = Requests::Sale;
    req2["qty"] = 1;
    req2["cost"] = 199;

	req3["UserId"] = 0;
    req3["ReqType"] = Requests::Sale;
    req3["qty"] = 1;
    req3["cost"] = 109;

	core.CreateApplication(req);
	core.CreateApplication(req2);
	core.CreateApplication(req3);

	auto head = core.sq.top();
	BOOST_CHECK_EQUAL(head->getAppCost(), 99);
}

BOOST_FIXTURE_TEST_CASE(testCoreQueueEquals, Fixture)
{
    nlohmann::json req;
	nlohmann::json req2;

    req["UserId"] = 0;
    req["ReqType"] = Requests::Sale;
    req["qty"] = 1;
    req["cost"] = 99;

	req2["UserId"] = 1;
    req2["ReqType"] = Requests::Sale;
    req2["qty"] = 1;
    req2["cost"] = 99;


	core.CreateApplication(req);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	core.CreateApplication(req2);

	auto head = core.sq.top();
	BOOST_CHECK_EQUAL(head->getAppCliendId(), 0);
}
*/

BOOST_FIXTURE_TEST_CASE(testCoreConsumer, Fixture)
{
    nlohmann::json req;
	nlohmann::json req2;

    req["UserId"] = bibaId;
    req["ReqType"] = Requests::Sale;
    req["qty"] = 10;
    req["cost"] = 99;

	req2["UserId"] = bobaId;
    req2["ReqType"] = Requests::Purchase;
    req2["qty"] = 1;
    req2["cost"] = 99;


	core.CreateApplication(req);
	core.CreateApplication(req2);

	core.Consume();

	BOOST_CHECK_EQUAL(core.sq.size(), 1);
	BOOST_CHECK(core.pq.empty());
}

BOOST_AUTO_TEST_SUITE_END()
