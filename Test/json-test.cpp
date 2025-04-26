#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace std;

int main()
{
    json j = json::parse(R"(
	  {
		"pi": 3.141,
		"happy": true
	  }
	)");
	cout << j["pi"] << endl;
	j["pi"] = "string";
	cout << j["pi"] << endl;



	return 0;
}