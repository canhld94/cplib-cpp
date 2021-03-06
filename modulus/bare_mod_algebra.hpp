#pragma once
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
