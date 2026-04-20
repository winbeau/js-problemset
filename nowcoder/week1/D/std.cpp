#include <bits/stdc++.h>
using namespace std;

int parseTime(const string &s) {
    // format HH:MM
    int h = (s[0] - '0') * 10 + (s[1] - '0');
    int m = (s[3] - '0') * 10 + (s[4] - '0');
    return h * 60 + m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    long long total = 0;

    const int LUNCH_S = 12 * 60;
    const int LUNCH_E = 13 * 60;
    const int DINNER_S = 17 * 60;
    const int DINNER_E = 18 * 60;

    for (int t = 0; t < T; ++t) {
        int days, firstDow;
        cin >> days >> firstDow;

        int dayMinutes[8] = {0};
        for (int d = 1; d <= 7; ++d) {
            string a, b;
            cin >> a >> b;
            int arrive = parseTime(a);
            int leave = parseTime(b);
            int study = leave - arrive;
            // subtract lunch overlap
            study -= max(0, min(leave, LUNCH_E) - max(arrive, LUNCH_S));
            // subtract dinner overlap
            study -= max(0, min(leave, DINNER_E) - max(arrive, DINNER_S));
            if (study < 0) study = 0;
            dayMinutes[d] = study;
        }

        for (int i = 1; i <= days; ++i) {
            int dow = ((firstDow - 1 + i - 1) % 7) + 1;
            total += dayMinutes[dow];
        }
    }

    cout << total << "\n";
    return 0;
}
