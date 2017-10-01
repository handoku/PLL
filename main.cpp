#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <set>
#include <map>

using namespace std;

const int inf = 1<<29;

class PLL
{
public:
    PLL(){};
    void init(int n);
    void graph_init();
    void preparing();
    void prunedBFS(int vk);
    int query(int u, int v);
    void test();
    bool cmp(int a, int b);
private:
    int n, m;
    vector<vector<int>> edg;
    vector<vector<pair<int, int>>> label;
    vector<int> id;
    vector<int> idx;
    vector<int> dis;
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
    while(!q.empty()){
        int u = q.front(); q.pop();
        if(u != vk){
            if(query(vk, u) <= dis[u]) continue;
            label[u].emplace_back(vk_idx,dis[u]);
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
    cout<<vk<<"---------"<<endl;
    for(int i=1;i<=n;i++){
        for(auto t : label[i]) cout<<t.first<<"=="<<t.second<<"  ";
        cout<<endl;
    }
    for(auto v : vis) dis[v] = inf;
}

bool PLL::cmp(int a, int b)
{
    return edg[id[a]].size() > edg[id[b]].size();
}

void PLL::preparing()
{
    for(int i=1;i<=n;i++) id[i] = i;
    sort(id.begin() + 1, id.end());
    for(int i=1;i<=n;i++){
        prunedBFS(i);
    }
}

int PLL::query(int u, int v)
{
    if(u == v) return 0;
    int a=0, b=0;
    int ans = inf;
    while(a < (int)label[u].size() && b < (int)label[v].size()){
        int ida = label[u][a].first, idb = label[v][b].first;
        int dis_u_a = label[u][a].second, dis_v_b = label[v][b].second;
        if(ida == idb){
            ans = min(ans, dis_u_a + dis_v_b);
        }
        else if(ida <= idb){
            a++;
        }
        else b++;
    }
    return ans;
}

void PLL::test()
{
    int q, u, v;
    scanf("%d", &q);
    while(q --){
        scanf("%d %d", &u, &v);
        cout<<query(u, v)<<endl;
    }
}

int main()
{
    PLL pll;
    pll.graph_init();
    pll.preparing();
    pll.test();
    puts("sdfsdf");
    return 0;
}
/*
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
