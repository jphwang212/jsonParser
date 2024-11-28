#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class jsonParser {
public:
    static void parseFile(string file) {
        if (jsonParser::isJSON(file) == 0) {
            typedef std::istreambuf_iterator<char> buf_iter;
            std::unordered_map<string, string> stringMap;
            std::locale::global(std::locale(""));
            std::ifstream readFile(file);
            string key, value;
            char colon;

            for (buf_iter i(readFile), e; i != e; i++) {
                std::stringstream ss, val;
                char c = *i;
                // logic for first opening curly brace
                if (ss.gcount() == 0 & c == '{') {
                    continue;
                } else if (c == '}') {
                    break;
                } else if (c == ',') {
                    bool boolVal;
                    int intVal;
                    ss >> std::quoted(key) >> colon >> std::quoted(value);
                    // have to simulate different types. c++ maps limited to values of 1 type
                    // prints value and type
                    if (ss.fail()) {
                        // try to put into int then bool
                        if (ss >> intVal) {
                            value = std::to_string(intVal);
                        } else if (ss >> value) {
                            if (!value.compare("true") || !value.compare("false")) {
                                // input boolean value
                                value = "[Boolean type]: " + value;
                            } else if (value.at(0) == '[') {
                                value = "[Array type]: " + value;
                            } else if (value.at(0) == '{') {
                                value = "[Object type]: " + value;
                            }
                        }
                    }
                    stringMap.insert({key, value});
                    // key = "";
                    // value = "";
                    // ss.str("");
                } else {
                    ss << c;
                }
            }
            for (const auto &pair : stringMap) {
                cout << pair.first << ": " << pair.second << endl;
            }
        }
    }
    // has matching sets of "{}"
    static int isJSON(string file) {
        if (std::filesystem::is_empty(file)) {
            cout << "[Invalid JSON]: " << file << " is empty\n";
            return 1;
        }
        int isJson = 0;
        std::locale::global(std::locale(""));
        std::wifstream readFile(file);
        wchar_t ch;
        if (readFile.is_open()) {
            while (readFile.get(ch)) {
                if (ch == '{') {
                    isJson++;
                } else if (ch == '}') {
                    isJson--;
                }
                if (isJson < 0) {
                    cout << "[Invalid JSON]: " << file << " is formatted incorrectly\n";
                    return 1;
                }
            }
        }
        cout << "[Valid JSON]: " << file << "\n";
        return 0;
    }
};
int main() {
    int validIsJson = jsonParser::isJSON("tests/step1/valid.json");
    int invalidIsJson = jsonParser::isJSON("tests/step1/invalid.json");
    cout << "Running tests ******\n[Step 1 Valid]\tShould = 0: " << validIsJson << "]\n";
    cout << "[Step 1 Invalid]\tShould = 1: " << invalidIsJson << "]\n";
    jsonParser::parseFile("tests/step2/valid2.json");
    jsonParser::parseFile("tests/step3/valid.json");
    jsonParser::parseFile("tests/step4/valid2.json");
    return 0;
}
