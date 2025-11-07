
ll count_inversion(vector<ll>& a) {
    int n = a.size();
    vector<ll> tmp(n);
    function<ll(int,int)> ms = [&](int l, int r) -> ll {
        if (r - l <= 1) return 0;
        int m = (l + r) / 2;
        ll inv = ms(l, m) + ms(m, r);
        int i = l, j = m, k = l;
        while (i < m || j < r) {
            if (j == r || (i < m && a[i] <= a[j]))
                tmp[k++] = a[i++];
            else {
                tmp[k++] = a[j++];
                inv += m - i;
            }
        }
        for (int t = l; t < r; ++t) a[t] = tmp[t];
        return inv;
    };
    return ms(0, n);
}