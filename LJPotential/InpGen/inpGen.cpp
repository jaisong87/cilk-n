#include<iostream>
using namespace std;

#define MAX_X 10000
#define MAX_Y 10000
#define MAX_Z 10000

int main()
{
//int seedX = 79437, seedY = 13987, stepX = 89461, stepY = 13, seedZ = 97, stepZ = 7997;
int seedX = 79437, seedY = 13987, stepX = 1, stepY = 1, seedZ = 97, stepZ = 1;

int N;
cin>>N;

cout<<N<<endl;

for(int i=1;i<=N;i++)
	{
		seedX +=stepX;
		seedX %= MAX_X;
		
		seedY += stepY;
		seedY %= MAX_Y;

		seedZ += stepZ;
		seedZ %= MAX_Z;

		cout<<seedX<<" "<<seedY<<" "<<seedZ<<endl;
	}

return 0;
}
