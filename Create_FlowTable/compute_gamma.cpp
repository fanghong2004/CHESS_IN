//---------------------------------------------------------------------------------------
//Name:: compute_gamma             									
//
// DESCRIPTION                                                 
//	computes mean x,y,z for each pch; sorts pches by elevation									
//	computes slope and gamma for neighbours of each pch		    
//	- gamma is 0 if slope is negative						
//-------------------------------------------------------------------------------------

#include <iostream>
#include "blender.h"
 
void compute_gamma(struct flow_struct *flow_table, int num_patches, double cell, int sc_flag) 
{
 	// local fuction declarations
	struct ID_struct  sort_flow_table(struct flow_struct *,int);
	int	find_patch(int , struct flow_struct *,  int);

 	// local variable declarations
	int p; //,z,h
	int inx;
	int neigh;
	int pch;
	
	double  rise, run; //mult,
	double xrun, yrun;
	struct adj_struct *aptr;
	struct ID_struct max_ID;

	// compute mean pch values
	for (pch = 1; pch <= num_patches; pch++) {
		flow_table[pch].x = flow_table[pch].x;
		flow_table[pch].y = flow_table[pch].y;
		flow_table[pch].z = flow_table[pch].z;
		if (sc_flag == 1)
			flow_table[pch].internal_slope =flow_table[pch].internal_slope;

		flow_table[pch].flna = flow_table[pch].flna / flow_table[pch].area;
		flow_table[pch].total_gamma = 0.;
		flow_table[pch].road_dist   = 0.;
		flow_table[pch].inflow_cnt  = 0;
	}

	//flow_table[pch].internal_slope = ( double )( tan(flow_table[pch].internal_slope) );
	max_ID = sort_flow_table(flow_table, num_patches);

	// calculate gamma for each neighbour
	for (pch = 1; pch <= num_patches; pch++) {
		aptr = flow_table[pch].adj_list;
		flow_table[pch].slope = 0.0;

		for (neigh = 1; neigh <= flow_table[pch].num_adjacent; neigh ++){
			p = aptr->patchID;
			inx = find_patch(num_patches, flow_table, p);
			if (inx == 0) {
				std::cout<<"\n For patch Neighbour not found "<<flow_table[pch].patchID<< p<<std::endl;
				exit(1);
			}
			aptr->inx = inx;

			// rise is difference in elevation between two cells
			// run is distance between centorid of two neighbors
			rise =  flow_table[pch].z - flow_table[inx].z;
			xrun =  pow( (flow_table[pch].x - flow_table[inx].x), 2.0);
			yrun =  pow( (flow_table[pch].y - flow_table[inx].y), 2.0);
			run =   sqrt(xrun+yrun); // * (cell);
			aptr->slope = ( double )(rise / run );
			//aptr->gamma = ( double )(aptr->slope ); 
			aptr->gamma = ( double )(aptr->perimeter *1.* aptr->slope );

			aptr->z     = flow_table[inx].z;		

			//if neighbor's elevation is identical, then aptr->gamma is zero, which
			//means water can not flow. To avide this, we set a minimum gamma
			//if (aptr->gamma ==0.) aptr->gamma  =0.001;

			//if neighbor's elevation is higher, then aptr->gamma is less than zero, which
			//means water can not flow upslope
			if (aptr->gamma < 0.0) aptr->gamma = 0.0;

			flow_table[pch].total_gamma += aptr->gamma;
			flow_table[pch].slope       += aptr->slope ; //* aptr->gamma
				
			aptr = aptr->next;
		}


		//  divided by total_gamma
		aptr = flow_table[pch].adj_list;

		if (flow_table[pch].total_gamma == 0.0)
			flow_table[pch].slope = 0.0;
		else
			flow_table[pch].slope = flow_table[pch].slope / flow_table[pch].total_gamma;

		
		for (neigh = 1; neigh <= flow_table[pch].num_adjacent; neigh ++)
		{
			if (flow_table[pch].total_gamma != 0.0) 
				aptr->gamma  = aptr->gamma / flow_table[pch].total_gamma;
			else 
				aptr->gamma = 0.0;
			aptr = aptr->next;
		}
	}
	
	return;
}



