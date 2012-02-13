#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

#define MAX 1000

bool dbg = false;

int main()
{
int N;
//int Px[MAX],Px[MAX],Pz[MAX];
double *Px,*Py,*Pz;

cin>>N;
Px = new double[N]; 
Py = new double[N]; 
Pz = new double[N]; 

for(int i=0;i<N;i++)
	cin>>Px[i]>>Py[i]>>Pz[i];

double E = 0.0;

if(dbg) cout<<fixed<<E<<endl;

for(int i=0;i<N;i++)
	for(int j=0;j<N;j++)
		{
		if(i!=j)
			{
		if(dbg)	{
				cout<<fixed<<Px[i]<<" "<<fixed<<Py[i]<<" "<<fixed<<Pz[i]<<endl;
				cout<<fixed<<Px[j]<<" "<<fixed<<Py[j]<<" "<<fixed<<Pz[j]<<endl;
			}
		double d = ( (Px[i]-Px[j])*(Px[i]-Px[j]) + (Py[i]-Py[j])*(Py[i]-Py[j]) + (Pz[i]-Pz[j])*(Pz[i]-Pz[j]));
		if(dbg) cout<<" d = "<<fixed<<d<<endl;
		double dis = sqrt(d);
		double r = 0; 
		if(dis!=0) r = 1.0/dis;
		double r3 = r*r*r; 
		double r6 = r3*r3;
		double r12 = r6*r6;
		E = E + (r12-r6);
		if(dbg) cout<<fixed<<r3<<" "<<fixed<<r6<<" "<<fixed<<r12<<" "<<fixed<<E<<endl;
			}
		}
cout<<fixed<<E<<endl;
return 0;
}
