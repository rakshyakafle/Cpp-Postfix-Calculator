#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// --- Reuse your existing helper functions here ---
int getVal(char c) {
    if (c == 'M' || c == 'm') return 1000;
    if (c == 'D' || c == 'd') return 500;
    if (c == 'C' || c == 'c') return 100;
    if (c == 'L' || c == 'l') return 50;
    if (c == 'X' || c == 'x') return 10;
    if (c == 'V' || c == 'v') return 5;
    if (c == 'I' || c == 'i') return 1;
    return 0;
}

bool isRoman(string s) {
    if (s.empty()) return false;
    for (char c : s) if (getVal(c) == 0) return false;
    return true;
}

int romanToDecimal(string s) {
    int total = 0, lastVal = 5000;
    for (int i = 0; i < s.length(); ) {
        int v1 = getVal(s[i]);
        int v2 = (i + 1 < s.length()) ? getVal(s[i + 1]) : 0;
        int chunk = (v1 >= v2) ? v1 : v2 - v1;
        if (chunk > lastVal) return -1;
        total += chunk;
        lastVal = chunk;
        i += (v1 >= v2) ? 1 : 2;
    }
    return total;
}

bool isDecimal(string s) {
    if (s.empty()) return false;
    int start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (start == s.length()) return false;
    for (int i = start; i < s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

int main() {
    string fileName = "Postfix_3.txt"; // Use the specific quiz file name
    ifstream inFile(fileName);
    if (!inFile) { cout << "File not found!"; return 1; }

    string line;
    long long grandTotal = 0;
    int validExpressions = 0;
    int invalidExpressions = 0;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<int> stack;
        bool possible = true;

        while (ss >> token) {
            // 1. Check if Operator
            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%") {
                if (stack.size() < 2) { possible = false; break; }
                int v2 = stack.back(); stack.pop_back();
                int v1 = stack.back(); stack.pop_back();
                if (token == "+") stack.push_back(v1 + v2);
                else if (token == "-") stack.push_back(v1 - v2);
                else if (token == "*") stack.push_back(v1 * v2);
                else if (token == "/") stack.push_back(v1 / v2);
                else if (token == "%") stack.push_back(v1 % v2);
            }
            // 2. Check if Decimal
            else if (isDecimal(token)) {
                stack.push_back(stoi(token));
            }
            // 3. Check if Roman
            else if (isRoman(token)) {
                int val = romanToDecimal(token);
                if (val == -1) { possible = false; break; }
                stack.push_back(val);
            }
            // 4. Unknown entry
            else {
                possible = false;
                break;
            }
        }

        // Final Validation: Stack must have exactly 1 item left
        if (possible && stack.size() == 1) {
            grandTotal += stack.back();
            validExpressions++;
        }
        else {
            invalidExpressions++;
        }
    }

    cout << "Sum of valid results: " << grandTotal << endl;
    cout << "Valid expressions: " << validExpressions << endl;
    cout << "Invalid expressions: " << invalidExpressions << endl;

    return 0;
}