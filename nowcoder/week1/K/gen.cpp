#include <bits/stdc++.h>
using namespace std;

static mt19937 rng(20240427);

// Compute max Q such that N^Q <= 3000
int maxQ(int N) {
    int q = 0;
    long long p = 1;
    while (p * N <= 3000) {
        p *= N;
        ++q;
    }
    return q; // Q >= 1 guaranteed because N>=3, so N^1=3..5 <=3000
}

// Write a single test group to ofs.
// tmpl has exactly N rows each of length N.
void writeGroup(ofstream &ofs, int N, const vector<string> &tmpl, int Q) {
    ofs << N << '\n';
    for (int i = 0; i < N; ++i) {
        // Ensure length is exactly N
        string line = tmpl[i];
        if ((int)line.size() < N) line.append(N - (int)line.size(), ' ');
        else if ((int)line.size() > N) line = line.substr(0, N);
        ofs << line << '\n';
    }
    ofs << Q << '\n';
}

// Generate a random template: pick one non-space char from set,
// randomly scatter that char into N*N grid (at least one non-space if possible).
vector<string> randomTemplate(int N, char ch) {
    vector<string> g(N, string(N, ' '));
    // probability for each cell to be filled: uniform in [0.3, 0.8]
    uniform_real_distribution<double> pr(0.25, 0.8);
    double p = pr(rng);
    bool anyFilled = false;
    uniform_real_distribution<double> u(0.0, 1.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (u(rng) < p) {
                g[i][j] = ch;
                anyFilled = true;
            }
        }
    }
    if (!anyFilled) {
        // force at least one filled cell
        uniform_int_distribution<int> di(0, N - 1);
        g[di(rng)][di(rng)] = ch;
    }
    return g;
}

void genRandomFile(const string &path) {
    ofstream ofs(path);
    uniform_int_distribution<int> groupDist(1, 6);
    int groups = groupDist(rng);
    const string charset = "#@*O+X";
    uniform_int_distribution<int> nDist(0, 2); // maps to 3,4,5
    uniform_int_distribution<int> cDist(0, (int)charset.size() - 1);
    for (int g = 0; g < groups; ++g) {
        int N = 3 + nDist(rng);
        int qmax = maxQ(N);
        uniform_int_distribution<int> qDist(1, qmax);
        int Q = qDist(rng);
        char ch = charset[cDist(rng)];
        vector<string> tmpl = randomTemplate(N, ch);
        writeGroup(ofs, N, tmpl, Q);
    }
    ofs << 0 << '\n';
}

void genStressFile(const string &path) {
    ofstream ofs(path);
    // Group 1: N=3 Q=7 all '#'
    {
        int N = 3;
        vector<string> tmpl(N, string(N, '#'));
        writeGroup(ofs, N, tmpl, 7);
    }
    // Group 2: N=4 Q=5 diagonal '@'
    {
        int N = 4;
        vector<string> tmpl(N, string(N, ' '));
        for (int i = 0; i < N; ++i) tmpl[i][i] = '@';
        writeGroup(ofs, N, tmpl, 5);
    }
    // Group 3: N=5 Q=4 border '*'
    {
        int N = 5;
        vector<string> tmpl(N, string(N, ' '));
        for (int j = 0; j < N; ++j) {
            tmpl[0][j] = '*';
            tmpl[N - 1][j] = '*';
        }
        for (int i = 0; i < N; ++i) {
            tmpl[i][0] = '*';
            tmpl[i][N - 1] = '*';
        }
        writeGroup(ofs, N, tmpl, 4);
    }
    ofs << 0 << '\n';
}

void genStressFileVariant(const string &path, int variant) {
    // Three stress files; slightly vary order/content while keeping the
    // three required maximum-size patterns. Keep them near the documented
    // configuration to preserve stress intent.
    ofstream ofs(path);
    auto writeFull3 = [&]() {
        int N = 3;
        vector<string> tmpl(N, string(N, '#'));
        writeGroup(ofs, N, tmpl, 7);
    };
    auto writeDiag4 = [&]() {
        int N = 4;
        vector<string> tmpl(N, string(N, ' '));
        for (int i = 0; i < N; ++i) tmpl[i][i] = '@';
        writeGroup(ofs, N, tmpl, 5);
    };
    auto writeBorder5 = [&]() {
        int N = 5;
        vector<string> tmpl(N, string(N, ' '));
        for (int j = 0; j < N; ++j) {
            tmpl[0][j] = '*';
            tmpl[N - 1][j] = '*';
        }
        for (int i = 0; i < N; ++i) {
            tmpl[i][0] = '*';
            tmpl[i][N - 1] = '*';
        }
        writeGroup(ofs, N, tmpl, 4);
    };

    if (variant == 0) {
        writeFull3();
        writeDiag4();
        writeBorder5();
    } else if (variant == 1) {
        writeBorder5();
        writeFull3();
        writeDiag4();
    } else {
        writeDiag4();
        writeBorder5();
        writeFull3();
    }
    ofs << 0 << '\n';
}

void genExtremeFile(const string &path, int kind) {
    ofstream ofs(path);
    if (kind == 0) {
        // Q=1: output template itself. Use a mid-size N=4 template with some chars.
        int N = 4;
        vector<string> tmpl(N, string(N, ' '));
        tmpl[0] = "+  +";
        tmpl[1] = " ++ ";
        tmpl[2] = " ++ ";
        tmpl[3] = "+  +";
        writeGroup(ofs, N, tmpl, 1);
        // also a small one
        int N2 = 3;
        vector<string> t2(N2, string(N2, ' '));
        t2[0] = "X X";
        t2[1] = " X ";
        t2[2] = "X X";
        writeGroup(ofs, N2, t2, 1);
    } else if (kind == 1) {
        // All-space template: output should be entirely blank.
        int N = 5;
        vector<string> tmpl(N, string(N, ' '));
        writeGroup(ofs, N, tmpl, 4);
        // Another group: N=3, Q=7 all blank (max size all blank)
        int N2 = 3;
        vector<string> t2(N2, string(N2, ' '));
        writeGroup(ofs, N2, t2, 7);
    } else {
        // Template with only the center character set (very sparse answer).
        int N = 3;
        vector<string> tmpl(N, string(N, ' '));
        tmpl[1][1] = 'O';
        writeGroup(ofs, N, tmpl, 7); // 3^7 = 2187, very sparse
        // Another: N=5 center only, Q=4 -> 625
        int N2 = 5;
        vector<string> t2(N2, string(N2, ' '));
        t2[2][2] = '#';
        writeGroup(ofs, N2, t2, 4);
        // Another: N=4 center-ish, Q=5 -> 1024
        int N3 = 4;
        vector<string> t3(N3, string(N3, ' '));
        t3[1][1] = '@';
        writeGroup(ofs, N3, t3, 5);
    }
    ofs << 0 << '\n';
}

int main() {
    // Files 1..14: random
    for (int i = 1; i <= 14; ++i) {
        string path = to_string(i) + ".in";
        genRandomFile(path);
    }
    // Files 15..17: stress
    for (int i = 15; i <= 17; ++i) {
        string path = to_string(i) + ".in";
        genStressFileVariant(path, i - 15);
    }
    // Files 18..20: extreme
    for (int i = 18; i <= 20; ++i) {
        string path = to_string(i) + ".in";
        genExtremeFile(path, i - 18);
    }
    return 0;
}
