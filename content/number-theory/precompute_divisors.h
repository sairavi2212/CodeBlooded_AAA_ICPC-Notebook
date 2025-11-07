
const int MAX = 1e5;
vector<int> divisor[MAX + 1];
void sieve() {
  for (int i = 1; i <= MAX; ++i) {
    for (int j = i; j <= MAX; j += i)
      divisor[j].push_back(i);
  }
}
