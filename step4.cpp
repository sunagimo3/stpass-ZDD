#include <bits/stdc++.h>
using namespace std;

void DeleteSelfLoop(int E) {
    //queは辺の集合を表している
    queue<pair<int, int>> que;
    //countは消去した自己ループの数を表している
    int count = 0;

    for (int i = 0; i < E; i++) {
        int a, b;
        cin >> a >> b;
        if (a == b) {
            count++;
        } 
        else {
            que.push({a, b});
        }
    }
    cout << que.size() << endl;
    while (!que.empty()) {
        pair<int, int> v = que.front();
        que.pop();
        cout << v.first << " " << v.second << endl;
    }
}

int main() {
    int V, E;
    cin >> V >> E;
    cout << V << " ";
    DeleteSelfLoop(E);
    return 0;
}
