#include<iostream>
#include<fstream>
#include<locale>
#include<string>
#include<unordered_map>
#include<vector>
#include<filesystem>
#include<sstream>
#include<iomanip>
using namespace std;

class jsonParser {
    public:
        static void parseFile(string file) {
            if (jsonParser::isJSON(file) == 0) {
                typedef std::istreambuf_iterator<char> buf_iter;
                std::unordered_map<string, string> stringMap;
                std::locale::global(std::locale(""));
                std::ifstream readFile(file);
                std::stringstream ss, val;
                string key, value, colon;
                bool boolVal;
                int intVal;

                for (buf_iter i(readFile), e; i!= e; i++) {
                    char c = *i;
                    if (c == '{') {
                        continue;
                    } else if (c == '}') {
                        break;
                    } else if (c == ',') {
                        ss >> std::quoted(key) >> colon >> std::quoted(value);
                        // have to simulate different types. c++ maps limited to values of 1 type
                        // prints value and type
                        if (ss.fail()) {
                            // try bool and convert to string for map
                            ss >> boolVal;
                            value = boolVal ? "true" : "false";
                        } else if (ss.fail()) {
                            // try int and convert to string for map
                            ss >> intVal;
                            value = std::to_string(intVal);
                        } else if (ss.fail() & (ss.peek() == '[')) {
                            // check if array type
                            value = "[Array type]: ";
                            char elem;
                            while (ss >> elem) {
                                if (elem == ',') {
                                    break;
                                }
                                value += elem;
                            }
                        } else if (ss.fail() & (ss.peek() == '{')) {
                            value = "[Object type]: ";
                            char elem;
                            while (ss >> elem) {
                                value += elem;
                                if (elem == '}') {
                                    break;
                                }

                            }
                        }
                        stringMap.insert({key, value});
                        key = "";
                        value = "";
                        ss.str("");
                    } else {
                        ss << c;
                    }
                }
                for (const auto& pair : stringMap) {
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
    return 0;
}
