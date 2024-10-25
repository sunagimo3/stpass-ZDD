#include <bits/stdc++.h>
using namespace std;
const int INF = numeric_limits<int>::max();

vector<int> dijkstra(int start, int V, const vector<vector<pair<int, int>>>& adj) {
    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}


int main(){
    int V,E;
    cin >> V >> E;
    int D;
    cin >> D;//Dはグラフの半径
    int s; //中心となるノード(start node)
    cin >> s;
    vector<pair<int,int>> e(E);
    vector<vector<int>> dis(V,vector<int>(V,-1));
    vector<vector<pair<int, int>>> adj(V);
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        w=1;
        cin >> u >> v;
        e[i].first = u;
        e[i].second = v;        
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 無向グラフの場合
    }
    for(int i=0;i<V;i++){
        dis[i] = dijkstra(i,V,adj);
    }
    cout << endl;
    for(int i=0;i<E;i++){
        int v1 = e[i].first;
        int v2 = e[i].second;
        int dis1 = dis[s][v1];
        int dis2 = dis[s][v2];
        if(dis1 == dis2){
            if(dis1 + 1 > D){
                continue;
            }
            else{
                cout << v1 << " " << v2 << endl;
            }
        }
        else if(dis1 != dis2){
            if(max(dis1,dis2) <= D){
                cout << v1 << " " << v2 << endl;
            }
            else{
                continue;
            }
        }
    }

    return 0;
}



