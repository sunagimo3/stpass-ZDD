#include <bits/stdc++.h>
using namespace std;
#include <math.h>
//stパス数え上げ問題をZDDで実装したコードにランダムに終点，始点をもってくる機能を追加した．
int main(){
    double path = 0;
    int E;
    cin >> E;//V,Eはそれぞれ頂点と辺の数を表している.
    vector<pair<int,int>> G(E);//Gは辺集合(ペア型で辺でつながれた両端を情報としてもつ)
    set<int> Vcount;//グラフに存在する頂点の数を集合としてとらえる
    for(int i=0;i<E;i++){ // i番目の辺について言及している
        int v1,v2;
        cin >> v1 >> v2;
        G[i].first = v1;
        G[i].second = v2;
        Vcount.insert(v1);
        Vcount.insert(v2);
    }
    // 開始時間を記録
    auto start = std::chrono::high_resolution_clock::now();
    int V = Vcount.size();//頂点の数V
    map<int,int> convert;//雑多な数をもつノードを0,1,2,,,,と再定義するためのmap(ZDDのアルゴリズム上変換が必要)
    map<int,int> rconvert;//最後に逆方向に復元するためのmap
    set<int> convertcheck;//すでに再定義の番号を割りふったかどうか確認するset
    int nextnode = 0;//再定義する時ノードに割り当てる番号
    for(int i=0;i<E;i++){
        int v1 = G[i].first;
        int v2 = G[i].second;
        if(!convertcheck.count(v1)){
            rconvert[nextnode] = v1;
            convert[v1] = nextnode;
            nextnode++;
            convertcheck.insert(v1);
        }
        if(!convertcheck.count(v2)){
            rconvert[nextnode] = v2;
            convert[v2] = nextnode;
            nextnode++;
            convertcheck.insert(v2);
        }
    }
    for(int i=0;i<E;i++){
        G[i].first = convert[G[i].first];
        G[i].second = convert[G[i].second];
    }
    vector<vector<int>> g(V);//gは頂点に接続している辺集合(ある頂点に接続している頂点集合)
    vector<pair<int,int>> fro(V);//froは頂点vがフロンティア上に存在する時の今見ている辺の上限と下限を示している.
    for(int i=0;i<V;i++){
        fro[i].first = -1;
        fro[i].second = E;
    }
    set<int> candidate;//頂点集合において始点と終点に選びうる頂点たち
    for(int i=0;i<E;i++){ // i番目の辺について言及している
        int v1,v2;
        v1 = G[i].first;
        v2 = G[i].second;
        candidate.insert(v1);
        candidate.insert(v2);
        fro[v1].first = max(i,fro[v1].first);// v1の上限を更新
        fro[v1].second = min(i,fro[v1].second);//v1の下限を更新
        fro[v2].first = max(i,fro[v2].first);//v2の上限を更新
        fro[v2].second = min(i,fro[v2].second);//v2の下限を更新
        G[i].first = v1;//i番目の辺の片方の頂点v1
        G[i].second = v2;//i番目の辺の片方の頂点v2
        g[v1].push_back(i);//g[v1]はv1に接続している辺集合を表している
        g[v2].push_back(i);//g[v2]はv2に接続している辺集合を表している
    }
    int s,t;//s,tは始点と終点を示している
    srand(static_cast<unsigned int>(std::time(nullptr)));
        // set を vector に変換
    vector<int> Can(candidate.begin(), candidate.end());
    for(int i = 0;i<100;i++){ // 1つのデータに対して10回の施行を行っている
        set<int> stV;//G2はstの辺集合
        // ランダムに2つの異なるインデックスを選ぶ
        int s = std::rand() % Can.size();
        int t;
        do {
            t = std::rand() % Can.size();
        } while (s == t); // 重複を避ける
        stV.insert(s);
        stV.insert(t);
    vector<pair<string,string>> A0;//0枝集合
    int a0count=0;//次発生するA0の枝番号を示している
    vector<queue<int>> layer0(E);//i番目の層からでるA0の枝番号の行列
    vector<pair<string,string>> A1;//1枝集合
    vector<queue<int>> layer1(E);//i番目の層からでるA1の枝番号の行列
    int a1count=0;//次発生するA1の枝番号を示している
    vector<vector<int>> layer(E);//i番目の層ラベルに存在するノード集合
    layer[0].push_back(0);//0番目の層ラベルに存在するノードは根だけであり,それは0番目のノードと定義する
    vector<vector<int>> deg(100000,vector<int>(V));//ノード番号がiのグラフにおける各頂点の次数集合
    //10000個のグラフ状態において各頂点の次数を表している.
    //この数字はこの数さえあれば大丈夫だよね的な奴
    //nが大きくなるとダメな感じになるから動的メモリ確保をしていく
    //補足.deg[i][v]はi番目のグラフ状態の頂点vの次数を表している
    vector<vector<int>> comp(100000,vector<int>(V));//あるグラフにおける各頂点の連結状態を表している.
    //10000個のグラフ状態において,各頂点の連結状態を表している.
    //この数字はこの数さえあれば大丈夫だよね的な奴
    //nが大きくなるとダメな感じになるから動的メモリ確保をしていく
    //補足.comp[i][v]はi番目のグラフ状態の頂点vの次数を表している
    for(int i=0;i<100000;i++){
        for(int j=0;j<V;j++){
            comp[i][j]=j;//j番目の頂点は全てj
        }
    }
    for(int i=0;i<V;i++){
        deg[0][i]=0;
    }
    //最初何の辺も通っていない状態においては全ての辺の次数は0である
    int nodenumber = 0;
    int countT=0;
    int countshare=0;
    //nodenumberは次発生するノードの番号を表している
    //countTはTである集合の数
    for(int i=0;i<E;i++){
        //iは層ラベル
        for(auto a:layer[i]){
            //aはノードの番号(表しているグラフの番号)
            for(int b=0;b<=1;b++){
                //bは0枝か1枝かを表現している
                int v1 = G[i].first;
                int v2 = G[i].second;
                //v1,v2はi番目の辺の両端の辺
                vector<int> checkcomp(V);
                //T-pruneが発動できるかどうかの判定に使う配列,今考えているグラフの連結成分を表示している
                for(int h=0;h<V;h++){
                    checkcomp[h]=comp[a][h];
                }
                //連結成分全てを変える必要がある(b=0の場合は変更しようがない)
                if(b==1){
                    int changenode = max(checkcomp[v1],checkcomp[v2]); //v1,v2の内大きいcompを持っているノードに操作を加える
                    int changenumber = min(checkcomp[v1],checkcomp[v2]);//v1,v2の内小さいcompを変化後のcompにする
                    for(int k=0;k<V;k++){
                        if(checkcomp[k]==changenode){
                            checkcomp[k] = changenumber;
                        }
                    }
                }
                //aから出るb枝
                vector<int> deg1(V);
                //deg1は前回nodeであるaのグラフの次数を引き継いだ上でb=1においては対象辺の両端の頂点次数を1つ増やしたものである.
                for(int j=0;j<V;j++){
                    deg1[j] = deg[a][j];
                }
                //deg1は前のノードの辺集合を引き継いでいる
                //deg1はaノードのb枝から生まれたノードが表すグラフの連結成分
                if(b==1){
                    deg1[v1]++;
                    deg1[v2]++;
                }
                bool fcheck=false;
                //F-pruneのフェーズ
                //b=0とb=1では枝刈りの操作が異なる 何も考えず全部同じ操作をしても同じ結果になるが分けて必要なものを必要なだけやる方が計算量は減る
                if(b==0){
                    //b=0,b=1にわける条件はb=0,1で枝刈りの手法が異なるため
                    //b=0であったらi番目の辺を辺集合に加えない
                    if(stV.count(v1)==true){
                        int count = deg1[v1];
                        for (auto e: g[v1]) {
                            if(e>i) count++;
                        }
                        if(count == 0){
                            fcheck=true;
                        }
                    }
                    //s,tの次数が0に確定する場面
                    if(stV.count(v2)==true){
                        int count = deg1[v2];
                        for (auto e: g[v2]) {
                            if(e>i) count++;
                        }
                        if(count == 0){
                            fcheck=true;
                        }
                    }
                    //s,tの次数が0に確定する場面
                    if(stV.count(v1)!=true){
                        int count = deg1[v1];
                        if(count==1){
                            for (auto e: g[v1]) {
                                if(e>i) count++;
                            }
                            if(count == 1){
                                fcheck=true;
                            }
                        }
                    }
                    //s,t以外のノードの次数が1に確定する場面
                    if(stV.count(v2)!=true){
                        int count = deg1[v2];
                        if(count==1){
                            for (auto e: g[v2]) {
                                if(e>i) count++;
                            }
                            if(count == 1){
                                fcheck=true;
                            }
                        }
                    }
                    //s,t以外のノードの次数が1に確定する場面
                }
                //b=0の枝刈りが終了したのでb=1の枝刈りに切り替える
                if(b==1){
                    if(stV.count(v1)==true){
                        int count = deg1[v1];
                        for (auto e: g[v1]) {
                            if(e>i) count++;
                        }
                        if(count == 0){
                            fcheck=true;
                        }
                    }
                    //s,tの次数が0に確定する場面
                    if(stV.count(v2)==true){
                        int count = deg1[v2];
                        for (auto e: g[v2]) {
                            if(e>i) count++;
                        }
                        if(count == 0){
                            fcheck=true;
                        }
                    }
                    //s,tの次数が0に確定する場面
                    if(stV.count(v1)==true){
                        if(deg1[v1] >= 2){
                            fcheck=true;
                        }                    
                    }
                    //s,tの次数が2以上に確定する場面
                    if(stV.count(v2)==true){
                        if(deg1[v2] >= 2){
                            fcheck=true;
                        } 
                    }
                    //s,tの次数が2以上に確定する場面
                    if(stV.count(v1)!=true){
                        if(deg1[v1] >= 3){
                            fcheck=true;
                        }                    
                    }
                    //s,t以外のノードの次数が3以上に確定する場面
                    if(stV.count(v2)!=true){
                        if(deg1[v2] >= 3){
                            fcheck=true;
                        }                    
                    }
                    //s,t以外のノードの次数が3以上に確定する場面
                    if(stV.count(v1)!=true){
                        int count = deg1[v1];
                        if(deg1[v1]==1){
                            for (auto e: g[v1]) {
                                if(e>i) count++;
                            }
                            if(count == 1){
                                fcheck=true;
                            }
                        }
                    }
                    //s,t以外のノードの次数が1に確定する場面
                    if(stV.count(v2)!=true){
                        int count = deg1[v2];
                        if(deg1[v2]==1){
                            for (auto e: g[v2]) {
                                if(e>i) count++;
                            }
                            if(count == 1){
                                fcheck=true;
                            }
                        }
                    }
                    //s,t以外のノードの次数が1に確定する場面
                    if(checkcomp[s]==checkcomp[t]){
                        for(int j=0;j<V;j++){
                            if(stV.count(j)!=true && deg1[j]==1){
                                fcheck=true;
                            }
                        }
                    }
                    //これはs to tまでのパスが発生した状態においてs,t以外の頂点のノードが1の場合, その状態は不適合つまり...
                    //s,t以外のノードの次数が1に確定する場面
                    //ループ発生条件
                    if(comp[a][v1]==comp[a][v2]){
                        fcheck=true;
                    }
                    //v1,v2が同じ連結成分である時、ループが発生することがわかる
                    //ループ発生条件終了
                }
                //F-pruneのフェーズ
                if(fcheck==true){
                    string a1 = to_string(a);
                    if(b==0){
                        layer0[i].push(a0count);
                        a0count++;
                        A0.push_back({a1,"F"});
                    }
                    if(b==1){
                        layer1[i].push(a1count);
                        a1count++;
                        A1.push_back({a1,"F"});
                    }
                    continue;
                }
                //F-pruneのフェーズ終了
                //checkcompは今考えているグラフ状態の連結状態を表現している
                //F-pruneフェーズ2:全ての辺の使用不使用を考えた結果、s to t pathが完成しない!
                if(i==E-1){
                    if(checkcomp[s]!=checkcomp[t]){
                        string a1 = to_string(a);
                        if(b==0){
                            layer0[i].push(a0count);
                            a0count++;
                            A0.push_back({a1,"F"});
                        }
                        if(b==1){
                            layer1[i].push(a1count);
                            a1count++;
                            A1.push_back({a1,"F"});
                        }
                        continue;
                    }
                }
                //T-prune sとtの連結成分が同じであればよい(例外ケースは上で削除済み)
                if(checkcomp[s]==checkcomp[t]){
                    countT++;
                    string a1 = to_string(a);
                    if(b==0){
                        layer0[i].push(a0count);
                        a0count++;
                        A0.push_back({a1,"T"});
                    }
                    if(b==1){
                        layer1[i].push(a1count);
                        a1count++;
                        A1.push_back({a1,"T"});
                    }
                    continue;
                }
                //T-prune終わり
                else{
                    //共有条件スタート
                    bool Same=false;
                    int BETA;
                    //層ラベルがi+1のノード集合の内1つでも今考えているものが存在すればよい.
                    //に対するtrue or falseの解答
                    if(layer[i+1].size()>0){
                        for(auto beta:layer[i+1]){
                            bool same = true;
                            //i+1番目の辺
                            for(int j=0;j<V;j++){
                                //jは頂点j=1,2....Vを示ている
                                if(fro[j].first>=i+1 && fro[j].second<=i+1){
                                    //上限と下限の間に存在すればその頂点はフロンティア上に存在するといえる.
                                    //次数についての共有スタート
                                    if(deg[beta][j] != deg1[j]){
                                        same = false;
                                    }
                                    //次数についての共有終了
                                    //連結成分についての共有スタート
                                    if(checkcomp[j] != comp[beta][j]){
                                        same = false;
                                    }
                                }
                                //連結成分についての共有終了
                            }
                            if(same==true){
                                Same = true;
                                BETA = beta;
                            }
                        }
                    }
                    if(Same == true){
                        countshare++;
                        string a1 = to_string(a);
                        string nodenumber1 = to_string(BETA);
                        if(b==0){
                            layer0[i].push(a0count);
                            a0count++;
                            A0.push_back({a1,nodenumber1});
                        }
                        if(b==1){
                            layer1[i].push(a1count);
                            a1count++;
                            A1.push_back({a1,nodenumber1});
                        }
                    }
                    //共有条件終了
                    else if(Same==false){
                        nodenumber++;
                        layer[i+1].push_back(nodenumber);//下の層にnodenumberを追加する
                        for(int l=0;l<V;l++){
                            comp[nodenumber][l] = checkcomp[l];
                        }
                        for(int j=0;j<V;j++){
                            deg[nodenumber][j] =  deg1[j];
                        }
                        string a1 = to_string(a);
                        string nodenumber1 = to_string(nodenumber);
                        if(b==0){
                            layer0[i].push(a0count);
                            a0count++;
                            A0.push_back({a1,nodenumber1});
                        }
                        if(b==1){
                            layer1[i].push(a1count);
                            a1count++;
                            A1.push_back({a1,nodenumber1});
                        }
                    }
                }
            }
        }
    }
        //ZDDを表現した決定図をターミナルに表示するコード
    /*
    for(int i=0;i<E;i++){
        for(auto n:layer[i]){
            cout << n << " ";
        }
        cout << endl;
    }
    cout << "T" << " " << "F" << endl;
    cout << endl;
    cout << "0-edge" <<endl;
    for(auto a:A0){
        cout << a.first << " " << a.second << endl;
    }
    cout << endl;
    cout << "1-edge" <<endl;
    for(auto a:A1){
        cout << a.first << " " << a.second << endl;
    }
    */
    //layer0,layer1に正しく要素が配置されたかを確認するだけのコード(より,コメントアウト)
    /*
    for(int i=0;i<E;i++){
        cout << "i= " << i << " " << "edge start" << endl; 
        while(!layer0[i].empty()){
            int v = layer0[i].front();
            layer0[i].pop();
            cout << A0[v].first << " " <<  A0[v].second << endl;
        }
        while(!layer1[i].empty()){
            int v = layer1[i].front();
            layer1[i].pop();
            cout << A1[v].first << " " <<  A1[v].second << endl;
        }
    }
    */
    /*
    for(int i=0;i<E;i++){
        cout << "i= " << i << " " << "edge start" << endl; 
        cout << "0-edge ";
        while(!layer0[i].empty()){
            int v = layer0[i].front();
            layer0[i].pop();
            cout << v << " ";
        }
        cout << endl;
        cout << "1-edge ";
        while(!layer1[i].empty()){
            int v = layer1[i].front();
            layer1[i].pop();
            cout << v << " ";
        }
        cout << endl;
    }
    */
    vector<int> hm(nodenumber+1,0);
    hm[0]=1;
    int answer = 0;
    //A0[v].second!="F"の例外処理を忘れない(自分はこれで5時間溶かした)
    for(int i=0;i<E;i++){
        while(!layer0[i].empty()){
            int v = layer0[i].front();
            layer0[i].pop();
            if(A0[v].second=="T"){
                int afbefore = stoi(A0[v].first);
                answer += hm[afbefore];
                continue;
            }
            else if(A0[v].second=="F"){
                continue;
            }
            else{
                int afbefore = stoi(A0[v].first);
                int afafter = stoi(A0[v].second);
                hm[afafter] += hm[afbefore];
            }
        }
        while(!layer1[i].empty()){
            int v = layer1[i].front();
            layer1[i].pop();
            if(A1[v].second=="T"){
                int afbefore = stoi(A1[v].first);
                answer += hm[afbefore];
                continue;
            }
            else if(A1[v].second=="F"){
                continue;
            }
            else{
                int afbefore = stoi(A1[v].first);
                int afafter = stoi(A1[v].second);
                hm[afafter] += hm[afbefore];
            }
        }
    }
    //それぞれのノードの持つ場合の数(How Many)を問いている
    /*
    for(int i=0;i<=nodenumber;i++){
        cout << hm[i] << endl;
    }
    */
    cout << rconvert[s] << " " << rconvert[t] << " " << answer << endl;
        path += answer;
}//ランダムに10個の値を取り出す操作が終わった.
// 終了時間を記録
    auto end = std::chrono::high_resolution_clock::now();

    // 経過時間を計算
    std::chrono::duration<double> elapsed = end - start;

    // 結果を出力
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    cout << "average path " << path/100 << endl;
    cout << "node number is" <<  V << " " << "edge number is" << E << endl;
    return 0;
}