#include<iostream>
#include<fstream>
#include<locale>
#include<string>
#include<unordered_map>
#include<filesystem>
#include<sstream>
#include<iomanip>
using namespace std;

class jsonParser {
    public:
        static void parseJson(wifstream& stream, unordered_map<wstring, wstring>& map) {
            //char key[256], value[256], colon[1], comma[1];
            wstring key, value, colon, comma, line;
            if (stream.is_open()) {
                while (std::getline(stream, line)) {
                    wistringstream iss(line);
                    iss >> std::quoted(key) >> colon >> std::quoted(value) >> comma;
                    map.insert({key, value});
                }
            }
        }
        static void parseFile(string file) {
            if (jsonParser::isJSON(file) == 0) {
                std::unordered_map<wstring, wstring> stringMap;
                std::locale::global(std::locale(""));
                std::wifstream readFile(file);
                wchar_t ch;
                if (readFile.is_open()) {
                    wstring key, value, colon, comma;
                    while (readFile.get(ch)) {
                        if (ch == '{') {
                            parseJson(readFile, stringMap);
                        } else if (ch == '}') {
                            break;
                        }
                    }
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
    return 0;
}
