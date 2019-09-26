//-----------------------------------------------------------------------------------------------------------
//  NAME:: remove_pits										    
//                                                              
//  DESCRIPTION::	remove pits from flow_table by having all pixels in the pit point to 						
//  the minimum elevation patch that points to somewhere outside of the pit								
//	a pixel is in the pit if it points to a pixel that eventually has no outgoing neighbours AND is below			
//  minimum elevation as described above						
//-----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "blender.h"
#include "file_io.h"

void  remove_pits(struct flow_struct *flow_table, int num_patches, int sc_flag, double cell, std::ofstream & f1)
{
 	// local fuction declarations
	double find_top(struct flow_struct *,int,double,int *, int *,int *);
	void   adjust_pit(struct flow_struct *,	int,int,double,double);

 	// local variable declarations
	int pch;
	int num_pit, num_in_pit;
	int	edge_inx;
	int *upslope_list;
	double edge_elev, top_elev;

	// allocate list
	upslope_list = (int *)malloc(num_patches * sizeof(int));
	num_pit = 0;

	for (pch = 1; pch <= num_patches; pch++) {
		// check to see if it is a pit
		if ( (flow_table[pch].total_gamma == 0.) && ((flow_table[pch].land != 1) ) ) {
			f1 << flow_table[pch].patchID <<std::endl;
			num_pit += 1;
			num_in_pit = 1;
			upslope_list[1] = pch;
			edge_inx = 0;

			top_elev = find_top(flow_table, pch, flow_table[pch].z,
						&num_in_pit, upslope_list, &edge_inx);

			edge_elev = flow_table[edge_inx].z;

			if (top_elev != 0.0) {
				adjust_pit(flow_table, pch, edge_inx, edge_elev, cell); 
			}
		}
	} 
	std::cout << "\n Number of pits " << num_pit <<std::endl;
	f1 << " Number of pits are " << num_pit <<std::endl;
	f1.close();

	//free(upslope_list)
	return;
}

