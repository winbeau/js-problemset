// Generator for "INTa 语言解释器"
// Produces 20 test inputs (1.in .. 20.in) as legal INTa programs.
// All variables are guaranteed to be defined before use.
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(20240427);

static inline int randint(int l, int r) {
    uniform_int_distribution<int> d(l, r);
    return d(rng);
}

// Scope stack: each scope is a set of defined variables (chars 'a'..'z').
using Scope = set<char>;

bool isDefined(const vector<Scope>& stk, char v) {
    for (const auto& s : stk)
        if (s.count(v)) return true;
    return false;
}

vector<char> collectDefined(const vector<Scope>& stk) {
    set<char> all;
    for (const auto& s : stk)
        for (char c : s) all.insert(c);
    return vector<char>(all.begin(), all.end());
}

// Generate an expression string with `ops` operands (1..10).
// Variables used must come from `defined` (non-empty if we use vars).
// Returns expression string (no spaces, operators +-*/&^, constants 0..999).
string genExp(int ops, const vector<char>& defined) {
    string s;
    auto genOperand = [&]() -> string {
        // 50/50 variable vs constant, but fallback to constant if none defined
        if (!defined.empty() && randint(0, 1) == 0) {
            char c = defined[randint(0, (int)defined.size() - 1)];
            return string(1, c);
        } else {
            int v = randint(0, 999);
            return to_string(v);
        }
    };
    const char* OPS = "+-*/&^";
    s += genOperand();
    for (int i = 1; i < ops; ++i) {
        char op = OPS[randint(0, 5)];
        s += op;
        s += genOperand();
    }
    return s;
}

// Pick a variable name for a new INTa line or for assignment target.
// For INTa: ideally a fresh letter in the current (top) scope.
// For assignment: could be any letter; if defined anywhere, modifies nearest; if not, defines in current.
char pickLetter() {
    return char('a' + randint(0, 25));
}

// Produce a fresh letter not in current top scope (if possible).
// If all 26 are taken in top scope, returns 0.
char pickFreshForTop(const Scope& top) {
    vector<char> cand;
    for (char c = 'a'; c <= 'z'; ++c)
        if (!top.count(c)) cand.push_back(c);
    if (cand.empty()) return 0;
    return cand[randint(0, (int)cand.size() - 1)];
}

// Generate a random program into `lines`.
// Parameters control size/shape.
struct GenConfig {
    int targetLines;      // desired number of lines (approx upper bound)
    int maxIfDepth;       // maximum nesting of IF
    int maxExpOps;        // max number of operands in an expression
    int minExpOps;        // min number of operands (usually 1)
    int printBias;        // higher -> more PRINT lines (percentage-ish)
    int ifBias;           // higher -> more IFs
    int assignBias;       // higher -> more assignments
    int declBias;         // higher -> more INTa declarations
};

