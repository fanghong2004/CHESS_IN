//-------------------------------------------------------------------------------------
//  NAME:: read_flow_table										
//                                                              
//  DESCRIPTION                                                 
//------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>
#include "blender.h"
#include "file_io.h" 

int read_flow_table(struct flow_struct *flow_table, int maxr, int maxc, double cell)
{
 	// local variable declarations
	int num_patches;
	int inx;
	int r,c,pch;
	int *patch=0; 
	double *dem=0;
	int *stream=0;
	int sc_flag=0;
	FILE *fl=0; 

 	// local function definitions
	void	zero_flow_table(struct flow_struct *, int, int );

	num_patches = 0;
	zero_flow_table(flow_table,maxr,maxc);

	for (r=0; r< maxr; r++) {
		for (c=0; c< maxc; c++)	{
			inx = r*maxc+c;
			if (patch[inx] == -99999) {
				std::cout<<"error in patch file use of -99999 as a patch label not allowed"<<std::endl;
				exit(1);
			}

			pch = find_patch(num_patches,flow_table,patch[inx]);
			if (pch == 0) {
				num_patches += 1;
				pch = num_patches;
			}
			flow_table[pch].patchID = patch[inx];
			flow_table[pch].area   += 1;
			flow_table[pch].x      += (double) (1.0 * r);
			flow_table[pch].y      += (double) (1.0 * c);
			flow_table[pch].z      += (double) (1.0 * dem[inx]);

			flow_table[pch].num_adjacent += check_neighbours(r, c, patch, dem, stream, &flow_table[pch], 
				flow_table[pch].num_adjacent, maxr, maxc, sc_flag, cell) ;
		}		
	}
	std::cout<<"\n Total number of patches is "<< num_patches<<std::endl;

	return(num_patches);
}



