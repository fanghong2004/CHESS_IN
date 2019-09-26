//---------------------------------------------------------------------------------------
//  NAME:: check_neighbours										
//                                                              
//  DESCRIPTION                                                 
//		- examines the neighbourhood of the patch figures out if it is at a border						
//		at any border, length of perimeter is added 			
//---------------------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include "blender.h"
#include "file_io.h"

int	check_neighbours_stream(int er, int ec, int *patch, double *dem, int *stream, struct flow_struct *flow_entry, int num_adj,
	int maxr, int maxc, int sc_flag, double cell)
{
	/* local function declarations */
	struct adj_struct  *check_list(int, int, adj_struct *);
	int check_list_min();
	int r, c;
	int p_neigh;
	int stream_neigh;
	int new_adj = 0;
	double ele_neigh = 0.;

	// for now streams don't point anywhere 			
	for (r = -1; r <= 1; r++) {
		for (c = -1; c <= 1; c++) {
			// don't look at neighbours beyond the edge 
			if ((er + r < maxr) && (er + r >= 0) && (ec + c < maxc) && (ec + c >= 0)) {

				// is the neighbour a different patch  or is it outside the basin - in which case we ignore it */
				// also, for stream pixels, ignore non-stream neighbours */
				p_neigh = patch[(er + r)*maxc + (ec + c)];
				stream_neigh = stream[(er + r)*maxc + (ec + c)];
				ele_neigh = dem[(er + r)*maxc + (ec + c)];

				//Start is neigh
				if ((p_neigh != flow_entry->patchID) && (p_neigh > 0) && (stream_neigh>0)) {

					/* create a list of neighbours if it does not exist already */
					if ((num_adj == 0)) {
						new_adj = 1;
						num_adj = 1;
						flow_entry->adj_list = new adj_struct;

						flow_entry->adj_list->patchID = p_neigh;
						flow_entry->adj_list->perimeter = 0.0;
						flow_entry->adj_list->next = NULL;
						flow_entry->adj_ptr = flow_entry->adj_list;
					}
					else {
						// search list for other entries for this neighbour 
						flow_entry->adj_ptr = flow_entry->adj_list;
						flow_entry->adj_ptr = (struct adj_struct *)check_list(
							p_neigh, num_adj, flow_entry->adj_ptr);

						// is this the first instance of the neighbour
						if ((flow_entry->adj_ptr->patchID != p_neigh) && (p_neigh > 0))

						{
							// land processing 
							if (flow_entry->land != 1) { //&& (ele_neigh < flow_entry->z )
								flow_entry->adj_ptr->next = new adj_struct;
								if (!flow_entry->adj_ptr->next){
									std::cout << "\nMemory Allocation Failed for " << flow_entry->patchID << std::endl;
									exit(1);
								}

								flow_entry->adj_ptr = flow_entry->adj_ptr->next;
								new_adj += 1;
								num_adj += 1;
								flow_entry->adj_ptr->patchID = p_neigh;
								flow_entry->adj_ptr->perimeter = 0.0;
								flow_entry->adj_ptr->next = NULL;

							} // end land processing 
							else { 		// stream processing
								if (stream_neigh != -9999) {  //exclude non-river type cells

									flow_entry->adj_ptr->next = new adj_struct;
									if (!flow_entry->adj_ptr->next){
										std::cout << "\nMemory Allocation Failed for " << flow_entry->patchID << std::endl;
										exit(1);
									}

									flow_entry->adj_ptr = flow_entry->adj_ptr->next;
									new_adj += 1;
									num_adj += 1;
									flow_entry->adj_ptr->patchID = p_neigh;
									flow_entry->adj_ptr->perimeter = 0.0;
									flow_entry->adj_ptr->next = NULL;
								}
							}
						} // end new entry
					} //end of creating a list of neighbours

					// add perimeter length
					//if (abs((c+r)/ 2)*1.0/1.0== 0.5 )
					if (abs(c) + abs(r) == 1)
						flow_entry->adj_ptr->perimeter += 1.;
					else
						flow_entry->adj_ptr->perimeter += 1.0 / sqrt(2.0);
				} // end is neigh
			} // end edges if
		} // end col 
	} // end row 

	flow_entry->adj_ptr = flow_entry->adj_list;

	return new_adj;
}


struct adj_struct  *check_list(int patchID, int max, adj_struct *list)
{
	int i, fnd;
	fnd = 0;
	i = 0;

	while ((fnd == 0) && (i < max)) {
		if ((list->patchID == patchID))
			fnd = 1;
		else {
			if (list->next == NULL)
				i = max;
			else {
				i += 1;
				list = list->next;
			}
		}
	}
	return(list);
}



