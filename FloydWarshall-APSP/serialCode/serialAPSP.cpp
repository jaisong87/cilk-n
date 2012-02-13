#include<iostream>
#include<iomanip>
#include<cassert>
using namespace std;

double ** Matrix = NULL; // Bad code!! Need to add a class
int ** updateMap = NULL;
int N;
static int updateCount = 0, updateCall = 0;
bool dbg = false;//true;

//Initialize the global matrix by memory allocation and assigning values
void initializeMatrix(int N)
{
Matrix = new double*[N+1];
updateMap = new int*[N+1];

for(int i=1;i<=N;i++)
	{
	Matrix[i] = new double[N+1];
	updateMap[i] = new int[N+1];
	}

for(int i=1;i<=N;i++)
	for(int j=1;j<=N;j++)
		{
		Matrix[i][j] = 0;
		updateMap[i][j] = 0;
		}
return;
}

void updateMatrix(int i, int j, int k)
	{
	updateCall++;
	//if( k <= (N-2) && (k<i) && (i<N)&&(k<j)&&(j<=N)&&(k>=1))
		{

		if(dbg) {
			cout<<i<<" "<<j<<" "<<k<<endl;
			}

			{
			Matrix[i][j] = min(Matrix[i][j], (Matrix[i][k] + Matrix[k][j]) );
			updateCount++;
			updateMap[i][j]++;
			}
		}
	}

//Initialize the global matrix by memory allocation and assigning values
void freeMatrix()
{

for(int i=1;i<=N;i++)
        {
        delete[] Matrix[i];
        delete[] updateMap[i];
        }

delete[] Matrix;
delete[] updateMap;
N = 0;
return;
}


void printMat()
{
for(int i=1;i<=N;i++,cout<<endl)
	for(int j=1;j<=N;j++)
		cout<<setw(10)<<setprecision(2)<<fixed<<Matrix[i][j];//<<"("<<updateMap[i][j]<<")";
}

int main()
{
int q;
//cout<<"Enter the size-parameter (N) for the matrix with size [ N X N ]"; 
while(cin>>q)
	{
//assert(q&(q-1) == 0); //q has to be a non-negative integer

N = q;  //(1<<q); 
initializeMatrix(N);

//cout<<endl<<"Input matrix of size "<<N<<" X "<<N<<endl;

for(int i=1;i<=N;i++)
	for(int j=1;j<=N;j++)
		{
			cin>>Matrix[i][j];		
		}

//cout<<" ========= Input Matrix ============== "<<endl;
//printMat();

for(int k=1;k<(N-1);k++)
	for(int i=1;i<N;i++)
		for(int j=1;j<=N;j++)
			updateMatrix(i,j,k);	 

//cout<<" Matrix was updated "<<updateCount<<" times out of "<<updateCall<<" calls...."<<endl;

//cout<<" ======== Output Matrix ============= "<<endl;
printMat();
cout<<endl;

freeMatrix();

	}

return 0;
}
