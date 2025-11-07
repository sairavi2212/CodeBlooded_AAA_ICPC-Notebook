int edit_distance(string a,string b){
 int n=a.size(),m=b.size();
 vector<int> dp(m+1),ndp(m+1);
 iota(dp.begin(),dp.end(),0);
 for(int i=1;i<=n;i++){
  ndp[0]=i;
  for(int j=1;j<=m;j++)
   ndp[j]=a[i-1]==b[j-1]?dp[j-1]:1+min({dp[j-1],dp[j],ndp[j-1]});
  swap(dp,ndp);
 }
 return dp[m];
}