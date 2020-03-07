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
<script type="text/javascript" src="../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../assets/css/copy-button.css" />


# :x: linear_algebra_matrix/test/linalg_longlong_matmul.test.cpp

<a href="../../../index.html">Back to top page</a>

* <a href="{{ site.github.repository_url }}/blob/master/linear_algebra_matrix/test/linalg_longlong_matmul.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-03-07 22:54:47+09:00


* see: <a href="http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D">http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D</a>


## Depends on

* :x: <a href="../../../library/linear_algebra_matrix/linalg_longlong.hpp.html">linear_algebra_matrix/linalg_longlong.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include <iostream>
#include "linear_algebra_matrix/linalg_longlong.hpp"
using namespace std;
#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D"
using lint = long long;
template<typename T> istream &operator>>(istream &is, vector<T> &vec){ for (auto &v : vec) is >> v; return is; }

int main()
{
    int N, M, L;
    cin >> N >> M >> L;
    vector<vector<lint>> A(N, vector<lint>(M)), B(M, vector<lint>(L));
    cin >> A >> B;
    auto C = matmul(A, B, 1e13);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L - 1; j++) {
            printf("%lld ", C[i][j]);
        }
        printf("%lld\n", C[i].back());
    }
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "linear_algebra_matrix/test/linalg_longlong_matmul.test.cpp"
#include <iostream>
#line 2 "linear_algebra_matrix/linalg_longlong.hpp"
#include <cassert>
#include <cstdlib>
#include <vector>
using namespace std;

// CUT begin
using lint = long long int;
// Solve ax+by=gcd(a, b)
lint extgcd(lint a, lint b, lint &x, lint &y)
{
    lint d = a;
    if (b != 0) d = extgcd(b, a % b, y, x), y -= (a / b) * x;
    else x = 1, y = 0;
    return d;
}
// Calc a^(-1) (MOD m)
lint mod_inverse(lint a, lint m)
{
    lint x, y;
    extgcd(a, m, x, y);
    return (m + x % m) % m;
}
vector<vector<lint>> gauss_jordan(vector<vector<lint>> mtr, lint mod)
{
    // Gauss-Jordan elimination 行基本変形のみを用いるガウス消去法
    int H = mtr.size(), W = mtr[0].size(), c = 0;
    for (int h = 0; h < H; h++)
    {
        if (c == W) break;
        int piv = -1;
        for (int j = h; j < H; j++) if (mtr[j][c]) {
            if (piv == -1 or abs(mtr[j][c]) > abs(mtr[piv][c])) piv = j;
        }
        if (piv == -1) { c++; h--; continue; }
        swap(mtr[piv], mtr[h]);
        if (h != piv) {
            for(int w = 0; w < W; w++) {
                mtr[piv][w] = mtr[piv][w] ? mod - mtr[piv][w] : 0;  // To preserve sign of determinant
            }
        }
        lint pivinv = mod_inverse(mtr[h][c], mod);
        for (int hh = 0; hh < H; hh++) {
            if (hh == h) continue;
            lint coeff = mtr[hh][c] * pivinv % mod;
            for (int w = W - 1; w >= c; w--) {
                mtr[hh][w] = mtr[hh][w] - mtr[h][w] * coeff % mod;
                if (mtr[hh][w] < 0) mtr[hh][w] += mod;
            }
        }
        c++;
    }
    return mtr;
}

int rank_gauss_jordan(const vector<vector<lint>> &mtr) // Rank of Gauss-Jordan eliminated matrix
{
    for (int h = (int)mtr.size() - 1; h >= 0; h--) {
        for (auto v : mtr[h]) if (v) return h + 1;
    }
    return 0;
}

lint mod_determinant(vector<vector<lint>> mtr, lint mod)
{
    if (mtr.empty()) return 1 % mod;
    assert(mtr.size() == mtr[0].size());
    lint ans = 1;
    mtr = gauss_jordan(mtr, mod);
    for (int i = 0; i < (int)mtr.size(); i++) ans = ans * mtr[i][i] % mod;
    return ans;
}

vector<vector<lint>> matmul(const vector<vector<lint>> &A, const vector<vector<lint>> &B, lint mod)
{
    int H = A.size(), W = B[0].size(), K = B.size();
    vector<vector<lint>> C(H, vector<lint>(W));
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            for (int k = 0; k < K; k++) {
                (C[i][j] += A[i][k] * B[k][j]) %= mod;
            }
        }
    }
    return C;
}

vector<lint> matmul(const vector<vector<lint>> &A, const vector<lint> &v, lint mod)
{
    vector<lint> res(A.size());
    for (int i = 0; i < (int)A.size(); i++) {
        for (int j = 0; j < (int)v.size(); j++) {
            (res[i] += A[i][j] * v[j]) %= mod;
        }
    }
    return res;
}
vector<vector<lint>> matpower(vector<vector<lint>> X, lint n, lint mod)
{
    vector<vector<lint>> res(X.size(), vector<lint>(X.size()));
    for (int i = 0; i < (int)res.size(); i++) res[i][i] = 1;
    while (n)
    {
        if (n & 1) res = matmul(res, X, mod);
        X = matmul(X, X, mod); n >>= 1;
    }
    return res;
}
#line 3 "linear_algebra_matrix/test/linalg_longlong_matmul.test.cpp"
using namespace std;
#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D"
using lint = long long;
template<typename T> istream &operator>>(istream &is, vector<T> &vec){ for (auto &v : vec) is >> v; return is; }

int main()
{
    int N, M, L;
    cin >> N >> M >> L;
    vector<vector<lint>> A(N, vector<lint>(M)), B(M, vector<lint>(L));
    cin >> A >> B;
    auto C = matmul(A, B, 1e13);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L - 1; j++) {
            printf("%lld ", C[i][j]);
        }
        printf("%lld\n", C[i].back());
    }
}

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
