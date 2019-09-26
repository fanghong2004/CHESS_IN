//--------------------------------------------------------------------------------------------
//  NAME:: print_drain_stats										
//                                                              
//  DESCRIPTION                                                 
//		- prints drainage info for each paths					
//--------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <iomanip>
#include "blender.h"

using namespace std;

void	print_drain_stats(int num_patches,struct flow_struct *flow_table, char *input_prefix)
{
	int i;
	ofstream outfile;
	char name[256];
	
	std::cout << "\n Printing Drainage Statistics " << std::endl;

	strcpy(name,input_prefix);
	strcat(name,"_stats.dat");
	outfile.open(name, ios::out);
	if (!outfile){
		std::cout << "Error opening flow_table output file\n" << std::endl;
		exit(1);
	}

	outfile << setw(8) << num_patches << endl;
	for (i=1; i<= num_patches; i++) {
    
		outfile << setw(6) << flow_table[i].patchID
			<< flow_table[i].x
			<< flow_table[i].y
			<< flow_table[i].z
			<< flow_table[i].patchID
			<< flow_table[i].acc_area
			<< flow_table[i].land
			<< flow_table[i].total_gamma
			<< flow_table[i].slope
			<< flow_table[i].area
			<< flow_table[i].num_adjacent << endl;
	}	
	
	//fprintf(outfile,"\n %6d %6d %6d %6.1f %6.1f %6.1f %10f %4d %15.8f %10.6f %9d %4d",  

	outfile.close();

	return;
}



