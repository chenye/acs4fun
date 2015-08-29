///////////////////////////////////////////////////////////////////////////////
//  Class Name: Ant Colony System for Function Optimization                  //
//  Version:    0.1                                                          //
//  Author:     Ye Chen, @ Sichuan University (when writing this code)       //
//  E-mail:     chenye84@gmail.com                                           //
//  Date:       2006-07-01                                                   //
///////////////////////////////////////////////////////////////////////////////

#include "CACS4Fun.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

CAcs4Fun::CAcs4Fun()
{
	need_release=false;			//indicate that memory hasn't been allocated
								//and needs release operation
}

CAcs4Fun::~CAcs4Fun()
{
	if(need_release)
		release_vars();
}

void CAcs4Fun::release_vars()
{
	int i,j;

	for (i=0; i<num_layers; i++)
	{
		for (j=0; j<10; j++)
			delete[] tau[i][j];
		delete[] tau[i];
	}
	delete[] tau;

	for (i=0; i<num_ants; i++)
		delete[] ant[i];
	delete[] ant;

	for (i=0; i<num_ants; i++)
		delete[] x[i];
	delete[] x;
	delete[] gbx;

	delete[] fit;
	delete[] y;
	delete[] gbant;
	delete[] ibant;

	need_release=false;
}

void CAcs4Fun::allocate_vars()
{
	int i, j;

	if(need_release==true)
		release_vars();
	//allocate memory for variables
	//float ***tau;
	tau=new float**[num_layers];
	for (i=0; i<num_layers; i++)
	{
		tau[i]=new float*[10];
		for (j=0; j<10; j++)		
			tau[i][j]=new float[10 +1];	//the last element is used to store the accumulate value of the foregoing 10 numbers
	}

	//int **ant;
	ant=new int*[num_ants];
	for (i=0; i<num_ants; i++)
		ant[i]=new int[num_layers];

	//float *fit;
	fit=new float[num_ants];

	//float *y;
	y=new float[num_ants];

	//int *gbant;
	gbant=new int[num_layers];

	//int *ibant;
	ibant=new int[num_layers];

	//float **x;
	x=new float*[num_ants];
	for (i=0; i<num_ants; i++)
		x[i]=new float[dimension];

	//float *gbx;
	gbx=new float[dimension];

	need_release=true;
	layers_per_dimension=num_layers/dimension;
}

void CAcs4Fun::init_vars()
{
	int i, j, k;
	//init variables for global best ant
	gbfit=100000;
	gby=0;

	//init tau
	for (i=0; i<num_layers; i++)
	{
		for (j=0; j<10; j++)
		{
			for (k=0; k<10; k++)
				//tau[i][j][k]=tau_max;
				tau[i][j][k]=tau0;
			//tau[i][j][10]=tau_max*10;		//sum of the foregoing ten tau
			tau[i][j][10]=tau0*10;
		}
	}
}

void CAcs4Fun::ant_move(int ant_no, int layer)
{
	int last_pos, next_pos;
	int i;
	float t;

	//last_pos stands for the node number of the ant chooses in the last move
	//and it will be 0 for the first move of each dimension of variable
	//if(layer==0)
	if(layer%layers_per_dimension==0)
		last_pos=0;
	else
		last_pos=ant[ant_no][layer-1];
	t=(float)rand()/RAND_MAX;
	if ( t<p0 )
	{
		//select a path with maximum pheromone
		next_pos=0;
		for (i=1; i<10; i++)
			if (tau[layer][last_pos][i]>tau[layer][last_pos][next_pos])
				next_pos=i;
		ant[ant_no][layer]=next_pos;
	}
	else
	{
		//select a path randomly according to pseudo-random-proportional rule
		next_pos=0;
		t=(float)rand()/RAND_MAX*tau[layer][last_pos][10];
		for (i=0; i<10 -1; i++)		//the for statement just need to run till i==8, as i==9 is the last choise
			if(tau[layer][last_pos][i]>t)
				break;
			else
				t-=tau[layer][last_pos][i];
		ant[ant_no][layer]=i;
	}
}

