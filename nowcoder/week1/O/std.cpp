#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static string trim(const string& s) {
    size_t left = 0;
    while (left < s.size() && isspace(static_cast<unsigned char>(s[left]))) {
        ++left;
    }
    size_t right = s.size();
    while (right > left && isspace(static_cast<unsigned char>(s[right - 1]))) {
        --right;
    }
    return s.substr(left, right - left);
}

static int64 readValue(const vector<unordered_map<char, int64>>& scopes, char name) {
    for (int i = static_cast<int>(scopes.size()) - 1; i >= 0; --i) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) {
            return it->second;
        }
    }
    return 0;
}

static void assignValue(vector<unordered_map<char, int64>>& scopes, char name, int64 value) {
    for (int i = static_cast<int>(scopes.size()) - 1; i >= 0; --i) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) {
            it->second = value;
            return;
        }
    }
    scopes.back()[name] = value;
}

static int64 parseOperand(const string& expr, size_t& pos, const vector<unordered_map<char, int64>>& scopes) {
    if (isdigit(static_cast<unsigned char>(expr[pos]))) {
        int64 value = 0;
        while (pos < expr.size() && isdigit(static_cast<unsigned char>(expr[pos]))) {
            value = value * 10 + (expr[pos] - '0');
            ++pos;
        }
        return value;
    }
    char name = expr[pos++];
    return readValue(scopes, name);
}

static int64 evaluate(const string& expr, const vector<unordered_map<char, int64>>& scopes) {
    size_t pos = 0;
    int64 result = parseOperand(expr, pos, scopes);
    while (pos < expr.size()) {
        char op = expr[pos++];
        int64 rhs = parseOperand(expr, pos, scopes);
        if (op == '+') {
            result += rhs;
        } else if (op == '-') {
            result -= rhs;
        } else if (op == '*') {
            result *= rhs;
        } else if (op == '/') {
            result /= rhs;
        } else if (op == '&') {
            result &= rhs;
        } else if (op == '^') {
            result ^= rhs;
        }
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> program;
    string line;
    while (getline(cin, line)) {
        string current = trim(line);
        if (!current.empty()) {
            program.push_back(current);
        }
    }

    vector<unordered_map<char, int64>> scopes(1);
    for (size_t pc = 0; pc < program.size(); ++pc) {
        const string& stmt = program[pc];
        if (stmt.rfind("INT", 0) == 0) {
            scopes.back()[stmt[3]] = 0;
            continue;
        }
        if (stmt.rfind("PRINT", 0) == 0) {
            cout << evaluate(stmt.substr(5), scopes) << '\n';
            continue;
        }
        if (stmt.rfind("IF ", 0) == 0) {
            if (evaluate(stmt.substr(3), scopes) != 0) {
                scopes.emplace_back();
            } else {
                int depth = 1;
                while (pc + 1 < program.size() && depth > 0) {
                    ++pc;
                    if (program[pc].rfind("IF ", 0) == 0) {
                        ++depth;
                    } else if (program[pc] == "ENDIF") {
                        --depth;
                    }
                }
            }
            continue;
        }
        if (stmt == "ENDIF") {
            if (scopes.size() > 1) {
                scopes.pop_back();
            }
            continue;
        }

        char name = stmt[0];
        size_t eq = stmt.find('=');
        assignValue(scopes, name, evaluate(stmt.substr(eq + 1), scopes));
    }

    return 0;
}
