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


# :warning: convolution/convolutive_translation_2d.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#a9595c1c24c33b16056d2ad07e71682d">convolution</a>
* <a href="{{ site.github.repository_url }}/blob/master/convolution/convolutive_translation_2d.hpp">View this file on GitHub</a>
    - Last commit date: 2020-08-25 07:05:42+09:00




## Depends on

* :heavy_check_mark: <a href="ntt.hpp.html">convolution/ntt.hpp</a>
* :heavy_check_mark: <a href="../modulus/modint_fixed.hpp.html">modulus/modint_fixed.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include <algorithm>
#include <utility>
#include <vector>
#include "convolution/ntt.hpp"

// Two dimensional fast DP using FFT.
// Complexity: O(HW log (HW)) for each step.
// Verification: TCO 2020 Round 3A 1000 ZombieRPGDice
// Verification: TCO 2020 Round 3B 500 ShortBugPaths
template <typename MODINT>
std::vector<std::vector<MODINT>> convolutive_translation_2d(
    const std::vector<std::vector<MODINT>> &initial_dist,  // size: H * W
    const std::vector<std::pair<std::pair<int, int>, MODINT>> &trans_coeffs,  // [((dx, dy), coefficient), ...]
    int nb_step = 1)
{
    int H = initial_dist.size(), W = initial_dist[0].size();
    int xlo = 0, xhi = 0, ylo = 0, yhi = 0;

    std::vector<std::pair<std::pair<int, int>, MODINT>> t2c_1d;
    for (auto p : trans_coeffs) if (p.second != 0)
    {
        if (p.first.first <= -H or p.first.first >= H) continue;
        if (p.first.second <= -W or p.first.second >= W) continue;
        xlo = std::max(xlo, -p.first.first), xhi = std::max(xhi, p.first.first);
        ylo = std::max(ylo, -p.first.second), yhi = std::max(yhi, p.first.second);
        t2c_1d.emplace_back(p);
    }
    const int WW = W + ylo + yhi;
    std::vector<MODINT> dp((H - 1) * WW + W);
    for (int i = 0; i < H; i++)
    {
        std::copy(initial_dist[i].begin(), initial_dist[i].end(), dp.begin() + i * WW);
    }

    int tlo = 0, thi = 0;
    for (auto p : t2c_1d)
    {
        int t = p.first.first * WW + p.first.second;
        tlo = std::max(tlo, -t), thi = std::max(thi, t);
    }

    std::vector<MODINT> trans1d(tlo + thi + 1);
    for (auto p : t2c_1d)
    {
        trans1d[tlo + p.first.first * WW + p.first.second] += p.second;
    }
    for (int t = 0; t < nb_step; t++)
    {
        auto dp_nxt = nttconv(dp, trans1d);
        for (int i = 0; i < H; i++)
        {
            std::copy(dp_nxt.begin() + i * WW + tlo, dp_nxt.begin() + i * WW + W + tlo, dp.begin() + i * WW);
        }
    }
    std::vector<std::vector<MODINT>> ret(H);
    for (int i = 0; i < H; i++)
    {
        ret[i].insert(ret[i].end(), dp.begin() + i * WW, dp.begin() + i * WW + W);
    }
    return ret;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "convolution/convolutive_translation_2d.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#line 2 "modulus/modint_fixed.hpp"
#include <iostream>
#line 4 "modulus/modint_fixed.hpp"
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
#line 3 "convolution/ntt.hpp"

#line 5 "convolution/ntt.hpp"
#include <array>
#include <cassert>
#include <tuple>
#line 9 "convolution/ntt.hpp"

// CUT begin
// Integer convolution for arbitrary mod
// with NTT (and Garner's algorithm) for ModInt / ModIntRuntime class.
// We skip Garner's algorithm if `skip_garner` is true or mod is in `nttprimes`.
// input: a (size: n), b (size: m)
// return: vector (size: n + m - 1)
template <typename MODINT>
std::vector<MODINT> nttconv(std::vector<MODINT> a, std::vector<MODINT> b, bool skip_garner = false);

constexpr int nttprimes[3] = {998244353, 167772161, 469762049};

// Integer FFT (Fast Fourier Transform) for ModInt class
// (Also known as Number Theoretic Transform, NTT)
// is_inverse: inverse transform
// ** Input size must be 2^n **
template <typename MODINT>
void ntt(std::vector<MODINT> &a, bool is_inverse = false)
{
    int n = a.size();
    if (n == 1) return;
    static const int mod = MODINT::get_mod();
    static const MODINT root = MODINT::get_primitive_root();
    assert(__builtin_popcount(n) == 1 and (mod - 1) % n == 0);

    static std::vector<MODINT> w{1}, iw{1};
    for (int m = w.size(); m < n / 2; m *= 2)
    {
        MODINT dw = root.power((mod - 1) / (4 * m)), dwinv = 1 / dw;
        w.resize(m * 2), iw.resize(m * 2);
        for (int i = 0; i < m; i++) w[m + i] = w[i] * dw, iw[m + i] = iw[i] * dwinv;
    }

    if (!is_inverse) {
        for (int m = n; m >>= 1;) {
            for (int s = 0, k = 0; s < n; s += 2 * m, k++) {
                for (int i = s; i < s + m; i++) {
#ifdef __clang__
                    a[i + m] *= w[k];
                    std::tie(a[i], a[i + m]) = std::make_pair(a[i] + a[i + m], a[i] - a[i + m]);
#else
                    MODINT x = a[i], y = a[i + m] * w[k];
                    a[i] = x + y, a[i + m] = x - y;
#endif
                }
            }
        }
    }
    else {
        for (int m = 1; m < n; m *= 2) {
            for (int s = 0, k = 0; s < n; s += 2 * m, k++) {
                for (int i = s; i < s + m; i++) {
#ifdef __clang__
                    std::tie(a[i], a[i + m]) = std::make_pair(a[i] + a[i + m], a[i] - a[i + m]);
                    a[i + m] *= iw[k];
#else
                    MODINT x = a[i], y = a[i + m];
                    a[i] = x + y, a[i + m] = (x - y) * iw[k];
#endif
                }
            }
        }
        int n_inv = MODINT(n).inv();
        for (auto &v : a) v *= n_inv;
    }
}
template <int MOD>
std::vector<ModInt<MOD>> nttconv_(const std::vector<int> &a, const std::vector<int> &b) {
    int sz = a.size();
    assert(a.size() == b.size() and __builtin_popcount(sz) == 1);
    std::vector<ModInt<MOD>> ap(sz), bp(sz);
    for (int i = 0; i < sz; i++) ap[i] = a[i], bp[i] = b[i];
    if (a == b) {
        ntt(ap, false);
        bp = ap;
    }
    else {
        ntt(ap, false);
        ntt(bp, false);
    }
    for (int i = 0; i < sz; i++) ap[i] *= bp[i];
    ntt(ap, true);
    return ap;
}
long long extgcd_ntt_(long long a, long long b, long long &x, long long &y)
{
    long long d = a;
    if (b != 0) d = extgcd_ntt_(b, a % b, y, x), y -= (a / b) * x;
    else x = 1, y = 0;
    return d;
}
long long modinv_ntt_(long long a, long long m)
{
    long long x, y;
    extgcd_ntt_(a, m, x, y);
    return (m + x % m) % m;
}
long long garner_ntt_(int r0, int r1, int r2, int mod)
{
    using mint2 = ModInt<nttprimes[2]>;
    static const long long m01 = 1LL * nttprimes[0] * nttprimes[1];
    static const long long m0_inv_m1 = ModInt<nttprimes[1]>(nttprimes[0]).inv();
    static const long long m01_inv_m2 = mint2(m01).inv();

    int v1 = (m0_inv_m1 * (r1 + nttprimes[1] - r0)) % nttprimes[1];
    auto v2 = (mint2(r2) - r0 - mint2(nttprimes[0]) * v1) * m01_inv_m2;
    return (r0 + 1LL * nttprimes[0] * v1 + m01 % mod * v2.val) % mod;
}
template <typename MODINT>
std::vector<MODINT> nttconv(std::vector<MODINT> a, std::vector<MODINT> b, bool skip_garner)
{
    int sz = 1, n = a.size(), m = b.size();
    while (sz < n + m) sz <<= 1;
    if (sz <= 16) {
        std::vector<MODINT> ret(n + m - 1);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) ret[i + j] += a[i] * b[j];
        }
        return ret;
    }
    int mod = MODINT::get_mod();
    if (skip_garner or std::find(std::begin(nttprimes), std::end(nttprimes), mod) != std::end(nttprimes))
    {
        a.resize(sz), b.resize(sz);
        if (a == b) { ntt(a, false); b = a; }
        else ntt(a, false), ntt(b, false);
        for (int i = 0; i < sz; i++) a[i] *= b[i];
        ntt(a, true);
        a.resize(n + m - 1);
    }
    else {
        std::vector<int> ai(sz), bi(sz);
        for (int i = 0; i < n; i++) ai[i] = a[i].val;
        for (int i = 0; i < m; i++) bi[i] = b[i].val;
        auto ntt0 = nttconv_<nttprimes[0]>(ai, bi);
        auto ntt1 = nttconv_<nttprimes[1]>(ai, bi);
        auto ntt2 = nttconv_<nttprimes[2]>(ai, bi);
        a.resize(n + m - 1);
        for (int i = 0; i < n + m - 1; i++) {
            a[i] = garner_ntt_(ntt0[i].val, ntt1[i].val, ntt2[i].val, mod);
        }
    }
    return a;
}
#line 5 "convolution/convolutive_translation_2d.hpp"

