#include "jsonparse.hpp"
#include <iostream>
#include <format>

using namespace std;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), int(expect), actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), double(expect), actual, "%f")
#define EXPECT_EQ_JSON(expect, str) EXPECT_EQ_INT((expect), (static_cast<int>(Parser::FromString(str).inner_.index())))

#define TEST_NUMBER(expect, json) \
    do {\
        auto res = Parser::FromString(json).inner_;\
        if (std::holds_alternative<int>(res)) {\
            EXPECT_EQ_INT(expect, std::get<2>(res));\
        } else {\
            EXPECT_EQ_DOUBLE(expect, std::get<3>(res));\
        }\
    } while(0)

// overload pattern
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// deduction guide
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace {
void test_parse_number() {
	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
	//TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
}
}

int main() {
    EXPECT_EQ_JSON(0, "null");
    EXPECT_EQ_JSON(1, "true");
    EXPECT_EQ_JSON(1, "false");
    EXPECT_EQ_JSON(3, "23.4");
    EXPECT_EQ_JSON(2, "234");

    test_parse_number();

    //std::visit(overloaded{
    //    [](double arg) { cout << format("double is {}\n", arg); },
    //    [](int arg) { cout << format("int is {}\n", arg); },
    //    [](string arg) { cout << format("string is {}\n", arg); },
    //    [](auto arg) { print(arg); }
    //    }, Parser::FromString("[1,2,3]").inner_);
    string_view sv = "{\"tet\":[1,2,3,  6.78, false, {\"str\": true }], \"th\": false}";
    try {
        auto res = Parser::FromString(sv);
        cout << res["tet"]["str"] << endl;
    }
    catch (std::logic_error& e) {
        cout << e.what() << endl;
    }

	return main_ret;
}