void CAcs4Fun::local_update(int ant_no, int layer)
{
	float t, tt;
	int last_pos, cur_pos;
	
	//last_pos stands for the node number of the ant chooses in the last move
	//and it will be 0 for the first move of each dimension of variable
	//if(layer==0)
	if(layer%layers_per_dimension==0)
		last_pos=0;
	else
		last_pos = ant[ant_no][layer-1];
	cur_pos = ant[ant_no][layer];

	t = tau[layer][last_pos][cur_pos];
	//tt = rho * t;
	tt=(1-rho)*t + rho*tau0;
	//if(tt>tau_max)
	//	tt=tau_max;
	//else if(tt<tau_min)
	//	tt=tau_min;
	tau[layer][last_pos][cur_pos]=tt;
	tau[layer][last_pos][10] += (tt-t);
}

void CAcs4Fun::get_best_ant()
{
	int i;

	ibfit=fit[0];
	ibant_no=0;
	for (i=1; i<num_ants; i++)
	{
		if (fit[i]<ibfit)
		{
			ibant_no=i;
			ibfit=fit[i];
		}
	}
	if(ibfit<gbfit)
	{
		for (i=0; i<num_layers; i++)
			gbant[i]=ant[ibant_no][i];
		for (i=0; i<dimension; i++)
			gbx[i]=x[ibant_no][i];
		gbfit=ibfit;
	}
}

void CAcs4Fun::global_update()
{
	float t, tt;
	int i;
	int last_pos, cur_pos;

	for (i=0; i<num_layers; i++)
	{
		//last_pos stands for the node number of the ant chooses in the last move
		//and it will be 0 for the first move of each dimension of variable
		if(i%layers_per_dimension==0)
		//if(i==0)
			last_pos=0;
		cur_pos=gbant[i];
		t  = tau[i][last_pos][cur_pos];
		//tt = alpha * t + 1.0/gbfit;
		tt=(1-alpha)*t + alpha/gbfit;
		tau[i][last_pos][cur_pos]=tt;
		//if(tt>tau_max)
		//	tt=tau_max;
		//else if(tt<tau_min)
		//	tt=tau_min;
		tau[i][last_pos][10] += (tt-t);
		last_pos=cur_pos;
	}
}

void CAcs4Fun::decode()
{
	int i, j, k;
	//float x[300];

	for (i=0; i<num_ants; i++)
	{
		fit[i]=0;
		for (j=0; j<dimension; j++)
		{
			x[i][j]=0;
			for (k=0; k<layers_per_dimension; k++)
			{
				x[i][j]+=ant[i][j*layers_per_dimension+k]*pow(10,-k-1);
			}
			//fit[i]+=x[j]*x[j]*100;
		}
	}
}

void CAcs4Fun::evaluate()
{
	int i, j;
	for (i=0; i<num_ants; i++)
	{
		//fit[i]=1-fabs((1-x[i][0])*x[i][0]*x[i][0]*sin(200*3.14159265*x[i][0]));
		for (j=0; j<dimension; j++)	//SphereModel, x belongs to (-5, 5]
			fit[i]+=(0.5-x[i][j])*(0.5-x[i][j])*100;
		
	}
}

void CAcs4Fun::start()
{
	int cur_layer, cur_ant;
	//int cur_best_ant;

	init_vars();
	for (cur_iter=0; cur_iter< num_iters; cur_iter++)
	{
		for (cur_layer=0; cur_layer<num_layers; cur_layer++)
		{
			for (cur_ant=0; cur_ant<num_ants; cur_ant++)
			{
				ant_move ( cur_ant, cur_layer );
				local_update ( cur_ant, cur_layer );
			}
		}
		decode();
		evaluate();
		get_best_ant();
		global_update();
		cout<<cur_iter<<" "<<gbfit<<endl;
	}
}
