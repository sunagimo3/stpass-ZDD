#include <bits/stdc++.h>
using namespace std;
#include <math.h>
int main(){
    int V,E;
    cin >> V >> E;
    vector<vector<int>> G(V);
    for(int i=0;i<E;i++){
        int v1,v2;
        cin >> v1 >> v2;
        G[v1].push_back(v2);
        G[v2].push_back(v1);
    }
    int rennum = 0;
    vector<int> root(V,-1); // それぞれの頂点における根ノード
    queue<int> que; // 次に探索するべきノード
    que.push(0);
    root[0]=rennum;
    bool FIN = false; // 全てを探索したかどうか
    while(true){
        if(FIN == true) break;
        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto x : G[v]){ // vに接続している頂点xについて
                if(root[x] == -1){ // xが未探索であるならば
                    root[x] = root[v]; 
                    que.push(x);
                }
            }
        }
        bool fin = true;
        for(int i=0;i<V;i++){
            if(root[i] == -1){ //頂点番号iが未探索であるならば
                rennum++;
                que.push(i);
                root[i] = rennum;
                fin = false;
                break;
            }
        }
        if(fin == true) FIN = true;
    }
    cout << rennum + 1 << endl;
}