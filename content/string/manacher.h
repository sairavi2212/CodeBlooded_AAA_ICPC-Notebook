vector<int> manacher_odd(string s) {
  int n = s.size();
  s = "$" + s + "^";
  vector<int> p(n + 2);
  int l = 0, r = 1;
  for (int i = 1; i <= n; i++) {
    p[i] = min(r - i, p[l + (r - i)]);
    while (s[i - p[i]] == s[i + p[i]]) {
      p[i]++;
    }
    if (i + p[i] > r) {
      l = i - p[i];
      r = i + p[i];
    }
  }
  return vector<int>(begin(p) + 1, end(p) - 1);
}

// gives a vector of size 2*n - 1 each element of which gives the length of
// longest palindrome centred at ith location
// if i is even(0,2,4 .. etc.) center is an element -> odd palindromes
// and if i is odd(1, 3, 5 .. etc.) center is between two elements -> even
// palindromes
vector<int> manacher(string s) {
  string t;
  for (auto c : s) {
    t += string("#") + c;
  }
  auto res = manacher_odd(t + "#");
  for (auto &e : res) {
    e = e - 1;
  }
  return vector<int>(begin(res) + 1, end(res) - 1);
}