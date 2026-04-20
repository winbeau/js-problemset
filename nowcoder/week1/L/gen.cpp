// gen.cpp - generator for 模拟编译系统
// Produces 1.in .. 20.in
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(20240427);

static int randint(int l, int r) {
    uniform_int_distribution<int> d(l, r);
    return d(rng);
}

// Set of currently-assigned (known) variables
struct Env {
    set<char> assigned;
    char pickAssigned() {
        vector<char> v(assigned.begin(), assigned.end());
        return v[randint(0, (int)v.size() - 1)];
    }
};

// Generate expression string within a length budget.
// Division denominators are always positive integer constants 1..9.
// depth: remaining recursion depth (0 => leaf).
static string genExpr(Env &env, int depth, int budget);

static string genLeaf(Env &env) {
    // Either a variable from assigned set (if any) or a small positive integer.
    if (!env.assigned.empty() && randint(0, 1) == 0) {
        char c = env.pickAssigned();
        return string(1, c);
    } else {
        int v = randint(0, 99);
        return to_string(v);
    }
}

static string genExpr(Env &env, int depth, int budget) {
    if (depth <= 0 || budget < 6) {
        return genLeaf(env);
    }
    // Randomly decide structure
    int r = randint(0, 4);
    if (r == 0) {
        // parenthesized sub-expression
        string inner = genExpr(env, depth - 1, budget - 2);
        return "(" + inner + ")";
    } else {
        // binary op
        char ops[4] = {'+', '-', '*', '/'};
        char op = ops[randint(0, 3)];
        string left = genExpr(env, depth - 1, budget / 2);
        string right;
        if (op == '/') {
            // denominator must be a positive constant 1..9
            int v = randint(1, 9);
            right = to_string(v);
        } else {
            right = genExpr(env, depth - 1, budget / 2);
        }
        string s = left + string(1, op) + right;
        if ((int)s.size() > budget) {
            return genLeaf(env);
        }
        return s;
    }
}

// Generate a long expression with approximate target length, respecting max 200 chars per line.
// The assignment line = "x=" + expr ; must fit in 200 chars total.
static string genLongExpr(Env &env, int targetLen) {
    // Build chain like leaf op leaf op leaf ... with occasional parens
    int maxExpr = 195; // leave room for "x=" and safety
    if (targetLen > maxExpr) targetLen = maxExpr;
    string s = genLeaf(env);
    while ((int)s.size() < targetLen) {
        char ops[4] = {'+', '-', '*', '/'};
        char op = ops[randint(0, 3)];
        string right;
        if (op == '/') right = to_string(randint(1, 9));
        else right = genLeaf(env);
        string candidate;
        if (randint(0, 2) == 0) {
            candidate = "(" + s + ")" + op + right;
        } else {
            candidate = s + op + right;
        }
        if ((int)candidate.size() > maxExpr) break;
        s = candidate;
    }
    return s;
}

// Generate a deeply-nested parenthesized expression like ((((a+b)*c)-d)*e)
static string genDeepNested(Env &env) {
    // Ensure at least 2 assigned vars
    vector<char> vs(env.assigned.begin(), env.assigned.end());
    if (vs.size() < 2) return genLeaf(env);
    // Use variables we have; cycle if needed
    // pattern: start with (v1 op v2), then wrap ((prev op v3)), ...
    shuffle(vs.begin(), vs.end(), rng);
    char ops[4] = {'+', '-', '*', '+'}; // avoid '/' to keep safe
    string s = "(" + string(1, vs[0]) + "+" + string(1, vs[1]) + ")";
    int n = (int)vs.size();
    int idx = 2;
    int layers = randint(3, 6);
    for (int i = 0; i < layers; ++i) {
        char v = vs[idx % n];
        idx++;
        char op = ops[randint(0, 3)];
        s = "(" + s + string(1, op) + string(1, v) + ")";
        if ((int)s.size() > 190) break;
    }
    return s;
}

static void writeProgram(const string &path, const vector<string> &lines) {
    FILE *f = fopen(path.c_str(), "w");
    for (auto &l : lines) {
        fputs(l.c_str(), f);
        fputc('\n', f);
    }
    fclose(f);
}

