#include<iostream>
#include<fstream>
#include<locale>
#include<string>
#include<filesystem>
using namespace std;

// has matching sets of "{}"
static int isJSON(string file) {
    if (std::filesystem::is_empty(file)) {
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
                return 1;
            }
        }
        readFile.close();
    }
    return 0;
}
int main() {
    int validIsJson = isJSON("tests/step1/valid.json");
    int invalidIsJson = isJSON("tests/step1/invalid.json");
    cout << "Running tests ******\n[Step 1 Valid]\tShould = 0: " << validIsJson << "]\n";
    cout << "[Step 1 Invalid]\tShould = 1: " << invalidIsJson << "]\n"; 
    return 0;
}
