#include <bits/stdc++.h>

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

bool isConnected(int nodeCnt, int edgeSet, vector<pair<int, int>> &edges) {
    int sz = edges.size();
    disjointSet djs(nodeCnt);
    for (int i = 0; edgeSet > 0; ++ i, edgeSet >>= 1) {
        if (edgeSet & 1) {
            djs.join(edges[i].first, edges[i].second);
        }
    }
    for (int i = 1; i < nodeCnt; ++ i) {
        if (djs.findRoot(0) != djs.findRoot(i)) {
            return false;
        }
    }
    return true;
}

void genAllGraph(int nodeCnt) {
    vector<pair<int, int>> allEdges;
    for (int i = 0; i < nodeCnt; ++ i) {
        for (int j = i + 1; j < nodeCnt; ++ j) {
            allEdges.emplace_back(i, j);
        }
    }
    int edgeCnt = nodeCnt * (nodeCnt - 1) / 2;
    for (int i = 1; i < (1 << edgeCnt); ++ i) {
        if (!isConnected(nodeCnt, i, allEdges)) {
            continue;
        }
        printf("%d %d\n", nodeCnt, __builtin_popcount(i));
        for (int j = 0, tmpi = i; tmpi > 0; ++j, tmpi >>= 1) {
            if (tmpi & 1) {
                printf("%d %d\n", allEdges[j].first, allEdges[j].second);
            }
        }
    }
}

int main() {
    genAllGraph(5);
    genAllGraph(6);
    return 0;
}
