vector<int> sort_cyclic_shifts(const string &a) {
  int n = a.size();
  const int alphabet = 256;

  vector<int> p(n), c(n);
  vector<int> cnt(max(alphabet, n), 0);

  // initial sort for h = 0
  for (int i = 0; i < n; i++)
    cnt[(unsigned char)a[i]]++;
  for (int i = 1; i < alphabet; i++)
    cnt[i] = cnt[i - 1] + cnt[i];
  for (int i = n - 1; i >= 0; i--)
    p[--cnt[(unsigned char)a[i]]] = i;

  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (a[p[i]] != a[p[i - 1]])
      classes++;
    c[p[i]] = classes - 1;
  }

  for (int h = 0; (1 << h) < n; h++) {
    vector<int> pn(n), cn(n);

    // first we are sorting on the basis of second half ...
    for (int i = 0; i < n; i++) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)
        pn[i] += n;
    }

    // now we sort on the base of first half ... (stable counting sort)
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++)
      cnt[c[pn[i]]]++;
    for (int i = 1; i < classes; i++)
      cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
      p[--cnt[c[pn[i]]]] = pn[i];

    // now we evaluate new classes
    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
      if (cur != prev)
        classes++;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }

  return p;
}

vector<int> suffix_array(string s) {
  s += '$';
  vector<int> sorted_shifts = sort_cyclic_shifts(s);
  // remove sentinel position
  sorted_shifts.erase(sorted_shifts.begin());
  return sorted_shifts;
}