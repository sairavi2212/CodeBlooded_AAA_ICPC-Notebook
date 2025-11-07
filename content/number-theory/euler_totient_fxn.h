int euler_totient_function(int n) {
  int res = 1;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int count = 0;
      int val = 1;
      while (n % i == 0) {
        count++;
        val = val * i;
        n = n / i;
      }
      res = res * ((val / i) * (i - 1));
    }
  }
  if (n > 1) {
    res = res * (n - 1);
  }
  return res;
}