///////////////////////////////////////////////////////////////////////////////
//  Ant Colony System for General Function Optimization                      //
//  Version:    0.1                                                          //
//  Author:     Ye Chen @ Sichuan University (when writing this code)       //
//  E-mail:     chenye84@gmail.com                                           //
//  Date:       2006-07-01                                                   //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "cacs4fun.h"

using namespace std;

int main()
{
	srand(time(NULL));
	//srand(0);
	CAcs4Fun acs;
	
	// Set parameters
	acs.alpha=0.2;      // alpha in pheromone global update rule
	acs.num_ants=20;    // number of ants to be used to search the solution space
	acs.num_iters=500;  // number of iteration
	acs.num_layers=60;  // number of layers or number of digits to encode a solution
	acs.dimension=10;   // dimension of the solutions, using 60 layers to encode 10 dimension means
	                    // 6 layers/digits are used to represent one dimension, or one real number
	acs.p0=0.8;         // the probability of using pseudo-propotional selection method
	acs.rho=0.2;        // rho is used in pheromone local update rule
	acs.tau0=0.01;      // initial value for the pheromone intensities
	
	acs.allocate_vars();
	acs.start();
	cout<<acs.gbfit<<endl;  // Get the objective function value for the global best ant
	
	// Output each dimension of the global best ant, i.e., the best solution found by the ant colony algorithm
	for (int i=0; i<acs.dimension; i++)
		cout<<"X["<<i<<"]="<<acs.gbx[i]<<endl;

	return 0;
}

