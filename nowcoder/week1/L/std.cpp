#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

static map<char, double> vars;
static string expr_str;
static size_t pos;

double parseExpr();

double parseFactor() {
    if (pos < expr_str.size() && expr_str[pos] == '(') {
        pos++; // consume '('
        double v = parseExpr();
        if (pos < expr_str.size() && expr_str[pos] == ')') pos++;
        return v;
    }
    if (pos < expr_str.size() && isdigit((unsigned char)expr_str[pos])) {
        double v = 0;
        while (pos < expr_str.size() && isdigit((unsigned char)expr_str[pos])) {
            v = v * 10 + (expr_str[pos] - '0');
            pos++;
        }
        return v;
    }
    // variable: single lowercase letter
    if (pos < expr_str.size() && islower((unsigned char)expr_str[pos])) {
        char c = expr_str[pos++];
        return vars[c];
    }
    return 0;
}

double parseTerm() {
    double v = parseFactor();
    while (pos < expr_str.size() && (expr_str[pos] == '*' || expr_str[pos] == '/')) {
        char op = expr_str[pos++];
        double r = parseFactor();
        if (op == '*') v = v * r;
        else v = v / r;
    }
    return v;
}

double parseExpr() {
    double v = parseTerm();
    while (pos < expr_str.size() && (expr_str[pos] == '+' || expr_str[pos] == '-')) {
        char op = expr_str[pos++];
        double r = parseTerm();
        if (op == '+') v = v + r;
        else v = v - r;
    }
    return v;
}

double evaluate(const string& s) {
    expr_str = s;
    pos = 0;
    return parseExpr();
}

static string trim(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b-1])) b--;
    return s.substr(a, b - a);
}

int main() {
    string line;
    while (getline(cin, line)) {
        string t = trim(line);
        if (t.empty()) continue;
        if (t == "exit") break;

        if (t.size() >= 5 && t.substr(0, 5) == "read ") {
            // collect variables
            vector<char> names;
            {
                stringstream ss(t.substr(5));
                string tok;
                while (ss >> tok) {
                    if (!tok.empty()) names.push_back(tok[0]);
                }
            }
            // next line: integers
            string dataLine;
            if (!getline(cin, dataLine)) break;
            stringstream ds(dataLine);
            for (size_t i = 0; i < names.size(); ++i) {
                long long x = 0;
                ds >> x;
                vars[names[i]] = (double)x;
            }
        } else if (t.size() >= 6 && t.substr(0, 6) == "print ") {
            vector<char> names;
            {
                stringstream ss(t.substr(6));
                string tok;
                while (ss >> tok) {
                    if (!tok.empty()) names.push_back(tok[0]);
                }
            }
            for (size_t i = 0; i < names.size(); ++i) {
                if (i) printf(" ");
                printf("%.2f", vars[names[i]]);
            }
            printf("\n");
        } else {
            // assignment: X=EXPR
            // find '='
            size_t eq = t.find('=');
            if (eq == string::npos) continue;
            char var = t[0];
            string expr = t.substr(eq + 1);
            double v = evaluate(expr);
            vars[var] = v;
        }
    }
    return 0;
}
