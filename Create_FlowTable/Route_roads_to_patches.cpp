//-------------------------------------------------------------------------------------
//  NAME:: route_roads_to_patches									
//	
// Description:: computes mean x,y,z for each pch sorts pches by elevation									
//	computes slope and gamma for neighbours of each pch 		    
//		- gamma is 0 if slope is negative						
//                                                              
//-------------------------------------------------------------------------------------

#include <fstream>
#include <stdio.h>
#include "blender.h"
#include "file_io.h" 

void route_roads_to_patches(struct flow_struct *flow_table, int num_patches, double cell, int fl_flag)
{
	void find_min_flna(struct flow_struct *,	int, int *);
	int	 find_mmax_flna();

 	/* local variable declarations */
	int    inx;
	int    pch;
	struct adj_struct *aptr;

	// calculate gamma for each neighbour
	for (pch = 1; pch <= num_patches; pch++){
		aptr = flow_table[pch].adj_list;

		if (flow_table[pch].land == 2) {
			if (fl_flag)
				find_min_flna(flow_table, pch,&inx);
			else
				find_max_flna(flow_table, pch,&inx);

		  	flow_table[pch].stream_ID.patch = flow_table[inx].patchID;
		  	flow_table[pch].stream_inx = inx;
		}
	}
	return;
}



