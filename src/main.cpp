///////////////////////////////////////////////////////////////////////////////
//  Class Name: Ant Colony System for Function Optimization                  //
//  Version:    0.1                                                          //
//  Author:     Ye Chen, @ Sichuan University (when writing this code)       //
//  E-mail:     chenye84@gmail.com                                           //
//  Date:       2006-07-01                                                   //
///////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include "cacs4fun.h"

void main()
{
	srand(time(NULL));
	//srand(0);
	CAcs4Fun acs;
	acs.alpha=0.2;
	acs.num_ants=20;
	acs.num_iters=500;
	acs.num_layers=60;	//layers_per_dimension=60/10=6
	acs.dimension=10;
	acs.p0=0.8;
	acs.rho=0.2;
	acs.tau0=0.01;
	acs.allocate_vars();
	acs.start();
	cout<<acs.gbfit<<endl;
	for (int i=0; i<acs.dimension; i++)
		cout<<"X["<<i<<"]="<<acs.gbx[i]<<endl;
}
