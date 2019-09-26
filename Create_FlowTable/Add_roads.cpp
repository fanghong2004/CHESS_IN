//--------------------------------------------------------------------------------------
//  NAME                                                        
//		 add_roads										        
//                                                              
//  SYNOPSIS                                                    
// 		 add_roads( 								            
//                                                              
//  OPTIONS                                                     
//		-v 	Verbose Option										
//                                                              
//  DESCRIPTION                                                 
//																
//	computes mean x,y,z for each pch							
//	sorts pches by elevation									
//	computes slope and gamma for neighbours of each pch		    
//		- gamma is 0 if slope is negative						
//                                                              
//  PROGRAMMER NOTES                                            
//-------------------------------------------------------------------------------------
#include <fstream>
#include "file_io.h"
#include "blender.h"

void add_roads(struct flow_struct *flow_table,int num_patches,double cell)
{
 	// local variable declarations
	int inx, neigh, pch, fnd;
	struct adj_struct *aptr;
	struct ID_struct ;

	// calculate gamma for each neighbour
	for (pch = 1; pch <= num_patches; pch++) {
		aptr = flow_table[pch].adj_list;
		if (flow_table[pch].land == 2) {
			fnd = find_stream(flow_table, pch, &inx);
		
			if (fnd == 0) exit(1);
		
			if (flow_table[inx].land == 1) {
				flow_table[pch].stream_ID.patch = flow_table[inx].patchID;
				flow_table[pch].stream_inx = inx;
				neigh = flow_table[pch].num_adjacent + 1;
			}
        }
	}
	
	return;
}



