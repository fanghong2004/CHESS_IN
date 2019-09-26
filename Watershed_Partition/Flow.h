#pragma once
#include <iostream>
#include "pch.h"

int assign_neighbours(struct neighbour_object *neighbours,
	int num_neighbours,
	struct patch_object *patch,
	std::ifstream & routing_file, int num_patches);

void recursive_mark(struct patch_object *patch, int);
void mark(struct patch_object *patch, int); 

void read(int num_neighbours, struct patch_object *patch, std::ifstream& FlowTableFile);
int find_next(struct patch_object *patch, int neighbors, double acc_area, double stand);
void find_patches(struct patch_object *patch, int &, int);

//===============================================================================================================================
//	Define a neighbours object.					
//===============================================================================================================================
struct	neighbour_object
{

	double gamma;		/* m**2 / day */
	struct	patch_object *patch;
};

struct patch_object
{
	int		ID;
	double	x;									/* meters	*/
	double	y;									/* meters	*/
	double	z;									/* meters	*/
	double  acc_area;                             /*accumulated area*/
	int     type;
	double	gamma;
	int	    num_neighbours;
	int     count;
	int     mark;
	int    order;
	int  partitionID;
	struct	neighbour_object 	    *neighbours;
};