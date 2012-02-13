/***********************************
 * Cilk-N
 *
 * Sample Cilk-code for Gaussian elimination
 *
 * Jaison george, Ajeesh Elikkottil
 *
 ************************************/
#include<iostream>
#include<cstdio>
#include<iomanip>
#include<cassert>
#include<sstream>
#include<bitset>
#include"cilkview.h"
using namespace std;

#define BASE_CASE_SIZE 1 //This means anything less than or equal to 4X4 matrix will go through serial code

// These are global variables that are used in the program
double ** Matrix = NULL; // Bad code!! Need to add a class
bool *** updateMap = NULL;
int N;
static int updateCount = 0, updateCall = 0;
bool dbg = false;//true;

/* Initialize the global matrix by memory allocation and assigning values
 * Initialize all global variables here
 */
void initializeMatrix(int N)
{
Matrix = new double*[N+1];
updateMap = new bool**[N+1];

for(int i=1;i<=N;i++)
	{
	Matrix[i] = new double[N+1];
	updateMap[i] = new bool*[N+1];
	}

for(int i=1;i<=N;i++)
	for(int j=1;j<=N;j++)
		{
		Matrix[i][j] = 0;
		updateMap[i][j] = new bool[N+1];
		for(int k=1;k<=N;k++)
			updateMap[i][j][k] = false;
		}
return;
}

/* Revert back the global matrix by memory allocation and assigning values
 * Dealloc, free of all global variables to be done here
 */
void freeMatrix()
{
for(int i=1;i<=N;i++)
        for(int j=1;j<=N;j++)
                {
                delete[] updateMap[i][j];
                }

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

/* This is the upate function
 * This is the only function that changes between APSP and Gaussian elimination
 */
void updateMatrix(int i, int j, int k)
	{
	updateCall++;
	if( k <= (N-2) && (k<i ) && (i<N)&&(k<j) && !updateMap[i][j][k])
		{

		if(dbg) {
			cout<<i<<" "<<j<<" "<<k<<endl;
			}

		//Matrix[j][i] = Matrix[j][i] - (Matrix[k][i]/Matrix[k][k])*Matrix[j][k];
		//if(false == updateMap[i][j])
			{
			Matrix[i][j] = Matrix[i][j] - (Matrix[i][k]*Matrix[k][j])/Matrix[k][k];
			updateCount++;
		/*	stringstream ss1; ss1<<k<<",";
			updateMap[i][j]+=ss1.str(); */
			updateMap[i][j][k] = true;
			}
		}
	}

/* Just a display function
 * same Format of Sample-I/O is followed
 */ 
void printMat()
{
for(int i=1;i<=N;i++,cout<<endl/*printf("\n")*/)
	for(int j=1;j<=N;j++)
		cout<<setw(10)<<setprecision(2)<<fixed<<Matrix[i][j];//<<"{"<<updateMap[i][j]<<"}";
		//printf("%5.2lf ",Matrix[i][j]);
}

/* @Class subMat - This is basically the specification of a submatrix
 * This class contains members that would clearly identify the boundaries of the submatrix
 */
class subMat{
int xs, xe, ys, ye; //XStart, Xend, YStart, Yend

public:
subMat(int x1,int x2,int y1,int y2):xs(x1),xe(x2),ys(y1),ye(y2) { 
assert(x1<=x2);
assert(y1<=y2);
}

int getxStart() { return xs; }
int getxEnd() { return xe; }
int getyStart() { return ys; }
int getyEnd() { return ye; }
int getSize() { return (xe-xs+1)*(ye-ys+1); } 

/* Use this to get X11 - top left half */
subMat get11() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xs,xmid,ys,ymid); //First quadrant
			return subMatrix;		
		}

/* Use this to get X12 - top right half */
subMat get12() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xmid+1,xe,ys,ymid); //First quadrant
			return subMatrix;		
		}


/* Use this to get X21 - bottom left half */
subMat get21() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xs,xmid,ymid+1,ye); //First quadrant
			return subMatrix;		
		}

/* Use this to get X22 - bottom right half */
subMat get22() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xmid+1,xe,ymid+1,ye); //First quadrant
			return subMatrix;		
		}

};

