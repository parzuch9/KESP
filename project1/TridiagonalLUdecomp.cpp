//  Solves linear equations for simple tridiagonal matrix using LU decomposition
//  This is armadillo version that calls the function solve. 
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <armadillo>
#include <cstdlib>
#include "time.h"
// use namespace for output and input
using namespace std;
using namespace arma;

// object for output files
ofstream ofile;
// Functions used
double f(double x){return 100.0*exp(-10.0*x);
}
double exact(double x) {return 1.0-(1-exp(-10))*x-exp(-10*x);}

// Begin main program
int main(int argc, char *argv[]){
  int exponent; 
    string filename;
    // We read also the basic name for the output file and the highest power of 10^n we want
    if( argc <= 1 ){
          cout << "Bad Usage: " << argv[0] <<
              " read also file name on same line and max power 10^n" << endl;
          exit(1);
    }
        else{
        filename = argv[1]; // first command line argument after name of program
        exponent = atoi(argv[2]);
    }
    // Loop over powers of 10
    for (int i = 1; i <= exponent; i++){
      int  n = (int) pow(10.0,i);
      // Declare new file name
      string fileout = filename;
      // Convert the power 10^i to a string
      string argument = to_string(i);
      // Final filename as filename-i-
      fileout.append(argument);
      double h = 1.0/(n);
      double hh = h*h;
      n = n-1;  //  shift so that only points between endpoints are studied
      mat A = zeros<mat>(n,n);
      // Set up arrays for the simple case
      vec b(n);  vec x(n);

      clock_t start, finish;
      start = clock();

      A(0,0) = 2.0;  A(0,1) = -1;  x(0) = h;  b(0) =  hh*f(x(0)); 
      x(n-1) = x(0)+(n-1)*h; b(n-1) = hh*f(x(n-1)); 
      for (int i = 1; i < n-1; i++){ 
        x(i) = x(i-1)+h; 
	b(i) = hh*f(x(i));
        A(i,i-1)  = -1.0;
        A(i,i)    = 2.0;
        A(i,i+1)  = -1.0;
      }
      A(n-1,n-1) = 2.0; A(n-2,n-1) = -1.0; A(n-1,n-2) = -1.0;
  // solve Ax = b
      vec solution  = solve(A,b);

      finish = clock();
      cout<<"Time: "<< (double)(finish - start)/((double)CLOCKS_PER_SEC)<<endl;

      ofile.open(fileout);
      ofile << setiosflags(ios::showpoint | ios::uppercase);
      for (int i = 0; i < n ;i++) {
	double RelativeError = fabs((exact(x(i))-solution(i))/exact(x(i)));
	ofile << setw(15) << setprecision(8) << x(i);
	ofile << setw(15) << setprecision(8) << solution(i);
	ofile << setw(15) << setprecision(8) << exact(x(i));
        ofile << setw(15) << setprecision(8) << log10(RelativeError) << endl;
      }
      ofile.close();
    }
    return 0;
}





