---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :heavy_check_mark: graph/dijkstra.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#f8b0b924ebd7046dbfa85a856e4682c8">graph</a>
* <a href="{{ site.github.repository_url }}/blob/master/graph/dijkstra.hpp">View this file on GitHub</a>
    - Last commit date: 2020-03-07 22:40:57+09:00




## Verified with

* :heavy_check_mark: <a href="../../verify/graph/test/dijkstra.test.cpp.html">graph/test/dijkstra.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#pragma once
#include <queue>
#include <utility>
#include <vector>
using namespace std;

using wedges = vector<vector<pair<long long int, long long int>>>; // (to, weight)

constexpr long long int INF = 1e17;
pair<vector<long long int>, vector<int>> dijkstra(int N, int s, const wedges &w)
{
    using lint = long long int;
    using plint = pair<long long int, long long int>;
    vector<lint> dist(N, INF);
    dist[s] = 0;
    vector<int> prev(N, -1);
    priority_queue<plint, vector<plint>, greater<plint>> pq;
    pq.emplace(0, s);
    while (!pq.empty())
    {
        plint p = pq.top();
        pq.pop();
        int v = p.second;
        if (dist[v] < p.first) continue;
        for (auto nx : w[v])
        {
            lint dnx = p.first + nx.second;
            if (dist[nx.first] > dnx)
            {
                dist[nx.first] = dnx, prev[nx.first] = v;
                pq.emplace(dnx, nx.first);
            }
        }
    }
    return make_pair(dist, prev); // (distance, previous_node)
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 2 "graph/dijkstra.hpp"
#include <queue>
#include <utility>
#include <vector>
using namespace std;

using wedges = vector<vector<pair<long long int, long long int>>>; // (to, weight)

constexpr long long int INF = 1e17;
pair<vector<long long int>, vector<int>> dijkstra(int N, int s, const wedges &w)
{
    using lint = long long int;
    using plint = pair<long long int, long long int>;
    vector<lint> dist(N, INF);
    dist[s] = 0;
    vector<int> prev(N, -1);
    priority_queue<plint, vector<plint>, greater<plint>> pq;
    pq.emplace(0, s);
    while (!pq.empty())
    {
        plint p = pq.top();
        pq.pop();
        int v = p.second;
        if (dist[v] < p.first) continue;
        for (auto nx : w[v])
        {
            lint dnx = p.first + nx.second;
            if (dist[nx.first] > dnx)
            {
                dist[nx.first] = dnx, prev[nx.first] = v;
                pq.emplace(dnx, nx.first);
            }
        }
    }
    return make_pair(dist, prev); // (distance, previous_node)
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
