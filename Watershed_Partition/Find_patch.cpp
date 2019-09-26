//=========================================================================================================================================
//		find_patch				
//  OPTIONS                                                   
//                                                            
//  DESCRIPTION                                               
//                                                            
//  PROGRAMMER NOTES                                          
//=========================================================================================================================================
#include "pch.h"
#include <stdio.h>
#include "flow.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct patch_object *find_patch(int patch_ID, struct patch_object *patch, int num_patches)
{
	//========================================================================================================================================
	//	Local Variable Definition. 							
	//========================================================================================================================================
	int i;
	int fnd;
	struct patch_object *temp_patch = { nullptr };

	//========================================================================================================================================
	//	find patches						
	//========================================================================================================================================
	i = 0;
	fnd = 0;
	while ((fnd == 0) && (i >= 0) && (i < num_patches)) {
		if (patch[i].ID == patch_ID) {
			temp_patch = &patch[i];
			fnd = 1;
			break;
		}
		else {
			i += 1;
		}
	}
	if (fnd == 0) {
		cerr<<"FATAL ERROR: Could not find patch %d\n"<<patch_ID<<endl;
		//return ;
	}
	return(temp_patch);
}//end find.patch.cpp


//=========================================================================================================================================
void recursive_mark(struct patch_object *patch, int neighbors) {
	struct patch_object *neigh;

	if (neighbors >= 1)
		for (int i = 0; i < neighbors; i++) {
			neigh = patch->neighbours[i].patch;
			neigh->mark = 0;
			recursive_mark(neigh, neigh->num_neighbours);
			//cout <<"neigh->ID is "<< neigh->ID << endl;
		}
	else
		return;
};

//===============================================================================================================================
void mark(struct patch_object *patch, int num_patches) {
	for (int i = 0; i < num_patches; i++) {
		patch[i].mark = 11;
	}
	return;
};


//===============================================================================================================================
void read(int num_neighbours, struct patch_object *patch, std::ifstream& FlowTableFile) {
	int ID;
	double GAMMA;
	for (int i = 0; i < num_neighbours; i++) {
		FlowTableFile >> ID >> GAMMA;
	}
	return;
}

//===============================================================================================================================
int find_next(struct patch_object *patch, int neighbors, double acc_area, double stand) {
	struct patch_object *neigh;

	if (neighbors >= 1) {
		for (int i = 0; i < neighbors; i++) {
			neigh = patch->neighbours[i].patch;
			if (neigh->acc_area >= acc_area + stand)
				return neigh->ID;
			else {
				neigh->mark = 0;
				find_next(neigh, neigh->num_neighbours, acc_area, stand);
			}
		}
	}
	return -999;
}

//===============================================================================================================================
void find_patches(struct patch_object *patch, int& code, int mark) {
	struct patch_object *neigh;

	//if (code == 4847) {
	//	cout << "patch order is " << patch->order << " " << patch->order << endl;
	//}
	//cout <<"id is "<< patch->ID <<" "<< patch->num_neighbours << endl;
	for (int i = 0; i < patch->num_neighbours; i++) {
		neigh = patch->neighbours[i].patch;
		if (neigh->ID == code) {
			if (patch->order == -999) {
				neigh->mark = 1;
				patch->partitionID = mark;

				patch->order = neigh->order + 1;
			}

			//find_patches(patch, code, mark);
			//cout << patch->mark << endl;
		}
	}
	return;
};

