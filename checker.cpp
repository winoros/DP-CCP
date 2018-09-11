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

bool isConnected(int nodeSet, vector<int> &nodes, vector<vector<int>> &edges) {
    disjointSet djs(*std::max_element(nodes.begin(), nodes.end()) + 1);
    for (auto &now: nodes) {
        for (auto &next: edges[now]) {
            if (nodeSet & (1 << next)) {
                djs.join(now, next);
            }
        }
    }
    for (int i = 1; i < nodes.size(); ++ i) {
        if (djs.findRoot(nodes[0]) != djs.findRoot(nodes[i])) {
            return false;
        }
    }
    return true;
}

vector<int> mask2vector(int nodeSet) {
    vector<int> ret;
    for (int i = 0; nodeSet > 0; ++ i, nodeSet >>= 1) {
        if (nodeSet & 1) {
            ret.push_back(i);
        }
    }
    return ret;
}

bool isConnected(int nodeSet, vector<vector<int>> &edges) {
    vector<int> nodes = mask2vector(nodeSet);
    return isConnected(nodeSet, nodes, edges);
}

bool isConnected(vector<int> &nodes, vector<vector<int>> &edges) {
    int nodeSet = 0;
    for (auto &x : nodes) {
        nodeSet |= (1 << x);
    }
    return isConnected(nodeSet, nodes, edges);
}

std::string setToString(int nodeSet) {
    std::string ans;
    for (int i = 0; nodeSet > 0; ++ i, nodeSet >>= 1) {
        if (nodeSet & 1) {
            ans += std::to_string(i);
            if (nodeSet > 1) {
                ans += ' ';
            }
        }
    }
    return ans;
}

vector<pair<int, int>> generateAllValidSet(int n, vector<vector<int>> &edges) {
    vector<pair<int, int>> pairs;
    for (int i = 1; i < (1 << n) - 1; ++ i) {
        if (!isConnected(i, edges)) {
            continue;
        }
        for (int j = i + 1; j < (1 << n) - 1; ++ j) {
            if (!isConnected(j, edges)) {
                continue;
            }
            if (i & j) {
                continue;
            }
            if (!isConnected(i | j, edges)) {
                continue;
            }
            // printf("(%s, %s)\n", setToString(i).c_str(), setToString(j).c_str());
            pairs.emplace_back(i, j);
        }
    }
    return pairs;
}

int main() {
	FILE *in = fopen("in", "r");
	FILE *out = fopen("out", "r");
	int n, m, caseCnt = 0;
	vector<vector<int>> edges;
	while (fscanf(in, "%d %d", &n, &m) != EOF) {
		edges.resize(n);
		for (auto &x : edges) {
		    x.clear();
		}
		for (int i = 0, u, v; i < m; ++ i) {
		    fscanf(in, "%d %d", &u, &v);
		    edges[u].push_back(v);
		    edges[v].push_back(u);
		}
		vector<bool> visited(1 << n);
		vector<pair<int, int>> correctPairs = generateAllValidSet(n, edges);
		vector<pair<int, int>> pairsToCheck;
		pairsToCheck.reserve(correctPairs.size());
		while(true) {
		    int cnt1, cnt2;
		    fscanf(out, "%d", &cnt1);
		    if (cnt1 == 0) {
		        break;
		    }
		    vector<int> set1(cnt1);
		    int mask1(0);
		    for (int i = 0; i < cnt1; ++ i) {
		        fscanf(out, "%d", &set1[i]);
		        mask1 |= (1 << set1[i]);
		    }
		    if (!isConnected(set1, edges)) {
		        printf("Failed at #%d, sub graph not connected\n", caseCnt);
		        return -1;
		    }
		    fscanf(out, "%d", &cnt2);
		    vector<int> set2(cnt2);
		    int mask2(0);
		    for (int i = 0; i < cnt2; ++ i) {
		        fscanf(out, "%d", &set2[i]);
		        mask2 |= (1 << set2[i]);
		    }
		    if (!isConnected(set2, edges)) {
                printf("Failed at #%d, sub graph not connected\n", caseCnt);
                return -1;
		    }
		    if (mask1 & mask2) {
		        printf("Failed at #%d, two part is intersecting\n", caseCnt);
		        return -1;
		    }
		    if (visited[mask1 | mask2]) {
		        printf("Failed at #%d, visited before dp it\n", caseCnt);
		    }
		    visited[mask1] = true;
		    visited[mask2] = true;
		    if (mask1 > mask2) {
		        std::swap(mask1, mask2);
		    };
		    pairsToCheck.emplace_back(mask1, mask2);
		}
		if (correctPairs.size() != pairsToCheck.size()) {
		    printf("Failed at #%d, the total size is not correct, should be %d\n", caseCnt, correctPairs.size());
		    return -1;
		}
		std::sort(correctPairs.begin(), correctPairs.end());
		std::sort(pairsToCheck.begin(), pairsToCheck.end());
		for (int i = 0; i < correctPairs.size(); ++ i) {
		    if (correctPairs[i].first != pairsToCheck[i].first || correctPairs[i].second != pairsToCheck[i].second) {
		        printf("Failed at #%d, invalid pair exists: %s, %s\n", caseCnt, setToString(pairsToCheck[i].first).c_str(), setToString(pairsToCheck[i].second).c_str());
		        return -1;
		    }
		}
		++ caseCnt;
	}
	return 0;
}
