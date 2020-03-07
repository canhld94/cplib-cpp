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


# :x: linear_algebra_matrix/test/linalg_modint_multiplication.test.cpp

<a href="../../../index.html">Back to top page</a>

* <a href="{{ site.github.repository_url }}/blob/master/linear_algebra_matrix/test/linalg_modint_multiplication.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-03-07 22:54:47+09:00


* see: <a href="http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D">http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D</a>


## Depends on

* :x: <a href="../../../library/linear_algebra_matrix/linalg_modint.hpp.html">linear_algebra_matrix/linalg_modint.hpp</a>
* :x: <a href="../../../library/modulus/bare_mod_algebra.hpp.html">modulus/bare_mod_algebra.hpp</a>
* :heavy_check_mark: <a href="../../../library/modulus/modint_fixed.hpp.html">modulus/modint_fixed.hpp</a>
* :heavy_check_mark: <a href="../../../library/modulus/modint_runtime.hpp.html">modulus/modint_runtime.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D"
#include "linear_algebra_matrix/linalg_modint.hpp"
#include "modulus/bare_mod_algebra.hpp"
#include "modulus/modint_fixed.hpp"
#include "modulus/modint_runtime.hpp"
#include <iostream>

constexpr int MODfixed = 1000003;
constexpr int MODruntime = 10007;

int main()
{
    int N, M, L;
    std::cin >> N >> M >> L;
    matrix<ModInt<MODfixed>> Afixed(N, M), Bfixed(M, L);
    ModIntRuntime::set_mod(MODruntime);
    std::cin >> Afixed >> Bfixed;
    matrix<ModIntRuntime> Aruntime(N, M), Bruntime(M, L);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Aruntime[i][j] = Afixed[i][j].val;
        }
    }

    for (int j = 0; j < M; j++) {
        for (int k = 0; k < L; k++) {
            Bruntime[j][k] = Bfixed[j][k].val;
        }
    }

    auto Cfixed = Afixed * Bfixed;
    auto Cruntime = Aruntime * Bruntime;

    for (int i = 0; i < N; i++)
    {
        for (int l = 0; l < L; l++) {
            std::cout << linear_congruence({1, 1}, {Cfixed[i][l].val, Cruntime[i][l].val}, {MODfixed, MODruntime}).first;
            std::cout << (l == L - 1 ? "\n" : " ");
        }
    }
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "linear_algebra_matrix/test/linalg_modint_multiplication.test.cpp"
#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D"
#line 2 "linear_algebra_matrix/linalg_modint.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

// CUT begin
template <typename T>
struct matrix
{
    int H, W;
    std::vector<T> elem;
    typename std::vector<T>::iterator operator[](int i) { return elem.begin() + i * W; }
    inline T &at(int i, int j) { return elem[i * W + j]; }
    inline T get(int i, int j) const { return elem[i * W + j]; }
    operator std::vector<std::vector<T>>() const {
        std::vector<std::vector<T>> ret(H);
        for (int i = 0; i < H; i++) std::copy(elem.begin() + i * W, elem.begin() + (i + 1) * W, std::back_inserter(ret[i]));
        return ret;
    }

    matrix(int H = 0, int W = 0) : H(H), W(W), elem(H * W) {}
    matrix(const std::vector<std::vector<T>> &d) : H(d.size()), W(d.size() ? d[0].size() : 0) {
        for (auto &raw : d) std::copy(raw.begin(), raw.end(), std::back_inserter(elem));
    }

    static matrix Identity(int N) { 
        matrix ret(N, N);
        for (int i = 0; i < N; i++) ret.at(i, i) = 1;
        return ret;
    }