// Two dimensional fast DP using FFT.
// Complexity: O(HW log (HW)) for each step.
// Verification: TCO 2020 Round 3A 1000 ZombieRPGDice
// Verification: TCO 2020 Round 3B 500 ShortBugPaths
template <typename MODINT>
std::vector<std::vector<MODINT>> convolutive_translation_2d(
    const std::vector<std::vector<MODINT>> &initial_dist,  // size: H * W
    const std::vector<std::pair<std::pair<int, int>, MODINT>> &trans_coeffs,  // [((dx, dy), coefficient), ...]
    int nb_step = 1)
{
    int H = initial_dist.size(), W = initial_dist[0].size();
    int xlo = 0, xhi = 0, ylo = 0, yhi = 0;

    std::vector<std::pair<std::pair<int, int>, MODINT>> t2c_1d;
    for (auto p : trans_coeffs) if (p.second != 0)
    {
        if (p.first.first <= -H or p.first.first >= H) continue;
        if (p.first.second <= -W or p.first.second >= W) continue;
        xlo = std::max(xlo, -p.first.first), xhi = std::max(xhi, p.first.first);
        ylo = std::max(ylo, -p.first.second), yhi = std::max(yhi, p.first.second);
        t2c_1d.emplace_back(p);
    }
    const int WW = W + ylo + yhi;
    std::vector<MODINT> dp((H - 1) * WW + W);
    for (int i = 0; i < H; i++)
    {
        std::copy(initial_dist[i].begin(), initial_dist[i].end(), dp.begin() + i * WW);
    }

    int tlo = 0, thi = 0;
    for (auto p : t2c_1d)
    {
        int t = p.first.first * WW + p.first.second;
        tlo = std::max(tlo, -t), thi = std::max(thi, t);
    }

    std::vector<MODINT> trans1d(tlo + thi + 1);
    for (auto p : t2c_1d)
    {
        trans1d[tlo + p.first.first * WW + p.first.second] += p.second;
    }
    for (int t = 0; t < nb_step; t++)
    {
        auto dp_nxt = nttconv(dp, trans1d);
        for (int i = 0; i < H; i++)
        {
            std::copy(dp_nxt.begin() + i * WW + tlo, dp_nxt.begin() + i * WW + W + tlo, dp.begin() + i * WW);
        }
    }
    std::vector<std::vector<MODINT>> ret(H);
    for (int i = 0; i < H; i++)
    {
        ret[i].insert(ret[i].end(), dp.begin() + i * WW, dp.begin() + i * WW + W);
    }
    return ret;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
