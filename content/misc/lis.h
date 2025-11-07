int LIS(vector<int>& a){
 vector<int> v;
 for(int x:a){
  auto it=lower_bound(v.begin(),v.end(),x);
  if(it==v.end()) v.push_back(x);
  else *it=x;
 }
 return (int)v.size();
}