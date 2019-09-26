//---------------------------------------------------------------------------------------
//  NAME:: compute_drainage_density								
//                                                          
//  DESCRIPTION                                                 
//	computes  drainage_density for basin
//---------------------------------------------------------------------------------------

#include "blender.h"
#include <iostream>

void compute_drainage_density(struct flow_struct *flow_table, int num_patches, double cell)
{
 	// local variable declarations
	int inx;
	int neigh;
	int pch;
	int n_adjacent;
	double run, xrun,yrun;
	double drainage_density, total_area;
	struct adj_struct *aptr;
	struct ID_struct;

	total_area = 0.0;
	// send area to  each upslope neighbour for each patch in the sort list

	drainage_density = 0.0;
	n_adjacent = 0;

	for (pch = 1; pch <= num_patches; pch++) {
		aptr = flow_table[pch].adj_list;
		total_area += flow_table[pch].area;
		
		// check to see if it is a stream patch
		// if is is, add the distance to downstream patch
		if ( (flow_table[pch].land == 1) )	{
			n_adjacent = 0;
			for (neigh = 1; neigh <= flow_table[pch].num_adjacent; neigh ++){
				inx = aptr->inx;

				if (aptr->gamma > 0.0) {	
					xrun = (double)( pow( (flow_table[pch].x - flow_table[inx].x), 2.0) );
					yrun = (double)( pow( (flow_table[pch].y - flow_table[inx].y), 2.0) );
					run =  (double)( sqrt(xrun+yrun) * (cell) );
				
					drainage_density += run;
					if (flow_table[inx].land != 1)
						std::cout<<"Downstream patch from is not a stream "<<flow_table[pch].patchID<<" "<<
						flow_table[inx].patchID << std::endl;
						n_adjacent += 1;
					}

				aptr = aptr->next;
			}
			
			if (n_adjacent == 0) {
				std::cout << "STREAM to outlet " << flow_table[pch].patchID << std::endl;
				drainage_density += sqrt(double(flow_table[pch].area * cell));
			}
		}	
	}

	drainage_density = (double)(drainage_density/(cell * total_area));
	std::cout << "Drainage Density is " << drainage_density << std::endl;

	return ;
}



