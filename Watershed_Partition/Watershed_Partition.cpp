// Watershed_Partition.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include "pch.h"
#include "Flow.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main()
{
	ifstream FlowTableFile;
	fstream out1;
	ofstream out2;

	patch_object patch = {};
	patch_object *ppatch = {&patch };
	patch_object *neigh = { &patch };
	long int num_patches = 0;
	int partition_numbers = 20;

	//========================================================================================================================================
	//opening flowtable 
	FlowTableFile.open("E:/clevenew/flowtable/cleve_flow_table_d8.dat",ios_base::in );
	if (!FlowTableFile)	{
		cerr << "fail to open flow table file!" << endl;
		return 1;
	}
	
	out1.open("E:/clevenew/flowtable/cleve_flow_table_partition.dat", ios::in | ios::out);
	if (!out1) {
		cerr << "fail to open partition output file!" << endl;
		return 1;
	}

	out2.open("E:/clevenew/flowtable/cleve_flow_table_partition_final.dat", ios::out);
	if (!FlowTableFile) {
		cerr << "fail to open partition final file!" << endl;
		return 1;
	}

	//========================================================================================================================================
	//First, read the flow table file to assign the patch.ID for each patch (necessary)
	FlowTableFile >> num_patches;
	cout << "The number of patches are " << num_patches << endl;

	ppatch = new patch_object[num_patches]; //dynamically allocate variable
	neigh = new patch_object[num_patches];  //dynmaically allocate variable

	int i = 0;
	while (!FlowTableFile.eof()) {
		FlowTableFile >> ppatch[i].ID >> ppatch[i].x >> ppatch[i].y >> ppatch[i].z >> ppatch[i].acc_area
			>> ppatch[i].type >> ppatch[i].gamma >> ppatch[i].num_neighbours;

		read(ppatch[i].num_neighbours, &ppatch[i], FlowTableFile);
		i++;
	}

	//========================================================================================================================================
	FlowTableFile.clear(); //this is needed for reset the pointer to the beggning of file (the following sentence)
	FlowTableFile.seekg(0, ios::beg);
	FlowTableFile >> num_patches;

	for(int i=0;i<num_patches;i++) {
		FlowTableFile >> ppatch[i].ID >> ppatch[i].x >> ppatch[i].y >> ppatch[i].z >> ppatch[i].acc_area
			>> ppatch[i].type >> ppatch[i].gamma >> ppatch[i].num_neighbours;

		ppatch[i].neighbours = new neighbour_object[ppatch[i].num_neighbours];
		ppatch[i].num_neighbours = assign_neighbours(ppatch[i].neighbours, ppatch[i].num_neighbours, &ppatch[i], FlowTableFile, num_patches);
	}
	
	//========================================================================================================================================
	//partition the watershed into a user specified number of sub-watersheds
	int kk = 0,jj=1;
	double stand = static_cast<double>(num_patches) /static_cast<double>(partition_numbers);
	cout << "here is ok" << endl;

	bool find = false;
	mark(ppatch, num_patches); //mark all patches 11

	do {
		kk = 1;
		for (int i = 0; i < num_patches; i++) {
			if (ppatch[i].acc_area >= stand && ppatch[i].type == 1 && ppatch[i].mark == 11) {

				ppatch[i].mark = 0;
				kk = 0;
				
				//cout<<ppatch[i].neighbours[0].patch->ID << endl;
				//getchar();
				find = true;
				if (find) {
					out1 << fixed << right << setw(10) << ppatch[i].ID << setw(12) << setprecision(3) << ppatch[i].x << setw(12)
						<< setprecision(3) << ppatch[i].y << setw(12) << setprecision(3) << ppatch[i].z << setw(12)
						<< setprecision(3) << ppatch[i].acc_area << setw(3) << ppatch[i].type << setw(12) 
						<< setprecision(5) << ppatch[i].gamma << setw(3) << ppatch[i].num_neighbours << endl;
					
					double seeding_area = ppatch[i].acc_area; //set seed area
					int jj=find_next(&ppatch[i], ppatch[i].num_neighbours, seeding_area, stand);
					
					if (jj != -999) {
						for (int j = i; j < num_patches; j++) {
							if (ppatch[j].ID == jj)
								out1 << fixed << right << setw(10) << ppatch[j].ID << setw(12) << setprecision(3) 
								<< ppatch[j].x << setw(12) << setprecision(3) << ppatch[j].y
								<< setw(12) << setprecision(3) << ppatch[j].z << setw(12) << setprecision(3) 
								<< ppatch[j].acc_area << setw(3) << ppatch[j].type << setw(12) << setprecision(5) 
								<< ppatch[j].gamma << setw(3) << ppatch[j].num_neighbours << endl;
							seeding_area = ppatch[j].acc_area;
						}
					}
					else if(jj==-999){
						int j = num_patches - 1;
						find = false;
					}
				}
			}
		}
	} while (kk == 0);

	//always output the mounth of the watershed
	int j=num_patches-1;
	out1 << fixed << right << setw(10) << ppatch[j].ID << setw(12) << setprecision(3) << ppatch[j].x << setw(12) << setprecision(3) 
		<< ppatch[j].y << setw(12) << setprecision(3) << ppatch[j].z << setw(12) << setprecision(3) << ppatch[j].acc_area
		<< setw(3) << ppatch[j].type << setw(12) << setprecision(5) << ppatch[j].gamma << setw(3) << ppatch[j].num_neighbours;

	
	//=====================================================================================================================================
	//out.close();
	//out1.open("E:/clevenew/flowtable/cleve_flow_table_partition.dat", ios::in);
	//read the mouthes of partitioned sub-watersheds
	out1.clear(); //this is needed for reset the pointer to the beggning of file (the following sentence)
	out1.seekg(0, ios::beg);

	int code, type, num_neighbors;
	double xx, yy, area, elev,gamma;
	for (int i = 0; i < num_patches; i++) {
		ppatch[i].mark =-1;
		ppatch[i].order = -999;
		ppatch[i].partitionID = -999;
	}

	int mark = 0;
	while (!out1.eof()) {
		out1 >> code >> xx >> yy >> elev >> area >> type >> gamma >> num_neighbors;
		for (int i = 0; i < num_patches; i++) {
			if (code == ppatch[i].ID) {
				ppatch[i].order = 0;
				mark++;
				ppatch[i].partitionID = mark;
				ppatch[i].mark = -1;
				
				//cout << "id is " <<code<<" "<< ppatch[i].ID << " " << mark << endl;
				break;
			}
		}

		for (int i = num_patches - 1; i >= 0; i--) {
			if (ppatch[i].order != -999. && ppatch[i].mark == -1) 
				code = ppatch[i].ID;

			for (int j = i; j >= 0; j--) {
				/*
					if (code == 4847) {
						cout<<" 1 " <<i<<" "<<ppatch[i].ID<<" "<<j<<" "<< ppatch[j].ID << endl;
						cout<<" 2 "<< ppatch[j].partitionID <<" "<<ppatch[j].order<< endl;
						//getchar();
					}
				*/
					if(i!=j && ppatch[j].partitionID==-999) //elevations could be same and so should not be used here
						find_patches(&ppatch[j], code, mark);
					/*
					if (code == 4847) {
						cout <<" 4 "<< ppatch[i].ID << " " << ppatch[j].ID << " "<<ppatch[j].partitionID<<endl;
						getchar();
					}
					*/
				}
		}
		//break;
		
		for (int j = 0; j < num_patches; j++) {
			if (ppatch[j].partitionID ==mark) {
				out2 << fixed << right << setw(10) << ppatch[j].ID << setw(12) << setprecision(3) << ppatch[j].x << setw(12)
					<< setprecision(3) << ppatch[j].y
					<< setw(12) << setprecision(3) << ppatch[j].z << setw(12) << ppatch[j].partitionID << endl;
			}
		}
	}

	//========================================================================================================================================
	//writtig the output data
	/*
	for (int j = 0; j < num_patches; j++) {
		if (ppatch[j].partitionID >0) {
			out2 << fixed << right << setw(10) << ppatch[j].ID << setw(12) << setprecision(3) << ppatch[j].x << setw(12)
				<< setprecision(3) << ppatch[j].y
				<< setw(12) << setprecision(3) << ppatch[j].z << setw(12) << ppatch[j].partitionID << endl;
		}
	}
	*/
	delete[] ppatch;
	delete[] neigh;

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
