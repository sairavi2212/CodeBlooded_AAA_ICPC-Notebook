#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long double ld;
const ld EPS = 1e-9;
const ld PI = acos(-1.0);

inline int sign(ld x) { return (x > EPS) - (x < -EPS); }
inline ld sq(ld x) { return x * x; }

struct P {
    ld x, y;
    P(ld x = 0, ld y = 0) : x(x), y(y) {}
    P operator+(const P& o) const { return P(x + o.x, y + o.y); }
    P operator-(const P& o) const { return P(x - o.x, y - o.y); }
    P operator*(ld s) const { return P(x * s, y * s); }
    P operator/(ld s) const { return P(x / s, y / s); }
    ld dot(const P& o) const { return x * o.x + y * o.y; }
    ld cross(const P& o) const { return x * o.y - y * o.x; }
    ld dist2() const { return x * x + y * y; }
    ld dist() const { return sqrt(dist2()); }
    P rotate(ld ang) const { return P(x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang)); }
    P perp() const { return P(-y, x); }
    P unit() const { return *this / dist(); }
    bool operator<(const P& o) const { return sign(x - o.x) != 0 ? x < o.x : sign(y - o.y) < 0; }
    bool operator==(const P& o) const { return sign(x - o.x) == 0 && sign(y - o.y) == 0; }
    friend ostream& operator<<(ostream& os, const P& p) { return os << "(" << p.x << ", " << p.y << ")"; }
};

ld cross(P a, P b, P c) { return (b - a).cross(c - a); }

struct Line {
    ld a, b, c; // ax + by = c
    Line(ld a, ld b, ld c) : a(a), b(b), c(c) {}
    Line(P p1, P p2) : a(p1.y - p2.y), b(p2.x - p1.x), c(a * p1.x + b * p1.y) {}
    ld eval(P p) const { return a * p.x + b * p.y - c; }
    ld norm2() const { return a * a + b * b; }
    bool parallel(const Line& o) const { return sign(a * o.b - o.a * b) == 0; }
    bool intersect(const Line& o, P& res) const {
        ld det = a * o.b - o.a * b;
        if (sign(det) == 0) return false;
        res.x = (c * o.b - o.c * b) / det;
        res.y = (a * o.c - o.a * c) / det;
        return true;
    }
    ld dist(P p) const { return abs(eval(p)) / sqrt(norm2()); }
    P proj(P p) const { ld d = eval(p) / norm2(); return P(p.x - a * d, p.y - b * d); }
    P refl(P p) const { return proj(p) * 2 - p; }
};

struct Seg {
    P s, e;
    Seg(P s, P e) : s(s), e(e) {}
    Line to_line() const { return Line(s, e); }
    bool on_seg(P p) const { return sign(cross(s, e, p)) == 0 && sign((p - s).dot(p - e)) <= 0; }
    ld dist(P p) const {
        if ((p - s).dot(e - s) < 0) return (p - s).dist();
        if ((p - e).dot(s - e) < 0) return (p - e).dist();
        return to_line().dist(p);
    }
};

ld area(const vector<P>& p) {
    ld a = 0;
    for (int i = 0; i < p.size(); i++) a += p[i].cross(p[(i + 1) % p.size()]);
    return a / 2.0;
}

bool is_convex(const vector<P>& p) {
    bool has_pos = false, has_neg = false;
    for (int i = 0; i < p.size(); i++) {
        int o = sign(cross(p[i], p[(i + 1) % p.size()], p[(i + 2) % p.size()]));
        if (o > 0) has_pos = true;
        if (o < 0) has_neg = true;
    }
    return !(has_pos && has_neg);
}

int in_poly(const vector<P>& p, P pt) { // 1: in, -1: on, 0: out
    bool in = false;
    for (int i = 0; i < p.size(); i++) {
        P a = p[i] - pt, b = p[(i + 1) % p.size()] - pt;
        if (a.y > b.y) swap(a, b);
        if (sign(a.cross(b)) == 0 && sign(a.dot(b)) <= 0) return -1;
        if (a.y <= 0 && b.y > 0 && sign(a.cross(b)) > 0) in = !in;
    }
    return in;
}

vector<P> convex_hull(vector<P> pts) {
    if (pts.size() <= 2) return pts;
    sort(pts.begin(), pts.end());
    vector<P> h;
    for (int i = 0; i < 2; i++) {
        int start = h.size();
        for (auto& pt : pts) {
            while (h.size() >= start + 2 && cross(h[h.size() - 2], h.back(), pt) <= 0)
                h.pop_back();
            h.push_back(pt);
        }
        h.pop_back(); reverse(pts.begin(), pts.end());
    }
    return h;
}

struct Circle {
    P c; ld r;
    bool in(P p) { return sign((p - c).dist() - r) <= 0; }
    int intersect(Line l, vector<P>& res) {
        ld d = l.dist(c);
        if (sign(d - r) > 0) return 0;
        P p = l.proj(c);
        if (sign(d - r) == 0) { res.push_back(p); return 1; }
        P dir = P(l.b, -l.a).unit();
        ld h = sqrt(max((ld)0.0, r * r - d * d));
        res.push_back(p + dir * h); res.push_back(p - dir * h);
        return 2;
    }
};

#endif