    matrix operator-() const { matrix ret(H, W); for (int i = 0; i < H * W; i++) ret.elem[i] = -elem[i]; return ret; }
    matrix operator+(const matrix &r) const { matrix ret(H, W); for (int i = 0; i < H * W; i++) ret.elem[i] += r.elem[i]; return ret; }
    matrix operator-(const matrix &r) const { matrix ret(H, W); for (int i = 0; i < H * W; i++) ret.elem[i] -= r.elem[i]; return ret; }
    matrix operator*(const matrix &r) const {
        matrix ret(H, r.W);
        for (int i = 0; i < H; i++) {
            for (int k = 0; k < W; k++) {
                for (int j = 0; j < r.W; j++) {
                    ret.at(i, j) += this->get(i, k) * r.get(k, j);
                }
            }
        } 
        return ret;
    }
    matrix &operator+=(const matrix &r) { return *this = *this + r; }
    matrix &operator-=(const matrix &r) { return *this = *this - r; }
    matrix &operator*=(const matrix &r) { return *this = *this * r; }
    bool operator==(const matrix &r) const { return H == r.H and W == r.W and elem == r.elem; }
    bool operator!=(const matrix &r) const { return H != r.H or W != r.W or elem != r.elem; }
    bool operator<(const matrix &r) const { return elem < r.elem; }
    matrix pow(int64_t n) const {
        matrix ret = Identity(H);
        if (n == 0) return ret;
        for (int i = 63 - __builtin_clzll(n); i >= 0; i--) {
            ret *= ret;
            if ((n >> i) & 1) ret *= (*this);
        }
        return ret;
    }
    matrix transpose() const {
        matrix ret(W, H);
        for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) ret.at(j, i) = this->get(i, j);
        return ret;
    }
    // Gauss-Jordan elimination
    // - Require inverse for every non-zero element
    // - Complexity: O(H^2 W)
    matrix gauss_jordan() const {
        int c = 0;
        matrix mtr(*this);
        for (int h = 0; h < H; h++) {
            if (c == W) break;
            int piv = -1;
            for (int j = h; j < H; j++) if (mtr.get(j, c)) {
                piv = j;
                break;
            }
            if (piv == -1) { c++; h--; continue; }
            if (h != piv) {
                for (int w = 0; w < W; w++) {
                    std::swap(mtr[piv][w], mtr[h][w]);
                    mtr.at(piv, w) *= -1; // To preserve sign of determinant
                }
            }
            for (int hh = 0; hh < H; hh++) if (hh != h) {
                T coeff = mtr.at(hh, c) * mtr.at(h, c).inv();
                for (int w = W - 1; w >= c; w--) {
                    mtr.at(hh, w) -= mtr.at(h, w) * coeff;
                }
            }
            c++;
        }
        return mtr;
    }
    int rank_of_gauss_jordan() const {
        for (int i = H * W - 1; i >= 0; i--) if (elem[i]) return i / W + 1;
        return 0;
    }
    T determinant_of_upper_triangle() const {
        T ret = 1;
        for (int i = 0; i < H; i++) ret *= get(i, i);
        return ret;
    }
    friend std::ostream &operator<<(std::ostream &os, const matrix &x) {
        os << "[(" << x.H << " * " << x.W << " matrix)";
        for (int i = 0; i < x.H; i++) {
            os << "\n[";
            for (int j = 0; j < x.W; j++) os << x.get(i, j) << ",";
            os << "]";
        }
        os << "]\n";
        return os;
    }
    friend std::istream &operator>>(std::istream &is, matrix &x) {
        for (auto &v : x.elem) is >> v;
        return is;
    }
};


// Fibonacci numbers f(n) = af(n - 1) + bf(n - 2)
// Example (a = b = 1): 0=>1, 1=>1, 2=>2, 3=>3, 4=>5, ...
template <typename T>
T Fibonacci(long long int k, int a = 1, int b = 1)
{
    matrix<T> mat(2, 2);
    mat[0][1] = 1;
    mat[1][0] = b;
    mat[1][1] = a;
    return mat.pow(k + 1)[0][1];
}
#line 2 "modulus/bare_mod_algebra.hpp"
#include <algorithm>
#include <utility>
#include <vector>

// CUT begin
using lint = long long;
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

// 蟻本 P.262
// 中国剰余定理を利用して，色々な素数で割った余りから元の値を復元
// 連立線形合同式 A * x = B mod M の解
// Output: x = first MOD second
std::pair<lint, lint> linear_congruence(const std::vector<lint> &A, const std::vector<lint> &B, const std::vector<lint> &M)
{
    lint x = 0, m = 1;
    for (int i = 0; i < (int)A.size(); i++) {
        lint a = A[i] * m, b = B[i] - A[i] * x, d = std::__gcd(M[i], a);
        if (b % d != 0) {
            return std::make_pair(0, -1); // 解なし
        }
        lint t = b / d * mod_inverse(a / d, M[i] / d) % (M[i] / d);
        x += m * t;
        m *= M[i] / d;
    }
    return std::make_pair((x < 0 ? x + m : x), m);
}

lint power(lint x, lint n, lint MOD)
{
    lint ans = 1;
    while (n > 0) {
        if (n & 1) (ans *= x) %= MOD;
        (x *= x) %= MOD;
       n >>= 1;
    }
   return ans;
}

