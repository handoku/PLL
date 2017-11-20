#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <set>
#include <unordered_map>
#include <sys/time.h>
#include <cassert>
#include <fstream>
#include <functional>

using namespace std;

const int inf = 1<<29;
int cnt;

class PLL
{
public:
    PLL(){};
    void init(int n); //init;
    void graph_init();
    void graph_init(ifstream &ifs); //get graph;
    void preparing(); //get index;
    void prunedBFS(int vk); //BFS with prune;
    int query_for_pruning(int u, int v); //query_1
    int query_distance(int u, int v); //get distance answer;
    void test(ofstream &ofs); //just for test;
private:
    int n, m;
    vector<vector<pair<int, int>>> edg; // storing graph : to, cost
    vector<vector<pair<int, int>>> label; // index
    vector<int> id; // get vertex according to position
    vector<int> _inv_id; // get vertex's id
    vector<int> dis; // storing current distance in BFS;
    vector<int> vis;
    vector<int> len_from_vk;
};

void PLL::init(int n)
{
    edg.resize(n+1, vector<pair<int, int>>(0));
    label.resize(n+1, vector<pair<int, int>>(0));
    id.resize(n+1);
    _inv_id.resize(n+1);
    dis.resize(n+1, inf);
    vis.resize(n+1, 0);
    len_from_vk.resize(n+1, inf);
}

void PLL::graph_init()
{
    int u, v, w;
    scanf("%d %d", &n, &m);
    init(n);
    for(int i=0;i<m;i++){
        scanf("%d %d %d", &u, &v, &w);
        edg[u].emplace_back(v, w); edg[v].emplace_back(u, w);
    }
}

void PLL::graph_init(ifstream &ifs)
{
    assert(ifs);
    int u, v, w;
    ifs>>n>>m;
    init(n);
    for(int i=0;i<m;i++){
        ifs>>u>>v>>w;
        edg[u].emplace_back(v, w); edg[v].emplace_back(u, w);
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
    priority_queue<pair<int, int>,vector<pair<int, int> >,greater<pair<int, int> > > q;
    vis[vk] = vk_idx;
    dis[vk] = 0;
    q.push(make_pair(dis[vk], vk));
    for(int i = 0; i<(int)label[vk].size(); i++){
        len_from_vk[label[vk][i].first] = label[vk][i].second;
    }
    while(!q.empty()){
        int u = q.top().second;
        int dist = q.top().first;
        q.pop();
        if(dis[u] < dist) continue;
        if(query_for_pruning(vk, u) <= dis[u] && _inv_id[u] != vk_idx) {
            cnt ++; // calculate how many times we have pruned
            continue;
        }
        label[u].emplace_back(vk, dis[u]);
        for(int i=0;i<(int)edg[u].size(); i++){
            int v = edg[u][i].first, w = edg[u][i].second;
            if(_inv_id[v] < vk_idx) continue;
            //if(vis[v] != vk_idx){
            if(dis[v] > dis[u] + w || vis[v] != vk_idx){
                vis[v] = vk_idx;
                dis[v] = dis[u] + w;
                q.push(make_pair(dis[v], v));
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
    unordered_map<int, int> dis_u_;
    for (int i=0;i<(int)label[u].size(); i++){
        dis_u_[label[u][i].first] = label[u][i].second;
    }
    int ans = inf;
    for (int i=0; i<(int)label[v].size(); i++){
        if (dis_u_.count(label[v][i].first)){
            ans = min(ans, dis_u_[label[v][i].first] + label[v][i].second);
        }
    }
    return ans;
}

void PLL::test(ofstream &ofs)
{
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            ofs<<i<<" "<<j<<" "<<query_distance(i, j)<<endl;
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
#endif // local_test


    for (int x=0;x<6;x++){
        char c = x + '0';
        string str = "../graph_";
        str += c;
        str += ".txt";
        ifstream ifs;
        ifs.open(str);
        PLL pll;
        pll.graph_init(ifs);
        timeval t1, t2;

        gettimeofday(&t1, NULL);
        pll.preparing();
        gettimeofday(&t2, NULL);

        double indexing_time = t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
        cout<<"index_time: "<<indexing_time<<" s"<<endl;

        str = "../pll_";
        str += c;
        str += ".out";
//        ofstream ofs(str);
//        pll.test(ofs);
        cout<<"ok graph_"<<x<<endl<<endl;
    }
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
1 2 1
2 3 3
2 4 1
2 6 7
4 5 2
4 7 5
6 7 4
2
1 7
3 7

case 2:
12 19
1 4 1
1 6 2
1 7 3
1 8 1
1 9 2
1 10 3
1 12 1
2 5 2
2 10 3
2 8 1
3 11 2
3 12 3
4 5 1
4 6 2
5 8 3
6 7 1
7 9 2
9 12 3
10 11 4

2
1 3
1 11
*/
