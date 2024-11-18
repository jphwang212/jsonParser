#include<iostream>
#include<fstream>
#include<locale>
#include<string>
using namespace std;

// has matching sets of "{}"
static int isJSON(string file) {
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
    }
    if ((readFile.rdstate() & ifstream::failbit) != 0) {
        return 1;
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