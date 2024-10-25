#include <bits/stdc++.h>
using namespace std;
//n重線を消すアルゴリズム
//集合として返している
set<pair<int,int>> same_edge_delete_function(int E) {
    set<pair<int,int>> S;
    //Sは辺集合を表したもの
    for(int i = 0; i < E; i++){
        int v1, v2;
        cin >> v1 >> v2;
        // 常に小さい方をfirstにすることで一貫性を持たせる
        if(v1 > v2) swap(v1, v2);
        S.insert({v1, v2});
    }
    return S;
}

//辺集合をcoutするアルゴリズム
void express_S(set<pair<int,int>> S){
    cout << S.size() << endl;
    for(auto v : S){
        cout << v.first << " " << v.second << endl;
    }
}
int main(){
    int V, E;
    cin >> V >> E;
    cout << V  << " ";
    express_S(same_edge_delete_function(E));
    return 0;
}
