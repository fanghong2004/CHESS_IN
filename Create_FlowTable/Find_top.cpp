//---------------------------------------------------------------------------------------
//  NAME:: find_top												
//																
//	Descripition:: recursive routing to find the top of a pit and to create a 
//  list of all patches in a pit. The top of a pit is the patch
//  with the minimum elevation which points to somewhere outside the pit														
//---------------------------------------------------------------------------------------
#include "blender.h"

double	find_top(
struct flow_struct *flow_table,
	int curr,
	double pit_elev,
	int *num_pit, int *upslope_list,
	int *edge_inx)
{
	int in_list(int, int *, int);
	struct	adj_struct *aptr;
	int 	i, inx, nnew;
	double	top_elev, min_elev;
	int	next_edge_inx;

	min_elev = 0.0;
	top_elev = 0.0;

	aptr = flow_table[curr].adj_list;
	// check to see if we are at the edge of the pit, find min elevation of
	// apixel draining from the pit 										

	if ((*edge_inx == 0.0) || (flow_table[curr].z < flow_table[*edge_inx].z)) {
		for (i = 1; i <= flow_table[curr].num_adjacent; i++){
			inx = aptr->inx;

			nnew = in_list(inx, upslope_list, *num_pit);
			if ((aptr->gamma > 0) && (nnew == 0)) {
				top_elev = flow_table[curr].z;
				if (((top_elev < min_elev) || (min_elev == 0.0)) && (top_elev > 0.0) && (aptr->z < pit_elev)){
					*edge_inx = aptr->inx;
					min_elev = top_elev;
				}
			}

			else if ((aptr->gamma == 0) && (nnew == 0)) {
				*num_pit += 1;
				upslope_list[*num_pit] = inx;
				next_edge_inx = *edge_inx;
				top_elev = find_top(flow_table, inx, pit_elev, num_pit, upslope_list, &(next_edge_inx));
				if (((top_elev < min_elev) || (min_elev == 0.0)) && (top_elev > 0.0)) {
					*edge_inx = next_edge_inx;
					min_elev = top_elev;
				}
			}
			aptr = aptr->next;
		} // end first pass
	} //

	return (min_elev);
}

