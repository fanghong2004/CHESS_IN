//This program uses watershed ASCII image to create patch file
//commandline/unix version, created by Guoping Tang
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

double min(double x[], int n);
double max(double x[], int n);

int main(int argc, char *argv[]){
	ifstream inFile;
	ofstream outFile;

	char   str[50];
	int    rows, cols, tot_pits = 0, neighbors = 0;
	double nodata, xllcorner, yllcorner, cellsize, newvalue;
	double **pDem;

	//Input watershed ascii files and create patch file for the watershed
	inFile.open(argv[1], ios::in);
	outFile.open(argv[2], ios::out);
	if (!inFile)
	{
		cerr << "fail to open input file!" << endl;
		return 1;
	}

	if (!outFile)
	{
		cerr << "fail to open output file!" << endl;
		return 2;
	}

	//read ArcINFO ascii DEM data file
	inFile.seekg(0, ios::beg);
	inFile >> str >> cols;
	inFile >> str >> rows;
	inFile >> str >> xllcorner;
	inFile >> str >> yllcorner;
	inFile >> str >> cellsize;
	inFile >> str >> nodata;

	//dynamically allocate two-dimensional array and read dem data;
	pDem = new double*[cols]; //开辟行  
	for (int i = 0; i <cols; i++)
	{
		pDem[i] = new double[rows];//开辟列  
	}

	for (int j = 0; j<rows; j++){
		for (int i = 0; i<cols; i++){
			inFile >> newvalue;
			pDem[i][j] = newvalue;
		}
	}

	//assign patch numbers/codes
	int m = 0;
	for (int j = 0; j<rows; j++){
		for (int i = 0; i<cols; i++){
			if (pDem[i][j] != nodata){
				m++;
				pDem[i][j] = m;
			}
		}
	}

	//output files
	outFile << setiosflags(ios::left) << setw(14) << "ncols" << setw(12) << cols << endl;
	outFile << setiosflags(ios::left) << setw(14) << "nrows" << setw(12) << rows << endl;
	outFile << setiosflags(ios::left) << setw(14) << "xllcorner" << setw(14) << fixed << setprecision(6) << xllcorner << endl;
	outFile << setiosflags(ios::left) << setw(14) << "yllcorner" << setw(14) << fixed << setprecision(6) << yllcorner << endl;
	outFile << setiosflags(ios::left) << setw(14) << "cellsize" << setw(12) << cellsize << endl;
	outFile << setiosflags(ios::left) << setw(14) << "NODATA_value" << setw(12) << nodata << endl;

	for (int j = 0; j<rows; j++){
		for (int i = 0; i<cols; i++){
			outFile << right << setw(8) << setprecision(0) << pDem[i][j];
		}
		outFile << endl;
	}

	//释放开辟的二维空间  
	for (int i = 0; i < cols; i++)
		delete[] pDem[i];
	delete[] pDem;

	cout << "The number of patchs are \n" << m << endl;

	return 0;
}

double max(double x[], int n)
{
	double y = 0;

	for (int i = 0; i < n; i++)
	{
		if (x[i] > y)
		{
			y = x[i];
		}
	}
	return y;
}

double min(double x[], int n)
{
	double y = 1000000000.;

	for (int i = 0; i < n; i++)
	{
		if (x[i] < y)
		{
			y = x[i];
		}
	}
	return y;
}