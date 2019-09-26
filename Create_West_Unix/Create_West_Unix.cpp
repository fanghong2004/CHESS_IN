//This program uses DEM ASCII image to create west-horizon file
//commandline/unix version, created by Guoping Tang
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

using namespace std;

double max(double x[], int n);

int main(int argc, char const *argv[])
{
	ifstream demFile;
	ofstream westFile;

	char str[50];
	int rows, cols;
	double *pDem, *pTan, *pSin, *pHorizon;
	double nodata, xllcorner, yllcorner, cellsize;

	//open and check if opening file is successful
	demFile.open(argv[1], ios::in);
	westFile.open(argv[2], ios::out);
	if (!demFile)
	{
		cerr << "fail to open input file!" << endl;
		return 1;
	}

	if (!westFile)
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
	westFile << setiosflags(ios::left) << setw(14) << "ncols" << setw(12) << cols << endl;
	westFile << setiosflags(ios::left) << setw(14) << "nrows" << setw(12) << rows << endl;
	westFile << setiosflags(ios::left) << setw(14) << "xllcorner" << setw(14) << fixed << setprecision(6) << xllcorner << endl;
	westFile << setiosflags(ios::left) << setw(14) << "yllcorner" << setw(14) << fixed << setprecision(6) << yllcorner << endl;
	westFile << setiosflags(ios::left) << setw(14) << "cellsize" << setw(12) << cellsize << endl;
	westFile << setiosflags(ios::left) << setw(14) << "NODATA_value" << setw(12) << nodata << endl;

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
			for (int m = 0; m< cols; m++)
			{
				pSin[m] = 0;
				pTan[m] = 0;
			}

			for (int j = 0; j < i; j++)
			{
				if (((i - j) * cellsize > 5000))
				{
					pSin[j] = 0;
				}
				pTan[j] = (pDem[j] - pDem[i]) / ((i - j) * cellsize);
				if (pTan[j] > 0)
					pSin[j] = 1 / (sqrt(1 + (1 / (pTan[j] * pTan[j]))));
				else
					pSin[j] = (-1) / (sqrt(1 + (1 / (pTan[j] * pTan[j]))));
			}

			pHorizon[i] = (double)((int)(max(pSin, cols) * 100 + 0.5)) / 100;
			westFile << left << fixed << setw(9) << setprecision(2) << pHorizon[i];
		}
		westFile << endl;
	}

	//release memory
	delete[] pDem;
	delete[] pTan;
	delete[] pSin;
	delete[] pHorizon;

	printf("The number of your rasters is %d\n", cols * rows);

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