//These are divide and conquer functions that helps in parallelizing the task
void funcA(subMat X, subMat U, subMat V, subMat W);
void funcB(subMat X, subMat U, subMat V, subMat W);
void funcC(subMat X, subMat U, subMat V, subMat W);
void funcD(subMat X, subMat U, subMat V, subMat W);

/* funcA -parallel divide & conquer function that performs parallel gaussian elimination
 * Calls are made to funcB, funcC, funcD
 */
void funcA(subMat X, subMat U, subMat V, subMat W)
{
if(dbg)
	{
		cout<<"calling funcA with Xsize "<<X.getSize()<<endl;
	}
//@TOTO => Opimization checks
if(BASE_CASE_SIZE >= X.getSize() ) //This means a 1*1 matrix
	{
		//Call the updateFunction
		int i1,i2, j1, j2, k1, k2;
		
		i1 = X.getxStart();
		i2 = X.getxEnd();
		
		j1 = X.getyStart();
		j2 = X.getyEnd();

		k1 = W.getxStart();
		k2 = W.getxEnd();

		for(int k=k1;k<=k2;k++)
			for(int i=i1;i<=i2;i++)
				for(int j=j1;j<=j2;j++)
					updateMatrix(i,j,k);

		return; 
	}

funcA(X.get11(), U.get11(), V.get11(), W.get11());

// <parallel>
	cilk_spawn funcB(X.get12(), U.get11(), V.get12(), W.get11());
		   funcC(X.get21(), U.get21(), V.get11(), W.get11()); 	
	cilk_sync;

//</parallel>
	
	cilk_spawn funcD(X.get22(), U.get21(), V.get12(), W.get11());
	           funcA(X.get22(), U.get22(), V.get22(), W.get22()); 	
	cilk_sync;	
	
// <parallel>
	cilk_spawn funcB(X.get21(), U.get22(), V.get22(), W.get21());
		   funcC(X.get12(), U.get12(), V.get22(), W.get22()); 	
	cilk_sync;

//</parallel>

	funcD(X.get11(), U.get12(), V.get21(), W.get22());
return;
}


void funcB(subMat X, subMat U, subMat V, subMat W)
{
if(dbg)
	{
		cout<<"calling funcB with Xsize "<<X.getSize()<<endl;
	}

//@TOTO => Opimization checks

/*
if(1 == X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i, j, k;
                i = X.getxStart(); 
                j = X.getyStart(); 
                k = W.getxStart();
                updateMatrix(i,j,k);
                return;
        }
*/
//@TOTO => Opimization checks
if(BASE_CASE_SIZE >= X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i1,i2, j1, j2, k1, k2;

                i1 = X.getxStart();
                i2 = X.getxEnd();

                j1 = X.getyStart();
                j2 = X.getyEnd();

                k1 = W.getxStart();
                k2 = W.getxEnd();
	
		
                for(int k=k1;k<=k2;k++)
                        for(int i=i1;i<=i2;i++)
                                for(int j=j1;j<=j2;j++)
                                        updateMatrix(i,j,k);

                return;
        }


//<parallel>
	cilk_spawn funcB(X.get11(), U.get11(), V.get11(), W.get11() );
		   funcB(X.get12(), U.get11(), V.get12(), W.get11() );
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcD(X.get21(), U.get21(), V.get11(), W.get11() );
		   funcD(X.get22(), U.get21(), V.get12(), W.get11() );
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcB(X.get21(), U.get22(), V.get21(), W.get22() );
		   funcB(X.get22(), U.get22(), V.get22(), W.get22() );
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcD(X.get11(), U.get12(), V.get21(), W.get22() );
		   funcD(X.get12(), U.get12(), V.get22(), W.get22() );
	cilk_sync;
//</parallel>

return;
}


