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
            string key, value, line;
            int colonIdx;
            char colon;
            std::stringstream ss;

            for (buf_iter i(readFile), e; i != e; i++) {
                char c = *i;
                // logic for first opening curly brace
                if (ss.gcount() == 0 && c == '{') {
                    continue;
                } else if (c == '}') {
                    break;
                } else {
                    ss << c;
                }
            }
            // stringstream holds content of json
            while (std::getline(ss, line, ',')) {
                colonIdx = line.find(':');
                if (colonIdx < 0) {
                    break;
                }
                key = line.substr(0, colonIdx);
                value = line.substr(colonIdx + 1, line.length() - 1);
                stringMap.insert({key, value});
            }
            // Print map
            for (const auto &pair : stringMap) {
                cout << pair.first << ": " << pair.second << "\n";
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
    jsonParser::parseFile("tests/step3/valid.json");
    return 0;
}
