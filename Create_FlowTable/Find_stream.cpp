//----------------------------------------------------------------------
//  NAME:: find_stream											
//                                                              
//  DESCRIPTION::  recursive routing to find the nearest stream link			
//----------------------------------------------------------------------
#include "blender.h"
 
int find_stream( struct flow_struct *flow_table,int curr, int *str_inx )
{
	struct	adj_struct *aptr;
	long int i, inx, fnd;

	fnd = 0;
	aptr = flow_table[curr].adj_list;

	// check to see if we are at the edge of the pit, find min elevation of
	// apixel draining from the pit 									
	if ( flow_table[curr].land != 1 ) {
		i = 1;
		while  (( i <= flow_table[curr].num_adjacent) && (fnd == 0) ) {
			inx = aptr->inx;

			if (aptr->gamma > 0.0) 
				fnd = ( int )find_stream(flow_table, inx, str_inx);

			aptr = aptr->next;
			i   += 1;
		} 
	}
	else {
		fnd = 1;
		*str_inx = curr;
	}

	return(fnd);
} 

