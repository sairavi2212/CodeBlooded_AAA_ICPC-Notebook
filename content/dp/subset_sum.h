int subset_sum(vector<int> &a, int sum) {
  vector<int> dp(sum + 1, 0);
  dp[0] = 1;
  for (int x : a)
    for (int s = sum; s >= x; s--)
      dp[s] += dp[s - x];
  return dp[sum];
}
