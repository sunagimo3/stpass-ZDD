#include <bits/stdc++.h>
using namespace std;
#include <math.h>
int main(){
    int V,E; //頂点数Vと辺数E
    cin >> V >> E;
    vector<int> G(V,0); //次数毎のノード数の情報
    for(int i=0;i<E;i++){
        int v1,v2;
        cin >> v1 >> v2;
        G[v1]++;
        G[v2]++;
    }
    int degmax = -1;
    for(int i=0;i<V;i++){
        degmax = max(G[i],degmax);
    }
    cout << degmax << endl;
    int deg = 0;
    int answer = 0;
    
    for(int deg = 0;deg <= degmax;deg++){
        int number = 0;
        for(int i=0;i<V;i++){
            if(deg == G[i]) number++;
        }
        answer += number;
        cout << deg << " " << number << endl;
    }


    cout << answer << endl;
}