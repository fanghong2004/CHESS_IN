//This program uses DEM ASCII image to create East-horizon file
//windows version, created by Guoping Tang
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <CString>
#include <cstdio>
#include <string>
#include <algorithm>//算法头文件 
using namespace std;

double max(double x[], int n);

int main(int argc, char const *argv[])
{
	ifstream demFile;
	ofstream eastFile;

	char str[50];
	int rows, cols;
	double *pDem, *pTan, *pSin, *pHorizon;
	double nodata, xllcorner, yllcorner, cellsize;

	//open and check if opening file is successful
	//demFile.open("D://gis_watershed//flowtable//xf_ws.dem", ios::in);
	//eastFile.open("D://gis_watershed//flowtable//xf_ws.east", ios::out);
	ifstream ifs;
	ifs.open("c:\\file4.txt", ios::in);
	
	/*char buf[1024];
	while (ifs.getline(buf, sizeof(buf)))
	{
		cout << buf << endl;
	}*/
	
	string buf_in;
	string buf_out;
	getline(ifs, buf_in);
	while (getline(ifs,buf_out))
	{
		cout << buf_out << endl;
	}
	
	
	//string buf1 = "C://out//xf.dem";
	//string buf1 = buf;
	//cout << buf1 << endl;
	//string buf1 = buf;
	demFile.open(buf_in, ios::in);
	//demFile.open("D://muzhen//guhui//250//flowtable//lxh_250.dem", ios::in);
	eastFile.open(buf_out, ios::out);
	//eastFile.open("C://out//xf.east", ios::out);
	//eastFile.open("D://muzhen//guhui//250//flowtable//lxh_250.east", ios::out);
	if (!demFile)
	{
		cerr << "fail to open input file!" << endl;
		return 1;
	}

	if (!eastFile)
	{
		cerr << "fail to open output file!" << endl;
		return 2;
	}

	//read ArcINFO ascii DEM data file
	demFile.seekg(0, ios::beg);
	demFile >> str >> cols;
	demFile >> str >> rows;
	demFile >> str >> xllcorner;
	demFile >> str >> yllcorner;
	demFile >> str >> cellsize;
	demFile >> str >> nodata;

	//write the header file for east horizon
	eastFile << setiosflags(ios::left) << setw(14) << "ncols" << setw(12) << cols << endl;
	eastFile << setiosflags(ios::left) << setw(14) << "nrows" << setw(12) << rows << endl;
	eastFile << setiosflags(ios::left) << setw(14) << "xllcorner" << setw(14) << fixed << setprecision(6) << xllcorner << endl;
	eastFile << setiosflags(ios::left) << setw(14) << "yllcorner" << setw(14) << fixed << setprecision(6) << yllcorner << endl;
	eastFile << setiosflags(ios::left) << setw(14) << "cellsize" << setw(12) << cellsize << endl;
	eastFile << setiosflags(ios::left) << setw(14) << "NODATA_value" << setw(12) << nodata << endl;


	//dynamically allocate space
	pDem = new double[cols];
	pTan = new double[cols];
	pSin = new double[cols];
	pHorizon = new double[cols];

	for (int n = 0; n < rows; n++)
	{
		for (int m = 0; m < cols; m++)
			demFile >> pDem[m];

		for (int i = 0; i < cols; i++)
		{
			for (int l = 0; l < cols; l++)
			{
				pSin[l] = 0;
				pTan[l] = 0;
			}

			for (int j = i + 1; j < cols && j > i && ((j - i) * cellsize <= 5000); j++)
			{
				pTan[j] = (pDem[j] - pDem[i]) / ((j - i) * cellsize);
				if (pTan[j] > 0)
					pSin[j] = 1 / (sqrt(1 + (1 / (pTan[j] * pTan[j]))));
				else
					pSin[j] = (-1) / (sqrt(1 + (1 / (pTan[j] * pTan[j]))));
			}

			pHorizon[i] = (double)((int)(max(pSin, cols) * 100 + 0.5)) / 100;
			eastFile << left << fixed << setw(9) << setprecision(2) << pHorizon[i];
		}
		eastFile << endl;
	}

	printf("The number of your rasters is %d\n", cols * rows);

	//release memory
	delete[] pDem;
	delete[] pTan;
	delete[] pSin;
	delete[] pHorizon;

	return 0;
}

double max(double x[], int n)
{
	double y = 0;

	for (int i = 0; i < n + 1; i++)
	{
		if (x[i] > y)
		{
			y = x[i];
		}
	}
	return y;
}