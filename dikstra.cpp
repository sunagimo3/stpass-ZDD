#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

vector<int> bfs(int start, int V, const vector<vector<int>>& adj) {
    vector<int> dist(V, -1);  // -1は未訪問を示す
    queue<int> q;
    
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] == -1) {  // 未訪問の場合
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

int main() {
    int V, E;
    cin >> V >> E;

    vector<vector<int>> adj(V);
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // 無向グラフの場合
    }

    int start, end;
    cin >> start >> end;

    vector<int> dist = bfs(start, V, adj);
    cout << "shortest path is " << dist[end] << endl;

    return 0;
}