// Build a random program: lines include read/assign/print, end with exit.
static vector<string> buildRandomProgram(int totalLines, int exprDepthMax) {
    Env env;
    vector<string> lines;

    // Always start with one read that assigns 1..3 vars
    {
        int k = randint(1, 3);
        set<char> chosen;
        while ((int)chosen.size() < k) {
            char c = (char)('a' + randint(0, 25));
            chosen.insert(c);
        }
        string readLine = "read";
        string dataLine;
        bool first = true;
        for (char c : chosen) {
            readLine += " ";
            readLine += c;
            if (!first) dataLine += " ";
            dataLine += to_string(randint(1, 100));
            first = false;
            env.assigned.insert(c);
        }
        lines.push_back(readLine);
        lines.push_back(dataLine);
    }

    // We target roughly totalLines statements (not counting data lines).
    // Track statement count.
    int stmtCount = 1;
    while (stmtCount < totalLines - 1) {
        int choice = randint(0, 9);
        if (choice < 2) {
            // read
            int k = randint(1, 3);
            set<char> chosen;
            int tries = 0;
            while ((int)chosen.size() < k && tries < 20) {
                char c = (char)('a' + randint(0, 25));
                chosen.insert(c);
                tries++;
            }
            string readLine = "read";
            string dataLine;
            bool first = true;
            for (char c : chosen) {
                readLine += " ";
                readLine += c;
                if (!first) dataLine += " ";
                dataLine += to_string(randint(1, 100));
                first = false;
                env.assigned.insert(c);
            }
            lines.push_back(readLine);
            lines.push_back(dataLine);
            stmtCount++;
        } else if (choice < 7) {
            // assignment
            char lhs = (char)('a' + randint(0, 25));
            int depth = randint(1, exprDepthMax);
            string expr = genExpr(env, depth, 180);
            string line = string(1, lhs) + "=" + expr;
            if ((int)line.size() > 200) {
                // fallback to simple
                line = string(1, lhs) + "=" + genLeaf(env);
            }
            lines.push_back(line);
            env.assigned.insert(lhs);
            stmtCount++;
        } else {
            // print
            if (env.assigned.empty()) continue;
            int k = randint(1, min((int)env.assigned.size(), 4));
            vector<char> pool(env.assigned.begin(), env.assigned.end());
            shuffle(pool.begin(), pool.end(), rng);
            string printLine = "print";
            for (int i = 0; i < k; ++i) {
                printLine += " ";
                printLine += pool[i];
            }
            lines.push_back(printLine);
            stmtCount++;
        }
    }

    // Ensure there's at least one print before exit
    bool hasPrint = false;
    for (auto &l : lines) {
        if (l.rfind("print", 0) == 0) { hasPrint = true; break; }
    }
    if (!hasPrint) {
        if (!env.assigned.empty()) {
            char c = *env.assigned.begin();
            lines.push_back(string("print ") + c);
        }
    }

    lines.push_back("exit");
    return lines;
}