void genProgram(vector<string>& lines, const GenConfig& cfg) {
    vector<Scope> stk;
    stk.push_back(Scope());
    int depth = 0; // current IF nesting depth (stk.size()-1)

    while ((int)lines.size() < cfg.targetLines) {
        int remaining = cfg.targetLines - (int)lines.size();
        // Must close remaining IFs before end: need `depth` ENDIFs at the tail.
        if (remaining <= depth) {
            // force ENDIFs to fit
            stk.pop_back();
            depth--;
            lines.push_back("ENDIF");
            continue;
        }

        // Choose an action.
        int total = cfg.printBias + cfg.ifBias + cfg.assignBias + cfg.declBias + (depth > 0 ? 10 : 0);
        int r = randint(1, total);
        int action; // 0=decl,1=assign,2=print,3=if,4=endif
        if (r <= cfg.declBias) action = 0;
        else if (r <= cfg.declBias + cfg.assignBias) action = 1;
        else if (r <= cfg.declBias + cfg.assignBias + cfg.printBias) action = 2;
        else if (r <= cfg.declBias + cfg.assignBias + cfg.printBias + cfg.ifBias) action = 3;
        else action = 4;

        // Rule overrides
        vector<char> defined = collectDefined(stk);
        if (action == 1 && defined.empty()) action = 0;      // no var to assign-modify meaningfully? still can define
        if (action == 2 && defined.empty()) action = 0;      // can't print meaningful expr -> use constants ok, but prefer decl
        if (action == 3) {
            if (depth >= cfg.maxIfDepth) action = (defined.empty() ? 0 : (randint(0, 1) ? 1 : 2));
            if (remaining < 2) action = (defined.empty() ? 0 : 2); // need room for ENDIF
        }
        if (action == 4 && depth == 0) action = 0;

        if (action == 0) {
            // INTa: define new variable in current top scope.
            char v = pickFreshForTop(stk.back());
            if (v == 0) {
                // top scope already has all 26 letters; fall back to assignment
                if (!defined.empty()) {
                    char tv = defined[randint(0, (int)defined.size() - 1)];
                    int ops = randint(cfg.minExpOps, cfg.maxExpOps);
                    string e = genExp(ops, defined);
                    lines.push_back(string(1, tv) + "=" + e);
                } else {
                    // impossible branch, just print a constant
                    lines.push_back("PRINT0");
                }
            } else {
                stk.back().insert(v);
                lines.push_back(string("INT") + v);
            }
        } else if (action == 1) {
            // Assignment. Target: either existing variable (modifies nearest)
            // or new one to define in current scope.
            char tv;
            if (!defined.empty() && randint(0, 3) != 0) {
                tv = defined[randint(0, (int)defined.size() - 1)];
            } else {
                char fresh = pickFreshForTop(stk.back());
                if (fresh == 0) {
                    if (defined.empty()) {
                        // Shouldn't happen; just declare something
                        lines.push_back("INTa");
                        if (!stk.back().count('a')) stk.back().insert('a');
                        continue;
                    }
                    tv = defined[randint(0, (int)defined.size() - 1)];
                } else {
                    tv = fresh;
                    stk.back().insert(tv);
                }
            }
            // Re-collect defined (tv is now defined either way) for RHS usage.
            defined = collectDefined(stk);
            int ops = randint(cfg.minExpOps, cfg.maxExpOps);
            string e = genExp(ops, defined);
            lines.push_back(string(1, tv) + "=" + e);
        } else if (action == 2) {
            // PRINT expression. If no variables defined, use only constants.
            int ops = randint(cfg.minExpOps, cfg.maxExpOps);
            string e = genExp(ops, defined);
            lines.push_back("PRINT" + e);
        } else if (action == 3) {
            // IF expression
            int ops = randint(cfg.minExpOps, cfg.maxExpOps);
            string e = genExp(ops, defined);
            lines.push_back("IF " + e);
            stk.push_back(Scope());
            depth++;
        } else {
            // ENDIF
            stk.pop_back();
            depth--;
            lines.push_back("ENDIF");
        }
    }
    // Close any remaining open IFs.
    while (depth > 0) {
        stk.pop_back();
        depth--;
        lines.push_back("ENDIF");
    }
}

void writeFile(int idx, const vector<string>& lines) {
    string name = to_string(idx) + ".in";
    FILE* fp = fopen(name.c_str(), "w");
    for (const auto& s : lines) {
        fputs(s.c_str(), fp);
        fputc('\n', fp);
    }
    fclose(fp);
}

int main() {
    // 1..14: random programs of varying shape.
    for (int id = 1; id <= 14; ++id) {
        vector<string> lines;
        GenConfig cfg;
        cfg.targetLines = randint(3, 100);
        cfg.maxIfDepth = randint(0, 3);
        cfg.minExpOps = 1;
        cfg.maxExpOps = randint(1, 10);
        cfg.printBias = randint(10, 40);
        cfg.ifBias = randint(0, 20);
        cfg.assignBias = randint(15, 40);
        cfg.declBias = randint(10, 30);
        genProgram(lines, cfg);
        writeFile(id, lines);
    }

    // 15..17: stress (near 100 lines, deep IF, long expressions, many PRINTs).
    for (int id = 15; id <= 17; ++id) {
        vector<string> lines;
        GenConfig cfg;
        cfg.targetLines = 100;
        cfg.maxIfDepth = 3;
        cfg.minExpOps = 6;
        cfg.maxExpOps = 10;
        cfg.printBias = 45;
        cfg.ifBias = 25;
        cfg.assignBias = 25;
        cfg.declBias = 15;
        genProgram(lines, cfg);
        // Trim to 100 lines just in case.
        if ((int)lines.size() > 100) lines.resize(100);
        writeFile(id, lines);
    }

    // 18: only INTa + PRINTa.
    {
        vector<string> lines;
        lines.push_back("INTa");
        lines.push_back("PRINTa");
        writeFile(18, lines);
    }

    // 19: "empty" program = single INTa line.
    {
        vector<string> lines;
        lines.push_back("INTa");
        writeFile(19, lines);
    }

    // 20: IF 0 ... ENDIF with nested IF that never enters.
    {
        vector<string> lines;
        lines.push_back("INTa");
        lines.push_back("INTb");
        lines.push_back("IF 0");
        lines.push_back("INTc");
        lines.push_back("c=1+2+3");
        lines.push_back("PRINTc");
        lines.push_back("IF 0");
        lines.push_back("INTd");
        lines.push_back("d=999*888");
        lines.push_back("PRINTd+c");
        lines.push_back("IF 0");
        lines.push_back("PRINTd^c&b");
        lines.push_back("ENDIF");
        lines.push_back("ENDIF");
        lines.push_back("ENDIF");
        lines.push_back("PRINTa+b");
        writeFile(20, lines);
    }

    return 0;
}
