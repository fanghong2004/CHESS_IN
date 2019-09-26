//-----------------------------------------------------------------------------------
//  NAME:: sort_flow_table										
//                                                              
//  DESCRIPTION:: sorts patches by elevation									
//		- using a bubble sort generates ID's based on sort list position finds
//        maximum partition list ID								
//----------------------------------------------------------------------------------
#include "blender.h"

struct ID_struct sort_flow_table(struct flow_struct *flow_table, int num_patches)
{
 	// local variable declarations
	int i,j;
	struct ID_struct max_ID;
	struct flow_struct temp_entry;

	for (i= num_patches-1; i>0; i--) 
		for (j=1; j <= i; j++){
			if (flow_table[j].z < flow_table[j+1].z) {
				temp_entry = flow_table[j];
				flow_table[j] = flow_table[j+1];
				flow_table[j+1] = temp_entry;
			}
		}

	max_ID.patch = 0;
	for (i= 1; i <= num_patches; i++) {
		if (flow_table[i].patchID > max_ID.patch) 
			max_ID.patch = flow_table[i].patchID;
		flow_table[i].ID_order = i;
	}
	
	return(max_ID);
}