void funcC(subMat X, subMat U, subMat V, subMat W)
{

if(dbg)
	{
		cout<<"calling funcC with Xsize "<<X.getSize()<<endl;
	}
//@TOTO => Opimization checks

/*if(1 == X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i, j, k;
                i = X.getxStart(); 
                j = X.getyStart(); 
                k = W.getxStart();
                updateMatrix(i,j,k);
                return;
        }
*/
//@TOTO => Opimization checks
if(BASE_CASE_SIZE >= X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i1,i2, j1, j2, k1, k2;

                i1 = X.getxStart();
                i2 = X.getxEnd();

                j1 = X.getyStart();
                j2 = X.getyEnd();

                k1 = W.getxStart();
                k2 = W.getxEnd();

                for(int k=k1;k<=k2;k++)
                        for(int i=i1;i<=i2;i++)
                                for(int j=j1;j<=j2;j++)
                                        updateMatrix(i,j,k);

                return;
        }

//<parallel>
	cilk_spawn funcC(X.get11(), U.get11(), V.get11(), W.get11());
		   funcC(X.get21(), U.get21(), V.get11(), W.get11());
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcD(X.get12(), U.get11(), V.get12(), W.get11());
		   funcD(X.get22(), U.get21(), V.get12(), W.get11());
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcC(X.get12(), U.get12(), V.get22(), W.get22());
		   funcC(X.get21(), U.get22(), V.get22(), W.get22());
	cilk_sync;
//</parallel>

//<parallel>
	cilk_spawn funcD(X.get11(), U.get12(), V.get21(), W.get22());
		   funcD(X.get21(), U.get22(), V.get21(), W.get22());
	cilk_sync;
//</parallel>

return;
}


void funcD(subMat X, subMat U, subMat V, subMat W)
{

if(dbg)
	{
		cout<<"calling funcD with Xsize "<<X.getSize()<<endl;
	}

//@TOTO => Opimization checks

/*if(1 == X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i, j, k;
                i = X.getxStart(); 
                j = X.getyStart(); 
                k = W.getxStart();
                updateMatrix(i,j,k);
                return;
        }
*/
//@TOTO => Opimization checks
if(BASE_CASE_SIZE >= X.getSize() ) //This means a 1*1 matrix
        {
                //Call the updateFunction
                int i1,i2, j1, j2, k1, k2;

                i1 = X.getxStart();
                i2 = X.getxEnd();

                j1 = X.getyStart();
                j2 = X.getyEnd();

                k1 = W.getxStart();
                k2 = W.getxEnd();

                for(int k=k1;k<=k2;k++)
                        for(int i=i1;i<=i2;i++)
                                for(int j=j1;j<=j2;j++)
                                        updateMatrix(i,j,k);

                return;
        }

//<parallel>
	cilk_spawn funcD( X.get11(), U.get11(), V.get11(), W.get11() );
	cilk_spawn funcD( X.get12(), U.get11(), V.get12(), W.get11() );
	cilk_spawn funcD( X.get21(), U.get21(), V.get11(), W.get11() );
		   funcD( X.get22(), U.get21(), V.get12(), W.get11() );
	cilk_sync;
//</parallel>


//<parallel>
	cilk_spawn funcD( X.get11(), U.get12(), V.get21(), W.get22() );
	cilk_spawn funcD( X.get12(), U.get12(), V.get22(), W.get22() );
	cilk_spawn funcD( X.get21(), U.get21(), V.get21(), W.get22() );
		   funcD( X.get22(), U.get22(), V.get22(), W.get22() );
	cilk_sync;
//</parallel>

return;
}



int cilk_main(int argc, char* argv[])
{
int q;
//cout<<"Enter the size-parameter (N) for the matrix with size [ N X N ]"; 
int count = 0;
while(cin>>q /*&& count <1*/)
	{
count++;
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
/*printMat();
cout<<endl;
*/

subMat X(1,N,1,N), U(1,N,1,N), V(1,N,1,N), W(1,N,1,N); //All submatrices of N*N size

cilk::cilkview cv;
cv.start();

funcA(X,U,V,W); 

cv.stop();
cv.dump("PARALLEL_GAUSSIAN");

//cout<<" Matrix("<<N<<"X"<<N<<") was updated "<<updateCount<<" times out of "<<updateCall<<" calls...."<<endl;

printMat();
cout<<endl;

freeMatrix();
	}

return 0;
}
