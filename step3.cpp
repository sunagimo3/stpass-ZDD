#include <bits/stdc++.h>
using namespace std;

//次数2のノードがあったときの処理
//次数が2のノードがあったら両端のノードの行き先を変化させる
void processDegreeTwoNodes(vector<vector<int>>& G, int V) {
    for (int i = 0; i < V; i++) {
        if (G[i].size() == 2) {
            int v1 = G[i][0];
            int v2 = G[i][1];

            // ノードiの隣接ノードの接続を調整
            for (int j = 0; j < G[v1].size(); j++) {
                if (G[v1][j] == i) G[v1][j] = v2;
            }
            for (int j = 0; j < G[v2].size(); j++) {
                if (G[v2][j] == i) G[v2][j] = v1;
            }
        }
    }
}

//次数2以外のノードの処理
void buildEdgeSet(const vector<vector<int>>& G, int V, set<pair<int,int>>& S, map<pair<int,int>,int>& mp) {
    for (int i = 0; i < V; i++) {
        if (G[i].size() != 2) {
            for (int j = 0; j < G[i].size(); j++) {
                int v = G[i][j];
                if (i != v) {
                    if (S.count({i, v}) || S.count({v, i})) {
                        mp[{i, v}]++;
                        mp[{v, i}]++;
                    } 
                    else {
                        S.insert({i, v});
                        mp[{i, v}]++;
                        mp[{v, i}]++;
                    }
                } 
                else {
                    // 自己ループの処理
                    if (S.count({i, v}) || S.count({v, i})) {
                        mp[{i, v}]++;
                    } 
                    else {
                        S.insert({i, v});
                        mp[{i, v}]++;
                    }
                }
            }
        }
    }
}

//辺をcoutする関数
void outputEdges(const set<pair<int,int>>& S, const map<pair<int,int>,int>& mp) {
    int e = 0;
    for (const auto& v : S) {
        for (int i = 0; i < mp.at(v) / 2; i++) {
            e++;
        }
    }
    cout << e << endl;
    for (const auto& v : S) {
        for (int i = 0; i < mp.at(v) / 2; i++) {
            cout << v.first << " " << v.second << endl;
        }
    }
}

int main() {
    int V, E;
    cin >> V >> E;
    vector<vector<int>> G(V);

    for (int i = 0; i < E; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        G[v1].push_back(v2);
        G[v2].push_back(v1);
    }

    processDegreeTwoNodes(G, V);

    //Sは今存在する辺集合を表している
    set<pair<int,int>> S;
    //mpは辺集合の数を表している
    map<pair<int,int>,int> mp;
    buildEdgeSet(G, V, S, mp);
    cout << V << " ";
    outputEdges(S, mp);

    return 0;
}
