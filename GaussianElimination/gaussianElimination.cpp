#include<iostream>
#include<iomanip>
#include<vector>
using namespace std;

void print_matrix(vector<vector<double> > mat, string header)
	{
		int X = mat.size();
		int Y=mat[0].size();
		cout<<header<<endl;		
		cout<<"---------------------------------------------------"<<endl;
		for(int i=0;i<X;i++,cout<<endl)
		for(int j=0;j<Y;j++,cout<<' ')
			cout<<setw(8)<<setprecision(4)<<mat[i][j];
		cout<<"---------------------------------------------------"<<endl;
	}

int main()
{
vector<vector<double> > mat, gaussianMat;
int N;
cin>>N;

//Take input for Matrix
for(int i=0;i<N;i++)
	{
	double tmp;
	vector<double> tmpRow;
	tmpRow.push_back(0);
	for(int j=0;j<N;j++)
		{
		cin>>tmp;
		tmpRow.push_back(tmp);
		}
	mat.push_back(tmpRow);
	}
gaussianMat = mat;

//Gaussian Elimination on the matrix
for(int k=1;k<=N;k++)
{
for(int i=1;i<=N;i++)
for(int j=1;j<=N;j++)
	{
//	if (k ≤ n − 2) ∧ (k < i < n) ∧ (k < j) then
//
if( (k<=(N-2)) && ( k< i ) && (i<N)&&  ( k < j ))
	mat[i][j] = mat[i][j] - ( mat[i][k]/mat[k][k])*mat[k][j];
//gaussianMat[i][j]= mat[i][j] − (mat[i][k]/mat[k][k])*(mat[k][j]);
	}
print_matrix(mat, "Initial Matrix");
}
print_matrix(mat, "Initial Matrix");
print_matrix(gaussianMat, "Gaussian Eliminated Matrix");

return 0;
}