// Build a heavy program: ~100 statement lines, long expressions, uses a..z.
static vector<string> buildHeavyProgram() {
    Env env;
    vector<string> lines;

    // Read all 26 variables in batches
    // Batch 1: a..i
    {
        string readLine = "read";
        string dataLine;
        for (int i = 0; i < 9; ++i) {
            readLine += " ";
            readLine += (char)('a' + i);
            if (i) dataLine += " ";
            dataLine += to_string(randint(1, 100));
            env.assigned.insert((char)('a' + i));
        }
        lines.push_back(readLine);
        lines.push_back(dataLine);
    }
    // Batch 2: j..r
    {
        string readLine = "read";
        string dataLine;
        for (int i = 9; i < 18; ++i) {
            readLine += " ";
            readLine += (char)('a' + i);
            if (i > 9) dataLine += " ";
            dataLine += to_string(randint(1, 100));
            env.assigned.insert((char)('a' + i));
        }
        lines.push_back(readLine);
        lines.push_back(dataLine);
    }
    // Batch 3: s..z
    {
        string readLine = "read";
        string dataLine;
        for (int i = 18; i < 26; ++i) {
            readLine += " ";
            readLine += (char)('a' + i);
            if (i > 18) dataLine += " ";
            dataLine += to_string(randint(1, 100));
            env.assigned.insert((char)('a' + i));
        }
        lines.push_back(readLine);
        lines.push_back(dataLine);
    }

    int stmtSoFar = 3;
    int target = 100;
    while (stmtSoFar < target - 1) {
        int choice = randint(0, 9);
        if (choice < 1) {
            // another read
            int k = randint(1, 4);
            set<char> chosen;
            int tries = 0;
            while ((int)chosen.size() < k && tries < 20) {
                char c = (char)('a' + randint(0, 25));
                chosen.insert(c);
                tries++;
            }
            string readLine = "read";
            string dataLine;
            bool first = true;
            for (char c : chosen) {
                readLine += " ";
                readLine += c;
                if (!first) dataLine += " ";
                dataLine += to_string(randint(1, 100));
                first = false;
                env.assigned.insert(c);
            }
            lines.push_back(readLine);
            lines.push_back(dataLine);
            stmtSoFar++;
        } else if (choice < 7) {
            // long assignment (~180-195 chars)
            char lhs = (char)('a' + randint(0, 25));
            int targetLen = randint(150, 195);
            string expr = genLongExpr(env, targetLen);
            string line = string(1, lhs) + "=" + expr;
            if ((int)line.size() > 200) {
                line = string(1, lhs) + "=" + genLeaf(env);
            }
            lines.push_back(line);
            env.assigned.insert(lhs);
            stmtSoFar++;
        } else {
            // print
            int k = randint(2, 6);
            vector<char> pool(env.assigned.begin(), env.assigned.end());
            shuffle(pool.begin(), pool.end(), rng);
            k = min(k, (int)pool.size());
            string printLine = "print";
            for (int i = 0; i < k; ++i) {
                printLine += " ";
                printLine += pool[i];
            }
            lines.push_back(printLine);
            stmtSoFar++;
        }
    }

    lines.push_back("exit");
    return lines;
}

int main() {
    // Cases 1..14: random, 5..50 lines, expr depth 1..3
    for (int i = 1; i <= 14; ++i) {
        int totalLines = randint(5, 50);
        int depthMax = randint(1, 3);
        auto lines = buildRandomProgram(totalLines, depthMax);
        writeProgram(to_string(i) + ".in", lines);
    }

    // Cases 15..17: heavy ~100 lines
    for (int i = 15; i <= 17; ++i) {
        auto lines = buildHeavyProgram();
        writeProgram(to_string(i) + ".in", lines);
    }

    // Case 18: minimal read a + 5 + print a + exit
    {
        vector<string> lines;
        lines.push_back("read a");
        lines.push_back("5");
        lines.push_back("print a");
        lines.push_back("exit");
        writeProgram("18.in", lines);
    }

    // Case 19: only read one variable then exit
    {
        vector<string> lines;
        lines.push_back("read x");
        lines.push_back("42");
        lines.push_back("exit");
        writeProgram("19.in", lines);
    }

    // Case 20: deeply nested assignment like ((((a+b)*c)-d)*e)
    {
        Env env;
        vector<string> lines;
        // Read a..e
        {
            string readLine = "read";
            string dataLine;
            for (int i = 0; i < 5; ++i) {
                readLine += " ";
                readLine += (char)('a' + i);
                if (i) dataLine += " ";
                dataLine += to_string(randint(1, 100));
                env.assigned.insert((char)('a' + i));
            }
            lines.push_back(readLine);
            lines.push_back(dataLine);
        }
        // Canonical deep nested expression from the spec
        lines.push_back("f=((((a+b)*c)-d)*e)");
        env.assigned.insert('f');
        // Another deeper nested expression programmatically
        {
            string deep = genDeepNested(env);
            lines.push_back(string("g=") + deep);
            env.assigned.insert('g');
        }
        lines.push_back("print a b c d e f g");
        lines.push_back("exit");
        writeProgram("20.in", lines);
    }

    return 0;
}
