#include<iostream>
#include<cassert>
#include<iomanip>
using namespace std;

int main()
{
int N;
cin>>N;

int q = N&(N-1);
assert(q==0);

int step = 489731, mod = 9999, seed = 948723;

cout<<N<<endl;
for(int i=1;i<=N;i++,cout<<endl)
	for(int i=1;i<=N;i++)
		{
			seed+=step;
			seed%=mod;
			seed++;					
			cout<<setw(10)<<setprecision(2)<<fixed<<seed;
		}
return 0;
}
