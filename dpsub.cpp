#include <bits/stdc++.h>
#include <unistd.h>

using std::vector;
using std::pair;

struct disjointSet {
    vector<int> p;
    disjointSet(int n);
    int findRoot(int x);
    void join(int x, int y);
};

disjointSet::disjointSet(int n) {
    p.resize(n);
    for (int i = 0; i < n; ++ i) {
        p[i] = i;
    }
}

int disjointSet::findRoot(int x) {
    return p[x] == x ? x : p[x] = findRoot(p[x]);
}

void disjointSet::join(int x, int y) {
    int px = findRoot(x);
    int py = findRoot(y);
    p[px] = py;
}

int totCnt;

void simulateDP(int s1, int s2, vector<int> &dp, vector<pair<int, int>> &edges) {
    for (auto &curEdge: edges) {
        if ((s1 & (1 << curEdge.first)) && (s2 & (1 << curEdge.second))) {
            dp[s1 | s2] = 0;
            usleep(1);
            ++ totCnt;
            return;
        }
        if ((s1 & (1 << curEdge.second)) && (s2 & (1 << curEdge.first))) {
            dp[s1 | s2] = 0;
            usleep(1);
            ++ totCnt;
            return;
        }
    }
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<int> dp(1 << n, -1);
    vector<vector<int>> edges(n);
    vector<pair<int, int>> allEdges;
    for (int i = 0, u, v; i < m; ++ i) {
        scanf("%d%d", &u, &v);
        edges[u].push_back(v);
        edges[v].push_back(u);
        allEdges.emplace_back(u, v);
    }
    for (int i = 0; i < n; ++ i) {
        dp[1 << i] = 0;
    }
    totCnt = 0;
    clock_t begin = clock();
    for (int state = 1; state < (1 << n); ++ state) {
        if (__builtin_popcount(state) == 1) {
            continue;
        }
        for (int sub = (state - 1) & state; sub > 0; sub = (sub - 1) & state) {
            int remain = state ^ sub;
            if (sub > remain) {
                continue;
            }
            if (dp[sub] == -1 || dp[remain] == -1) {
                continue;
            }
            simulateDP(sub, remain, dp, allEdges);
        }
    }
    printf("elapsed %f, dp entered %d\n", float(clock() - begin) / CLOCKS_PER_SEC, totCnt);
    return 0;
}