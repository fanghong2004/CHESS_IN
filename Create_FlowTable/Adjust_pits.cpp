//--------------------------------------------------------------
//  NAME: adjust_pit												
//
//  DESCRIPTION                                                 
//	have pit point to patch pointed to by the lowest edge 	    
//--------------------------------------------------------------
#include <iostream>
#include "blender.h"

void	adjust_pit(struct flow_struct *flow_table,	int curr, int edge_inx, double edge_elev, double cell)
 {
	long int  j;
	double  total_perimeter;
	double  xrun, yrun;
	double  rise;
	struct	adj_struct *aptr;

	aptr = flow_table[curr].adj_list;
	total_perimeter = aptr->perimeter;

	for (j=1; j < flow_table[curr].num_adjacent; j++) {
		aptr = aptr->next;
		total_perimeter += aptr->perimeter;
	}

	aptr->next = new adj_struct;

	if (!aptr->next){
		std::cout<<"\n Not enough memory"<<std::endl;
		exit(0);
	}

	aptr = aptr->next;
	aptr->patchID = flow_table[edge_inx].patchID;

	aptr->perimeter = total_perimeter;
	aptr->z   = ( double )edge_elev;
	aptr->inx = edge_inx;

	xrun = (flow_table[curr].x - flow_table[edge_inx].x);
	xrun = xrun * xrun;
	yrun = (flow_table[curr].y - flow_table[edge_inx].y);
	yrun = yrun * yrun;
	rise = flow_table[curr].z -flow_table[edge_inx].z;
			
	if ((yrun + xrun) <= 0.000000001)
		aptr->slope = rise;
	else
		//aptr->slope = ( double )(rise / (sqrt(xrun+yrun) * (cell) ) );
		aptr->slope = ( double )(rise / sqrt(xrun+yrun));

	aptr->gamma = 1.0;
	flow_table[curr].total_gamma = 	( double )(aptr->perimeter *aptr->slope / (cell) );
	flow_table[curr].z = ( double )( edge_elev+MIN_RISE );
	flow_table[curr].num_adjacent += 1;

	return ;  
} 

