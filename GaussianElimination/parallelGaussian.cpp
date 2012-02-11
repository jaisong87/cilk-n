#include<iostream>
#include<iomanip>
#include<cassert>
using namespace std;

double ** Matrix = NULL; // Bad code!! Need to add a class
int N;
static int updateCount = 0, updateCall = 0;
bool dbg = false;//true;

//Initialize the global matrix by memory allocation and assigning values
void initializeMatrix(int N)
{
Matrix = new double*[N+1];
for(int i=1;i<=N;i++)
	Matrix[i] = new double[N+1];

for(int i=1;i<=N;i++)
	for(int j=1;j<=N;j++)
		Matrix[i][j] = 0;
return;
}

void updateMatrix(int i, int j, int k)
	{
	if( k <= (N-2) && (k<i ) && (i<N)&&(k<j))
		{

		if(dbg) {
			cout<<i<<" "<<j<<" "<<k<<endl;
			}

		updateCall++;
		Matrix[j][i] = Matrix[j][i] - (Matrix[k][i]/Matrix[k][k])*Matrix[j][k];
		updateCount++;
		}
	}

void printMat()
{
for(int i=1;i<=N;i++,cout<<endl)
	for(int j=1;j<=N;j++,cout<<' ')
		cout<<setw(7)<<setprecision(4)<<Matrix[i][j];
}

// @Class subMat - This is basically the specification of a submatrix
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

subMat get11() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xs,xmid,ys,ymid); //First quadrant
			return subMatrix;		
		}

subMat get12() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xmid+1,xe,ys,ymid); //First quadrant
			return subMatrix;		
		}


subMat get21() {
			int xmid = (xs+xe)>>1;
			int ymid = (ys+ye)>>1;
			subMat subMatrix(xs,xmid,ymid+1,ye); //First quadrant
			return subMatrix;		
		}

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

void funcA(subMat X, subMat U, subMat V, subMat W)
{
if(dbg)
	{
		cout<<"calling funcA with Xsize "<<X.getSize()<<endl;
	}
//@TOTO => Opimization checks
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

funcA(X.get11(), U.get11(), V.get11(), W.get11());

// <parallel>
	funcB(X.get12(), U.get11(), V.get12(), W.get11());
	funcC(X.get21(), U.get21(), V.get11(), W.get11()); 	
//</parallel>
	
	funcD(X.get22(), U.get21(), V.get12(), W.get11());
	funcA(X.get22(), U.get22(), V.get22(), W.get22()); 	

// <parallel>
	funcB(X.get21(), U.get22(), V.get22(), W.get21());
	funcC(X.get12(), U.get12(), V.get22(), W.get22()); 	
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

//<parallel>
funcB(X.get11(), U.get11(), V.get11(), W.get11() );
funcB(X.get12(), U.get11(), V.get12(), W.get11() );
//</parallel>

//<parallel>
funcD(X.get21(), U.get21(), V.get11(), W.get11() );
funcD(X.get22(), U.get21(), V.get12(), W.get11() );
//</parallel>

//<parallel>
funcB(X.get21(), U.get22(), V.get21(), W.get22() );
funcB(X.get22(), U.get22(), V.get22(), W.get22() );
//</parallel>

//<parallel>
funcD(X.get11(), U.get12(), V.get21(), W.get22() );
funcD(X.get12(), U.get12(), V.get22(), W.get22() );
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

//<parallel>
funcC(X.get11(), U.get11(), V.get11(), W.get11());
funcC(X.get21(), U.get21(), V.get11(), W.get11());
//</parallel>

//<parallel>
funcD(X.get12(), U.get11(), V.get12(), W.get11());
funcD(X.get22(), U.get21(), V.get12(), W.get11());
//</parallel>

//<parallel>
funcC(X.get12(), U.get12(), V.get22(), W.get22());
funcC(X.get21(), U.get22(), V.get22(), W.get22());
//</parallel>

//<parallel>
funcD(X.get11(), U.get12(), V.get21(), W.get22());
funcD(X.get21(), U.get22(), V.get21(), W.get22());
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

//<parallel>
funcD( X.get11(), U.get11(), V.get11(), W.get11() );
funcD( X.get12(), U.get11(), V.get12(), W.get11() );
funcD( X.get21(), U.get21(), V.get11(), W.get11() );
funcD( X.get22(), U.get21(), V.get12(), W.get11() );
//</parallel>


//<parallel>
funcD( X.get11(), U.get12(), V.get21(), W.get22() );
funcD( X.get12(), U.get12(), V.get22(), W.get22() );
funcD( X.get21(), U.get21(), V.get21(), W.get22() );
funcD( X.get22(), U.get22(), V.get22(), W.get22() );
//</parallel>

return;
}



int main()
{
int q;
cout<<"Enter the size-parameter (q) for the matrix with size [ 2^q X 2^q ]"; cin>>q;
assert(q>=0); //q has to be a non-negative integer

N = (1<<q); 
initializeMatrix(N);

cout<<endl<<"Input matrix of size "<<N<<" X "<<N<<endl;

for(int i=1;i<=N;i++)
	for(int j=1;j<=N;j++)
		{
			cin>>Matrix[i][j];		
		}

cout<<" ========= Input Matrix ============== "<<endl;
printMat();

subMat X(1,N,1,N), U(1,N,1,N), V(1,N,1,N), W(1,N,1,N); //All submatrices of N*N size

funcA(X,U,V,W); 
cout<<" Matrix was updated "<<updateCount<<" times out of "<<updateCall<<" calls...."<<endl;

cout<<" ======== Output Matrix ============= "<<endl;
printMat();

return 0;
}
