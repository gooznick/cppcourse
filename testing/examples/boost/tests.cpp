#define BOOST_TEST_MODULE BoostTestDemo
#include <boost/test/included/unit_test.hpp>   // <-- header-only runner

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <stdexcept>
#include <string>
#include <vector>
#include <thread>
namespace bdata = boost::unit_test::data;

// ------------------------
// Helpers + small "library"
// ------------------------
static int div_int(int a, int b) {
    if (b == 0) throw std::invalid_argument("division by zero");
    return a / b;
}

struct Buffer {
    std::vector<int> v;
    void push(int x) {
        if (x < 0) throw std::range_error("negative not allowed");
        v.push_back(x);
    }
    std::size_t size() const { return v.size(); }
};

// ------------------------
// Suite 1: Basics
// ------------------------
BOOST_AUTO_TEST_SUITE(BasicsSuite)

BOOST_AUTO_TEST_CASE(Smoke)
{
    BOOST_TEST(1 + 1 == 2);
}

BOOST_AUTO_TEST_CASE(Strings)
{
    std::string s = "hello";
    BOOST_TEST(s.size() == 5u);
    BOOST_TEST(s.find("ell") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(FloatingTolerance)
{
    double x = 0.1 + 0.2;     // classic
    BOOST_TEST(x == 0.3, boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------
// Suite 2: Exceptions
// ------------------------
BOOST_AUTO_TEST_SUITE(ExceptionsSuite)

BOOST_AUTO_TEST_CASE(DivByZeroThrows)
{
    BOOST_CHECK_THROW(div_int(10, 0), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(DivNoThrow)
{
    BOOST_CHECK_NO_THROW(div_int(10, 2));
    BOOST_TEST(div_int(10, 2) == 5);
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------
// Suite 3: Fixture
// ------------------------
struct BufferFixture {
    Buffer buf;
    BufferFixture() {
        buf.push(1);
        buf.push(2);
    }
};

BOOST_FIXTURE_TEST_SUITE(FixtureSuite, BufferFixture)

BOOST_AUTO_TEST_CASE(InitialState)
{
    BOOST_TEST(buf.size() == 2u);
}

BOOST_AUTO_TEST_CASE(PushRejectsNegative)
{
    BOOST_CHECK_THROW(buf.push(-7), std::range_error);
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------
// Suite 4: Data-driven tests
// ------------------------
BOOST_AUTO_TEST_SUITE(DataDrivenSuite)

BOOST_DATA_TEST_CASE(DivisionTable,
    bdata::make(std::vector<int>{10, 20, 30})*
    bdata::make(std::vector<int>{1, 2, 3 }),
    a, b)
{
    BOOST_TEST(div_int(a, b) * b == a);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DisabledSuite)

// Utility function to simulate work
void simulate_work(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// 1. Check that test takes less than 100ms
// Note: Boost timeout fails the test if it exceeds this limit.
BOOST_AUTO_TEST_CASE(test_fast_execution, *boost::unit_test::timeout(1)) {
    // We expect this to pass
    simulate_work(50);
    BOOST_CHECK(true);
}

// 2. Fail if it takes more than 1000ms (1 second)
// The timeout decorator is the standard way to enforce this.
BOOST_AUTO_TEST_CASE(test_timeout_failure, *boost::unit_test::timeout(1)) {
    // This will be terminated by the framework and marked as FAILED
    simulate_work(1200);
    BOOST_CHECK(true);
}

// 3. This test is ignored (disabled)
BOOST_AUTO_TEST_CASE(test_ignored, *boost::unit_test::disabled()) {
    BOOST_ERROR("This code will never run");
}
BOOST_AUTO_TEST_SUITE_END()
