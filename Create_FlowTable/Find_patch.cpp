//--------------------------------------------------------------------------------
//  NAME:: find_patch										        
//                                                              
//  DESCRIPTION                                                 
//		- locates a patch based on ID value						
//--------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "file_io.h"
#include "blender.h"

int	find_patch(int num_patches, struct flow_struct *flow_table,  int patchID)
{
	int fnd,inx;

	fnd = 0;
	inx = 0;

	while ( (fnd == 0) && (inx <= num_patches) ) {
		 if ( flow_table[inx].patchID == patchID)
			 fnd = inx;
		 else
			 inx += 1;
	}
	
	return(fnd);
}



