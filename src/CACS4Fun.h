///////////////////////////////////////////////////////////////////////////////
//  Class Name: Ant Colony System for Function Optimization                  //
//  Version:    0.1                                                          //
//  Author:     Ye Chen, @ Sichuan University (when writing this code)       //
//  E-mail:     chenye84@gmail.com                                           //
//  Date:       2006-07-01                                                   //
///////////////////////////////////////////////////////////////////////////////

#pragma once

class CAcs4Fun
{
public:
	float alpha, rho, p0;
	//float tau_min, tau_max;
	float tau0;
	int dimension;
	int *gbant;				//global best ant
	float gbfit;			//global best fit
	float gby;				//global best y
	float *gbx;				//global best x
	int num_layers;			//number of layers
	int num_ants;			//number of ants
	int num_iters;			//maximum number of iterations
protected:
	float ***tau;
	int **ant;
	float *fit;
	float *y;
	float **x;				//x[i][j] stands for the j-th dimension of the variant decoded from the i-th ant
	int *ibant;				//iteration best ant
	int ibant_no;			//NO. of iteration best ant
	float ibfit;			//iteration best fit
	float iby;				//iteration best y
	int cur_iter;			//current iteration
	//int *startpos;			//start position of ...

	bool need_release;		//memory has been allocated
	int layers_per_dimension;		//number of layers to represent one dimension of the variants
									//assume to represent each dimension in the same accuracy

public:
	void allocate_vars();	//allocate memory for variables
	void release_vars();	//release memory allocated for variables
	void start();
	void init_vars();		//initialize variables used in searching process
	void ant_move(int ant_no, int layer);
							//the ant_no-th ant chooses an node in the layer-th layer
	void local_update(int ant_no, int layer);
							//the arguments are similar to function ant_move
	void get_best_ant();	//find out the best ant of current iteration and update the global best ant
	void global_update();

	virtual void evaluate();
	virtual void decode();

	CAcs4Fun();
	~CAcs4Fun();				//call release_vars
};

