//==============================================================================================
//  Program name: create_flowpaths								
//  Options                                                     
//                                                              
//		-v 	Verbose Option										
//		-s  print drainage statistics							
//		-sc	stream connectivity is assumed						
//			1	random slope value								
//			2	internal slope value							
//			0	no connectivity (default)						
//		-sd scale dem values by this amount
//		-st	scale streamside transmissivity						
//		-pre	input image file name prefix					
//		-a arcview ascii data files (default is GRASS ascii)	
//                                                              
//  DESCRIPTION                                                 
//=============================================================================================
#include <iostream>
#include <fstream>
#include "blender.h"
#include "file_io.h"

using namespace std;

int main(int argc, char *argv[])
{	
	// local variable declarations
	ofstream   out1, out2;
	int		   i, maxr, maxc, num_patches;
	double     xllcorner, yllcorner, cell;
	int		   vflag, s_flag,  sc_flag, arc_flag;
	int		   st_flag,prefix_flag;
	char       *input_prefix = { nullptr };

	// filenames for each image and file
	char    fndem[MAXS], fntable[MAXS], fnpatch[MAXS], fnslope[MAXS];
	char	fnstream[MAXS], name1[MAXS] = { NULL }, name2[MAXS] = { NULL };

	// set pointers for images
	double    *dem = { nullptr }, *slope = { nullptr };
	int       *patch = { nullptr }, *stream = { nullptr };
	struct	  flow_struct	*flow_table = { nullptr };

	vflag = 0;		 // verbose flag					 
	s_flag = 1;		 // printing stats flag			 
	sc_flag = 1;	 // stream connectivity flag		 
	st_flag = 0;	 // scaling stream side patches 	 
	arc_flag = 1;	 // arcview input data flag		 
	prefix_flag = 0; // input prefix flag  

	i = 1;
	while (i < argc) {

		if (strcmp(argv[i], "-sc") == 0){
			i += 1;
			if (i == argc)  {
				cerr << "\n FATAL ERROR: Output prefix not specified \n" << endl;
				exit(0);
			}
			sc_flag = (int)atoi(argv[i]);
		}

		if (strcmp(argv[i], "-s") == 0){
			s_flag = 1;
		}

		if (strcmp(argv[i], "-pre") == 0){
			i += 1;
			if (i == argc)  {
				cerr << "\n FATAL ERROR: Output prefix not specified\n" << endl;
				exit(0);
			}

			//-------------------------------------------------------------------------//
			// Allocate an array for the output prefix and read in the output prefix   //
			//-------------------------------------------------------------------------//
			if ((input_prefix = (char *)malloc((1 + strlen(argv[i]))*sizeof(char))) == NULL){
				cerr << "\n FATAL ERROR: Cannot allocat output_prefix\n" << endl;
				exit(0);
			}
			strcpy(input_prefix, argv[i]);
			prefix_flag = 1;
		}

		if (strcmp(argv[i], "-a") == 0){
			arc_flag = 1;
		}
		i += 1;
	} //end of while


	if (prefix_flag == 0) {
		cerr << "\n FATAL ERROR: You must specify a prefix for image files \n" << endl;
		return 1;
	}

	//Now starting creating flow table
	cout << "Starting creating flow paths!" << endl;

	//Assigning input image names
	input_prompt(&maxr, &maxc, &xllcorner, &yllcorner, &cell,input_prefix, fndem, fnslope, fnpatch, fnstream, fntable, arc_flag);

	// open some diagnostic output files
	strcpy_s(name1, input_prefix);
	strcat_s(name1, ".build");
	out1.open(name1, ios::out);
	if (!out1){
		cerr << "cannot open build file \n" << endl;
		return 2;
	}

	strcpy(name2, input_prefix);
	strcat(name2, ".pit");
	out2.open(name2, ios::out);
	if (!out2){
		cerr << "cannot open pit file \n" << endl;
		return 3;
	}

	// Dynamically allocate memory for input map images
	patch = new int[maxr*maxc];
	input_ascii_int(patch, fnpatch, maxr, maxc, arc_flag);	

	dem = new double[maxr*maxc];
	input_ascii_double(dem, fndem, maxr, maxc, arc_flag);

	if (sc_flag == 1) {
		slope = new double[maxr*maxc];
		input_ascii_double(slope, fnslope, maxr, maxc, arc_flag);
	}
	
	stream = new int[maxr*maxc];
	input_ascii_int(stream, fnstream, maxr, maxc, arc_flag);

	// allocate flow table
	flow_table = new flow_struct[maxr*maxc];

	num_patches = build_flow_table(flow_table, dem, slope, patch, stream, maxr,
		maxc,xllcorner,yllcorner, sc_flag, cell); 
	out1.close();

	// processes patches - computing means and neighbour slopes and gammas
	compute_gamma(flow_table, num_patches, cell, sc_flag);

	// remove pits and re-order patches appropriately
	remove_pits(flow_table, num_patches, sc_flag, cell, out2);

	if (s_flag == 1) {
		compute_upslope_area(flow_table, num_patches, cell);
		//print_drain_stats(num_patches, flow_table);
		//compute_dist_from_road(flow_table, num_patches, out2, cell);	
		//compute_drainage_density(flow_table, num_patches, cell);
	}

	print_flow_table(num_patches, flow_table, sc_flag, cell, input_prefix);
	
	//close output files
	out2.close( );

	//release dynamically allocatable array
	delete[] patch;	
	delete[] dem;
	delete[] slope;
	delete[] stream;
	delete[] flow_table;

	cout << "Finishing creating flow path!" << endl;

	return 0;
}


