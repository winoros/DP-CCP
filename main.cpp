#include <bits/stdc++.h>
#include <unistd.h>

using std::vector;
using std::pair;
using std::queue;

vector<int> now2orig, orig2now;

vector<int> mask2vector(int nodeSet) {
	vector<int> ret;
	for (int i = 0; nodeSet > 0; ++ i, nodeSet >>= 1) {
		if (nodeSet & 1) {
			ret.push_back(i);
		}
	}
	return ret;
}

int neig[1 << 15];

// get the direct neighbor list of given nodes.
int getNeighbor(int nodeSet, vector<vector<int>> &edges) {
    if (neig[nodeSet] != -1) {
        return neig[nodeSet];
    }
    int ans = 0;
    if (nodeSet > 0) {
        int firstOne = __builtin_ctz(nodeSet);
        ans = getNeighbor(nodeSet ^ (1 << firstOne), edges);
        for (auto &x : edges[now2orig[firstOne]]) {
            ans |= (1 << orig2now[x]);
        }
        ans &= ~nodeSet;
    }
    return neig[nodeSet] = ans;
}

int smallThan(int now) {
    return (1 << now) - 1;
}

void enumerateCsgRec(int curNodeSet, int excludeNodeSet, vector<vector<int>> &edges, vector<int> &sg) {
    int neig = getNeighbor(curNodeSet, edges);
    neig &= ~excludeNodeSet;
    vector<int> allSubSet;
    allSubSet.reserve(1 << __builtin_popcount(neig));
    for (int nowSet = neig; nowSet > 0; nowSet = (nowSet - 1) & neig) {
        allSubSet.push_back(nowSet);
    }
    std::reverse(allSubSet.begin(), allSubSet.end());
    // Small set first.
    for (auto &nowSet : allSubSet) {
        sg.push_back(nowSet | curNodeSet);
    }
    // Small set first.
    for (auto &nowSet : allSubSet) {
        enumerateCsgRec(curNodeSet | nowSet, excludeNodeSet | neig, edges, sg);
    }
}

void enumerateCsg(int n, vector<vector<int>> &edges, vector<int> &sg) {
	for (int i = n - 1; i >= 0; -- i) {
		sg.push_back(1 << i);
		enumerateCsgRec(1 << i, smallThan(i), edges, sg);
	}
}

void enumerateCmp(int setToBePaired, vector<vector<int>> &edges, vector<int> &paired) {
	int minOfSet = __builtin_ctz(setToBePaired);
	int setToExclude = smallThan(minOfSet) | setToBePaired;
	int N = getNeighbor(setToBePaired, edges) & (~setToExclude);
	// printf("%d %d %d %d\n", setToBePaired, minOfSet, setToExclude, N);
	vector<int> nodesOfSet = mask2vector(N);
	std::reverse(nodesOfSet.begin(), nodesOfSet.end());
	for (auto &now : nodesOfSet) {
		paired.push_back(1 << now);
		enumerateCsgRec(1 << now, setToExclude | (smallThan(now) & N), edges, paired);
	}
}

void bfsReLabel(int n, vector<vector<int>> &edges) {
	queue<int> q;
	vector<bool> visited(n);
	q.push(0);
	orig2now.clear();
	orig2now.resize(n);
	now2orig.clear();
	now2orig.resize(n);
	int pt = 0;
	while(!q.empty()) {
		int now = q.front();
		q.pop();
		if (visited[now]) {
		    continue;
		}
		visited[now] = true;
		orig2now[now] = pt, now2orig[pt] = now;
		++ pt;
		for (auto &x : edges[now]) {
			if (visited[x]) {
				continue;
			}
			q.push(x);
		}
	}
}

std::string setToString(int nodeSet) {
    std::string ans;
    for (int i = 0; nodeSet > 0; ++ i, nodeSet >>= 1) {
        if (nodeSet & 1) {
            ans += std::to_string(now2orig[i]);
            if (nodeSet > 1) {
                ans += ' ';
            }
        }
    }
    return ans;
}

void printVector(vector<int> &v) {
    for (size_t i = 0; i < v.size(); ++ i) {
        printf("%d%c", v.at(i), " \n"[i == v.size() - 1]);
    }
}

void printForCheck(int s1, int s2) {
    std::cout << __builtin_popcount(s1) << " " << setToString(s1) << "\n";
    std::cout << __builtin_popcount(s2) << " " << setToString(s2) << "\n";
}

void simulateDPCost() {
    usleep(1);
}

int main() {
	int n, m;
	while (scanf("%d%d", &n, &m) != EOF ) {
        vector<vector<int>> edges(n, vector<int>());
        for (int i = 0, u, v; i < m; ++i) {
            scanf("%d%d", &u, &v);
            edges[u].push_back(v);
            edges[v].push_back(u);
        }
        int totCnt = 0;
        memset(neig, -1, sizeof(neig));
        clock_t begin = clock();
        bfsReLabel(n, edges);
        vector<int> sg, paired;
        enumerateCsg(n, edges, sg);
        for (auto &nowSet : sg) {
            // printf("now set: %d, actual nodes: %s\n", nowSet, setToString(nowSet).c_str());
            paired.clear();
            enumerateCmp(nowSet, edges, paired);
            totCnt += paired.size();
            for (auto &pairedSet : paired) {
#ifdef CHECKING
                printForCheck(nowSet, pairedSet);
#else
                simulateDPCost();
#endif
            }
        }
#ifdef CHECKING
        printf("0\n");
#else
        printf("elapsed %f, dp entered %d\n", float(clock() - begin) / CLOCKS_PER_SEC, totCnt);
#endif
    }
	return 0;
}
