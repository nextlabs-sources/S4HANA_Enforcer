#include "json/json.h"
#include <iostream>
/** \brief Write a Value object to a string.
 * Example Usage:
 * $g++ stringWrite.cpp -ljsoncpp -std=c++11 -o stringWrite
 * $./stringWrite
 * {
 *     "action" : "run",
 *     "data" :
 *     {
 *         "number" : 1
 *     }
 * }
 */
int main() {
  Json::Value root;
  Json::Value data;
  constexpr bool shouldUseOldWay = false;
  root["action"] = "run";
  data["number"] = 1;
  root["data"] = data;

  if (shouldUseOldWay) {
    Json::FastWriter writer;
    const std::string json_file = writer.write(root);
    std::cout << json_file << std::endl;
  } else {
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, root);
    std::cout << json_file << std::endl;
  }

  Json::Value * pjsv = NULL;

    {
        Json::Value * pv = new Json::Value;

        (*pv) = root;
        (*pv)["component"] = "comp_value";
        pjsv = pv;
        Json::Value arr;
        Json::Value temp("str1");
        arr.append(temp);
        Json::Value temp2("str2");
        arr.append(temp2);
        (*pjsv)["sub"] = arr;


    }
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, *pjsv);
    std::cout << json_file << std::endl;













  return EXIT_SUCCESS;
}
