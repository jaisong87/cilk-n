#include<iostream>
#include<cassert>
using namespace std;


int main()
{
int seed = 2381947,mod = 395731, step = 7794783;
int N, val = 0;

cout<<"Enter(q) the size of the desired [2^q X 2^q] matrix:";
cin>>N;
assert(N>=0);

cout<<N<<endl;
for(int i=1;i<=N;i++,cout<<endl)
	for(int j=1;j<=N;j++,cout<<' ')	
		{
			val+=step;
			val%=mod;
			cout<<val;
		}
return 0;
}