// Find smallest primitive root for given prime P （最小の原始根探索）
// Complexity: maybe O(sqrt(p))
// Algorithm: <http://kirika-comp.hatenablog.com/entry/2018/03/12/210446>
// Verified: <https://yukicoder.me/submissions/405938>
// Sample:
//  - 998244353 ( = (119 << 23) + 1 ) -> 3
//  - 163577857 ( = (39 << 22) + 1 ) -> 23
//  - 2 -> 1
//  - 1 -> -1

lint find_smallest_primitive_root(lint p)
{
    std::vector<lint> fac;
    lint v = p - 1;
    for (lint pp = 2; pp * pp <= v; pp++) // prime factorization of (p - 1)
    {
        int e = 0;
        while (v % pp == 0) e++, v /= pp;
        if (e) fac.push_back(pp);
    }
    if (v > 1) fac.push_back(v);

    for (lint g = 1; g < p; g++)
    {
        if (power(g, p - 1, p) != 1) return -1;
        bool ok = true;
        for (auto pp : fac)
        {
            if (power(g, (p - 1) / pp, p) == 1)
            {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}
#line 2 "modulus/modint_fixed.hpp"
#include <iostream>
#include <vector>
#include <set>

// CUT begin
template <int mod>
struct ModInt
{
    using lint = long long;
    static int get_mod() { return mod; }
    static int get_primitive_root() {
        static int primitive_root = 0;
        if (!primitive_root) {
            primitive_root = [&](){
                std::set<int> fac;
                int v = mod - 1;
                for (lint i = 2; i * i <= v; i++) while (v % i == 0) fac.insert(i), v /= i;
                if (v > 1) fac.insert(v);
                for (int g = 1; g < mod; g++) {
                    bool ok = true;
                    for (auto i : fac) if (ModInt(g).power((mod - 1) / i) == 1) { ok = false; break; }
                    if (ok) return g;
                }
                return -1;
            }();
        }
        return primitive_root;
    }
    int val;
    constexpr ModInt() : val(0) {}
    constexpr ModInt &_setval(lint v) { val = (v >= mod ? v - mod : v); return *this; }
    constexpr ModInt(lint v) { _setval(v % mod + mod); }
    explicit operator bool() const { return val != 0; }
    constexpr ModInt operator+(const ModInt &x) const { return ModInt()._setval((lint)val + x.val); }
    constexpr ModInt operator-(const ModInt &x) const { return ModInt()._setval((lint)val - x.val + mod); }
    constexpr ModInt operator*(const ModInt &x) const { return ModInt()._setval((lint)val * x.val % mod); }
    constexpr ModInt operator/(const ModInt &x) const { return ModInt()._setval((lint)val * x.inv() % mod); }
    constexpr ModInt operator-() const { return ModInt()._setval(mod - val); }
    constexpr ModInt &operator+=(const ModInt &x) { return *this = *this + x; }
    constexpr ModInt &operator-=(const ModInt &x) { return *this = *this - x; }
    constexpr ModInt &operator*=(const ModInt &x) { return *this = *this * x; }
    constexpr ModInt &operator/=(const ModInt &x) { return *this = *this / x; }
    friend constexpr ModInt operator+(lint a, const ModInt &x) { return ModInt()._setval(a % mod + x.val); }
    friend constexpr ModInt operator-(lint a, const ModInt &x) { return ModInt()._setval(a % mod - x.val + mod); }
    friend constexpr ModInt operator*(lint a, const ModInt &x) { return ModInt()._setval(a % mod * x.val % mod); }
    friend constexpr ModInt operator/(lint a, const ModInt &x) { return ModInt()._setval(a % mod * x.inv() % mod); }
    constexpr bool operator==(const ModInt &x) const { return val == x.val; }
    constexpr bool operator!=(const ModInt &x) const { return val != x.val; }
    bool operator<(const ModInt &x) const { return val < x.val; }  // To use std::map<ModInt, T>
    friend std::istream &operator>>(std::istream &is, ModInt &x) { lint t; is >> t; x = ModInt(t); return is; }
    friend std::ostream &operator<<(std::ostream &os, const ModInt &x) { os << x.val;  return os; }
    constexpr lint power(lint n) const {
        lint ans = 1, tmp = this->val;
        while (n) {
            if (n & 1) ans = ans * tmp % mod;
            tmp = tmp * tmp % mod;
            n /= 2;
        }
        return ans;
    }
    constexpr lint inv() const { return this->power(mod - 2); }
    constexpr ModInt operator^(lint n) const { return ModInt(this->power(n)); }
    constexpr ModInt &operator^=(lint n) { return *this = *this ^ n; }

    inline ModInt fac() const {
        static std::vector<ModInt> facs;
        int l0 = facs.size();
        if (l0 > this->val) return facs[this->val];

        facs.resize(this->val + 1);
        for (int i = l0; i <= this->val; i++) facs[i] = (i == 0 ? ModInt(1) : facs[i - 1] * ModInt(i));
        return facs[this->val];
    }

    ModInt doublefac() const {
        lint k = (this->val + 1) / 2;
        if (this->val & 1) return ModInt(k * 2).fac() / ModInt(2).power(k) / ModInt(k).fac();
        else return ModInt(k).fac() * ModInt(2).power(k);
    }

    ModInt nCr(const ModInt &r) const {
        if (this->val < r.val) return ModInt(0);
        return this->fac() / ((*this - r).fac() * r.fac());
    }

    ModInt sqrt() const {
        if (val == 0) return 0;
        if (mod == 2) return val;
        if (power((mod - 1) / 2) != 1) return 0;
        ModInt b = 1;
        while (b.power((mod - 1) / 2) == 1) b += 1;
        int e = 0, m = mod - 1;
        while (m % 2 == 0) m >>= 1, e++;
        ModInt x = power((m - 1) / 2), y = (*this) * x * x;
        x *= (*this);
        ModInt z = b.power(m);
        while (y != 1) {
            int j = 0;
            ModInt t = y;
            while (t != 1) j++, t *= t;
            z = z.power(1LL << (e - j - 1));
            x *= z, z *= z, y *= z;
            e = j;
        }
        return ModInt(std::min(x.val, mod - x.val));
    }
};

// constexpr lint MOD = 998244353;
// using mint = ModInt<MOD>;
#line 2 "modulus/modint_runtime.hpp"
#include <iostream>
#include <vector>
#include <set>

// CUT begin
struct ModIntRuntime
{
    using lint = long long int;
    static int get_mod() { return mod; }
    int val;
    static int mod;
    static std::vector<ModIntRuntime> &facs()
    {
        static std::vector<ModIntRuntime> facs_;
        return facs_;
    }
    static int &get_primitive_root() {
        static int primitive_root_ = 0;
        if (!primitive_root_) {
            primitive_root_ = [&](){
                std::set<int> fac;
                int v = mod - 1;
                for (lint i = 2; i * i <= v; i++) while (v % i == 0) fac.insert(i), v /= i;
                if (v > 1) fac.insert(v);
                for (int g = 1; g < mod; g++) {
                    bool ok = true;
                    for (auto i : fac) if (ModIntRuntime(g).power((mod - 1) / i) == 1) { ok = false; break; }
                    if (ok) return g;
                }
                return -1;
            }();
        }
        return primitive_root_;
    }
    static void set_mod(const int &m) {
        if (mod != m) facs().clear();
        mod = m;
        get_primitive_root() = 0;
    }
    ModIntRuntime &_setval(lint v) { val = (v >= mod ? v - mod : v); return *this; }
    ModIntRuntime() : val(0) {}
    ModIntRuntime(lint v) { _setval(v % mod + mod); }
    explicit operator bool() const { return val != 0; }
    ModIntRuntime operator+(const ModIntRuntime &x) const { return ModIntRuntime()._setval((lint)val + x.val); }
    ModIntRuntime operator-(const ModIntRuntime &x) const { return ModIntRuntime()._setval((lint)val - x.val + mod); }
    ModIntRuntime operator*(const ModIntRuntime &x) const { return ModIntRuntime()._setval((lint)val * x.val % mod); }
    ModIntRuntime operator/(const ModIntRuntime &x) const { return ModIntRuntime()._setval((lint)val * x.inv() % mod); }
    ModIntRuntime operator-() const { return ModIntRuntime()._setval(mod - val); }
    ModIntRuntime &operator+=(const ModIntRuntime &x) { return *this = *this + x; }
    ModIntRuntime &operator-=(const ModIntRuntime &x) { return *this = *this - x; }
    ModIntRuntime &operator*=(const ModIntRuntime &x) { return *this = *this * x; }
    ModIntRuntime &operator/=(const ModIntRuntime &x) { return *this = *this / x; }
    friend ModIntRuntime operator+(lint a, const ModIntRuntime &x) { return ModIntRuntime()._setval(a % mod + x.val); }
    friend ModIntRuntime operator-(lint a, const ModIntRuntime &x) { return ModIntRuntime()._setval(a % mod - x.val + mod); }
    friend ModIntRuntime operator*(lint a, const ModIntRuntime &x) { return ModIntRuntime()._setval(a % mod * x.val % mod); }
    friend ModIntRuntime operator/(lint a, const ModIntRuntime &x) { return ModIntRuntime()._setval(a % mod * x.inv() % mod); }
    bool operator==(const ModIntRuntime &x) const { return val == x.val; }
    bool operator!=(const ModIntRuntime &x) const { return val != x.val; }
    bool operator<(const ModIntRuntime &x) const { return val < x.val; }  // To use std::map<ModIntRuntime, T>
    friend std::istream &operator>>(std::istream &is, ModIntRuntime &x) { lint t; is >> t; x = ModIntRuntime(t); return is; }
    friend std::ostream &operator<<(std::ostream &os, const ModIntRuntime &x) { os << x.val;  return os; }
 
    lint power(lint n) const {
        lint ans = 1, tmp = this->val;
        while (n) {
            if (n & 1) ans = ans * tmp % mod;
            tmp = tmp * tmp % mod;
            n /= 2;
        }
        return ans;
    }
    lint inv() const { return this->power(mod - 2); }
    ModIntRuntime operator^(lint n) const { return ModIntRuntime(this->power(n)); }
    ModIntRuntime &operator^=(lint n) { return *this = *this ^ n; }
 
    ModIntRuntime fac() const {
        int l0 = facs().size();
        if (l0 > this->val) return facs()[this->val];
 
        facs().resize(this->val + 1);
        for (int i = l0; i <= this->val; i++) facs()[i] = (i == 0 ? ModIntRuntime(1) : facs()[i - 1] * ModIntRuntime(i));
        return facs()[this->val];
    }
 
    ModIntRuntime doublefac() const {
        lint k = (this->val + 1) / 2;
        if (this->val & 1) return ModIntRuntime(k * 2).fac() / ModIntRuntime(2).power(k) / ModIntRuntime(k).fac();
        else return ModIntRuntime(k).fac() * ModIntRuntime(2).power(k);
    }
 
    ModIntRuntime nCr(const ModIntRuntime &r) const {
        if (this->val < r.val) return ModIntRuntime(0);
        return this->fac() / ((*this - r).fac() * r.fac());
    }

    ModIntRuntime sqrt() const {
        if (val == 0) return 0;
        if (mod == 2) return val;
        if (power((mod - 1) / 2) != 1) return 0;
        ModIntRuntime b = 1;
        while (b.power((mod - 1) / 2) == 1) b += 1;
        int e = 0, m = mod - 1;
        while (m % 2 == 0) m >>= 1, e++;
        ModIntRuntime x = power((m - 1) / 2), y = (*this) * x * x;
        x *= (*this);
        ModIntRuntime z = b.power(m);
        while (y != 1) {
            int j = 0;
            ModIntRuntime t = y;
            while (t != 1) j++, t *= t;
            z = z.power(1LL << (e - j - 1));
            x *= z, z *= z, y *= z;
            e = j;
        }
        return ModIntRuntime(std::min(x.val, mod - x.val));
    }
};
int ModIntRuntime::mod = 1;
#line 6 "linear_algebra_matrix/test/linalg_modint_multiplication.test.cpp"
#include <iostream>

constexpr int MODfixed = 1000003;
constexpr int MODruntime = 10007;

int main()
{
    int N, M, L;
    std::cin >> N >> M >> L;
    matrix<ModInt<MODfixed>> Afixed(N, M), Bfixed(M, L);
    ModIntRuntime::set_mod(MODruntime);
    std::cin >> Afixed >> Bfixed;
    matrix<ModIntRuntime> Aruntime(N, M), Bruntime(M, L);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Aruntime[i][j] = Afixed[i][j].val;
        }
    }

    for (int j = 0; j < M; j++) {
        for (int k = 0; k < L; k++) {
            Bruntime[j][k] = Bfixed[j][k].val;
        }
    }

    auto Cfixed = Afixed * Bfixed;
    auto Cruntime = Aruntime * Bruntime;

    for (int i = 0; i < N; i++)
    {
        for (int l = 0; l < L; l++) {
            std::cout << linear_congruence({1, 1}, {Cfixed[i][l].val, Cruntime[i][l].val}, {MODfixed, MODruntime}).first;
            std::cout << (l == L - 1 ? "\n" : " ");
        }
    }
}

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
