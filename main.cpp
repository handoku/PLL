#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <set>
#include <map>
#include <cassert>
#include <functional>

using namespace std;

const int inf = 1<<29;
using std::placeholders::_1;
using std::placeholders::_2;
class PLL
{
public:
    PLL(){};
    void init(int n); //init;
    void graph_init(); //get graph;
    void preparing(); //get index;
    void prunedBFS(int vk); //BFS with prune;
    int query_for_pruning(int u, int v); //query_1
    int query_distance(int u, int v); //get distance answer;
    void test(); //just for test;
    bool cmp(int a, int b); // for sorting id;
private:
    int n, m;
    vector<vector<int>> edg; // storing graph
    vector<vector<pair<int, int>>> label; // index
    vector<int> id; // for vertex
    vector<int> dis; // storing current distance in BFS;
    vector<int> label_vk; //storing vk's index in BFS.
};

void PLL::init(int n)
{
    edg.resize(n+1, vector<int>(0));
    label.resize(n+1, vector<pair<int, int>>(0));
    id.resize(n+1);
    dis.resize(n+1,inf);
}

void PLL::graph_init()
{
    int u, v;
    scanf("%d %d", &n, &m);
    init(n);
    for(int i=0;i<m;i++){
        scanf("%d %d", &u, &v);
        edg[u].push_back(v); edg[v].push_back(u);
    }
//    cout<<n<<" "<<m<<endl;
//    for(int i=1;i<=n;i++){
//        for(auto v : edg[i]) cout<<" "<<v;
//        cout<<endl;
//    }
}

void PLL::prunedBFS(int vk_idx)
{
    int vk = id[vk_idx];
    queue<int> q;
    q.push(vk);
    vector<int> vis;
    vis.push_back(vk);
    dis[vk] = 0;
    //cout<<vis.size()<<" "<<dis[vk]<<endl;
    while(!q.empty()){
        int u = q.front(); q.pop();
        if(u != vk){
            if(query_for_pruning(vk, u) <= dis[u]) continue;
            label[u].emplace_back(vk_idx, dis[u]);
        }
        for(int i=0;i<(int)edg[u].size(); i++){
            int v = edg[u][i];
            if(dis[v] == inf){
                dis[v] = dis[u] + 1;
                vis.push_back(v);
                q.push(v);
            }
        }
    }
//    cout<<vk<<"---------"<<endl;
//    for(int i=1;i<=n;i++){
//        for(auto t : label[i]) cout<<t.first<<"=="<<t.second<<"  ";
//        cout<<endl;
//    }
    for(int i=0; i<(int)vis.size(); i++) dis[vis[i]] = inf;
}

bool PLL::cmp(int a, int b)
{
    return edg[id[a]].size() > edg[id[b]].size();
}

void PLL::preparing()
{
    vector<pair<int, int>> node(n+1);
    for(int i=1;i<=n;i++) node[i] = make_pair(edg[i].size(), i);
    sort(node.rbegin(), node.rend()-1);
    for(int i=1;i<=n;i++) id[i] = node[i].second;
//    for(int i=1;i<=n; i++)
//        cout<<i<<" "<<id[i]<<" "<<edg[id[i]].size()<<endl;
//    exit(0);
    for(int i=1;i<=n;i++){
        //cout<<"what "<<i<<endl;
        prunedBFS(i);
    }
}

int PLL::query_for_pruning(int u, int v)
{
    if(u == v) return 0;
    int a=0, b=0;
    int ans = inf;
    while(a < (int)label[u].size() && b < (int)label[v].size()){
        int ida = label[u][a].first, idb = label[v][b].first;
        int dis_u_a = label[u][a].second, dis_v_b = label[v][b].second;
        if(ida == idb){
            ans = min(ans, dis_u_a + dis_v_b);
            a ++;
            b ++;
        }
        else if(ida <= idb){
            a++;
        }
        else b++;
    }
    return ans;
}

int PLL::query_distance(int u, int v)
{
    //assert(1<=u&&u<=n&&1<=v&&v<=n);
    if(u == v) return 0;
    int a=0, b=0;
    int ans = inf;
    while(a < (int)label[u].size() && b < (int)label[v].size()){
        int ida = label[u][a].first, idb = label[v][b].first;
        int dis_u_a = label[u][a].second, dis_v_b = label[v][b].second;

        if(id[ida] == v) return dis_u_a;
        if(id[idb] == u) return dis_v_b;
        if(ida == idb){
            ans = min(ans, dis_u_a + dis_v_b);
            a ++;
            b ++;
        }
        else if(ida <= idb){
            a++;
        }
        else b++;
    }
    return ans==inf?-1:ans;
}

void PLL::test()
{
    int q, u, v;
    for(int i=1; i<=n; i++){
        for(int j=i+1;j<=n;j++){
            assert(query_distance(i, j) == query_distance(j, i));
        }
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cout<<i<<" "<<j<<" "<<query_distance(i, j)<<endl;
        }
    }
//    scanf("%d", &q);
//    while(q --){
//        scanf("%d %d", &u, &v);
//        cout<<query_distance(u, v)<<endl;
//    }
}

int main()
{
#ifdef local_test
    freopen("../graph.txt", "r", stdin);
    freopen("../pll.out", "w", stdout);
#endif // local_test
    PLL pll;
    pll.graph_init();
    pll.preparing();
    pll.test();
    return 0;
}
/*
case:
n m
edg1
.
.
.
edgm
q
query[1]
.
.
.
query[q]

case 1:
7 7
1 2
2 3
2 4
2 6
4 5
4 7
6 7
2
1 7
3 7

case 2:
12 19
1 4
1 6
1 7
1 8
1 9
1 10
1 12
2 5
2 10
2 8
3 11
3 12
4 5
4 6
5 8
6 7
7 9
9 12
10 11

2
1 3
1 11
*/
