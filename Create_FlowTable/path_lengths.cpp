//---------------------------------------------------------------------------------------
//  NAME:: path_lengths											
//                                                              
//  DESCRIPTION::	recursive routing to find the nearest stream link	        
//---------------------------------------------------------------------------------------
#include <iostream>
#include "blender.h"

double	path_lengths(struct flow_struct *flow_table,
					 int curr, int *str_inx)
 {
	struct	adj_struct *aptr;
	int 	i, inx, fnd;
	double dist_neighbour;

	fnd = 0;
	aptr = flow_table[curr].adj_list;

	// check to see if we are at the edge of the pit, find min elevation of
	// apixel draining from the pit 										
	if ( flow_table[curr].land != 1 ) {
		std::cout << "\n Current patch is %d" << flow_table[curr].patchID << std::endl;
		std::cout << "\n Next patch is %d" << flow_table[aptr->inx].patchID << std::endl;

		i = 1;
		while  ( i <= flow_table[curr].num_adjacent){
			inx = aptr->inx;

			dist_neighbour = pow( 30 * (flow_table[curr].x - flow_table[inx].x), 2.0) +
					pow( 30 * (flow_table[curr].y - flow_table[inx].y), 2.0) +  
					pow((flow_table[curr].z - flow_table[inx].z), 2.0) ;

			dist_neighbour = pow(dist_neighbour, 0.5);

			flow_table[inx].path_length += aptr->gamma * dist_neighbour;

			if ((flow_table[inx].path_length ==  0.0) && (flow_table[inx].land != 1))
				flow_table[curr].path_length += aptr->gamma * path_lengths(flow_table, inx, str_inx);
			else  
				flow_table[curr].path_length += aptr->gamma * flow_table[inx].path_length;
		
			aptr = aptr->next;
			i += 1;
		} 
		flow_table[curr].path_length = flow_table[curr].path_length / flow_table[curr].total_gamma;
	} /* end if */
	else
		flow_table[curr].path_length = 0.0;

	return(flow_table[curr].path_length);
} 

