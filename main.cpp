#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <set>
#include <map>
#include <sys/time.h>
#include <cassert>
#include <functional>

using namespace std;

const int inf = 1<<29;
int cnt;

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
private:
    int n, m;
    vector<vector<int>> edg; // storing graph
    vector<vector<pair<int, int8_t>>> label; // index
    vector<int> id; // get vertex according to position
    vector<int> _inv_id; // get vertex's id
    vector<int> dis; // storing current distance in BFS;
    vector<int> vis; //flag denoting if vertex has been visited
    vector<int> len_from_vk;
};

void PLL::init(int n)
{
    edg.resize(n+1, vector<int>(0));
    label.resize(n+1, vector<pair<int, int8_t>>(0));
    id.resize(n+1);
    _inv_id.resize(n+1);
    dis.resize(n+1,inf);
    vis.resize(n+1,0);
    len_from_vk.resize(n+1, inf);
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
}

void PLL::preparing()
{
    vector<pair<int, int>> node(n+1);
    for(int i=1;i<=n;i++) node[i] = make_pair(edg[i].size(), i);
    sort(node.rbegin(), node.rend()-1);
    for(int i=1;i<=n;i++) id[i] = node[i].second, _inv_id[node[i].second] = i;
    for(int i=1;i<=n;i++){
        prunedBFS(i);
    }


//    cout<<"index: "<<endl;
//    for(int i=1;i<=n;i++){
//        cout<<"from "<<id[i]<<": "<<endl;
//        for(auto v : label[i]){
//            cout<<id[v.first]<<" : "<<v.second<<" ";
//        }
//        cout<<endl;
//    }

    cerr<<"pruned times: "<<cnt<<endl;
    int label_size = 0;
    for(int i=1;i<=n;i++) label_size += label[i].size();
    cerr<<"average label size: "<<1.0*label_size/n<<endl;

}

void PLL::prunedBFS(int vk_idx)
{
    int vk = id[vk_idx];
    queue<int> q;
    q.push(vk);
    vis[vk] = vk_idx;
    dis[vk] = 0;
    for(int i = 0; i<(int)label[vk].size(); i++){
        len_from_vk[label[vk][i].first] = label[vk][i].second;
    }
    while(!q.empty()){
        int u = q.front(); q.pop();
        if(query_for_pruning(vk, u) <= dis[u] && _inv_id[u] != vk_idx) {
            cnt ++; // calculate how many times we have pruned
            continue;
        }
        label[u].emplace_back(vk_idx, dis[u]);
        for(int i=0;i<(int)edg[u].size(); i++){
            int v = edg[u][i];
            if(_inv_id[v] < vk_idx) continue;
            if(vis[v] != vk_idx){
                dis[v] = dis[u] + 1;
                vis[v] = vk_idx;
                q.push(v);
            }
        }
    }
    for(int i = 0; i<(int)label[vk].size(); i++){
        len_from_vk[label[vk][i].first] = inf;
    }
}

int PLL::query_for_pruning(int u, int v)
{
    if(u == v) return 0;
    int ans = inf;
    for(int i=0; i<(int)label[v].size(); i++){
        ans = min(ans, len_from_vk[label[v][i].first] + label[v][i].second);
    }
    return ans;
}

int PLL::query_distance(int u, int v)
{
    assert(1<=u&&u<=n&&1<=v&&v<=n);
    if(_inv_id[u] < _inv_id[v]) swap(u, v);
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
    while(a < (int)label[u].size()){
        int ida = label[u][a].first, dis_u_a = label[u][a].second;
        if(id[ida] == v) return dis_u_a;
        a++;
    }
    return ans==inf?-1:ans;
}

void PLL::test()
{
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cout<<i<<" "<<j<<" "<<query_distance(i, j)<<endl;
        }
    }
//    int q, u, v;
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
    timeval t1, t2;

    gettimeofday(&t1, NULL);
    pll.preparing();
    gettimeofday(&t2, NULL);

    double indexing_time = t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
    cerr<<"index_time: "<<indexing_time<<" s"<<endl;
    //pll.test();
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
