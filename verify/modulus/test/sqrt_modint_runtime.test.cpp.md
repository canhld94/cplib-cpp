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


# :x: modulus/test/sqrt_modint_runtime.test.cpp

<a href="../../../index.html">Back to top page</a>

* <a href="{{ site.github.repository_url }}/blob/master/modulus/test/sqrt_modint_runtime.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-03-07 22:54:47+09:00


* see: <a href="https://judge.yosupo.jp/problem/sqrt_mod">https://judge.yosupo.jp/problem/sqrt_mod</a>


## Depends on

* :heavy_check_mark: <a href="../../../library/modulus/modint_runtime.hpp.html">modulus/modint_runtime.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include <iostream>
#include "modulus/modint_runtime.hpp"
using namespace std;

int main()
{
    int T;
    cin >> T;
    while (T--) {
        int Y, P;
        cin >> Y >> P;
        ModIntRuntime::mod = P;
        ModIntRuntime m = Y;
        if (Y) {
            m = m.sqrt();
            cout << (m ? m.val : -1) << endl;
        }
        else cout << 0 << endl;
    }
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "modulus/test/sqrt_modint_runtime.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include <iostream>
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
#line 4 "modulus/test/sqrt_modint_runtime.test.cpp"
using namespace std;

int main()
{
    int T;
    cin >> T;
    while (T--) {
        int Y, P;
        cin >> Y >> P;
        ModIntRuntime::mod = P;
        ModIntRuntime m = Y;
        if (Y) {
            m = m.sqrt();
            cout << (m ? m.val : -1) << endl;
        }
        else cout << 0 << endl;
    }
}

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
