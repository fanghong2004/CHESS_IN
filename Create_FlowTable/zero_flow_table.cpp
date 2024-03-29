//---------------------------------------------------------------------------------------
//  NAME:: zero_flow_table                                                       
//                               
//  DESCRIPTION                                                 
//		- locates a patch based on ID value						
//---------------------------------------------------------------------------------------
#include "blender.h"

void zero_flow_table(struct flow_struct *flow_table, int maxr, int maxc)
{
	int r, c;
	int inx;

	for (r=0; r< maxr; r++) {
		for (c=0; c< maxc; c++) {
			inx = r*maxc+c;
			flow_table[inx].patchID = -99999;
			flow_table[inx].ID_order = -99999;
			flow_table[inx].area = 0;
			flow_table[inx].x = 0.0;
			flow_table[inx].y = 0.0;
			flow_table[inx].z = 0.0;
			flow_table[inx].land = 0;
			flow_table[inx].flna = 0.0;
			flow_table[inx].total_gamma = 0.0;
			flow_table[inx].acc_area = 0.0;
			flow_table[inx].path_length = 0.0;
			flow_table[inx].num_adjacent = 0;
			//flow_table[inx].adj_list = NULL;
			//flow_table[inx].adj_ptr = NULL;
		}
	}
	return;
}



