#include <fstream>
#ifndef PPMWRITER_H_GUARD
#define PPMWRITER_H_GUARD
#include "PPMWriter.h"
#include <stdio.h>
#include <iostream>


PPMWriter::PPMWriter(): fileName("outputImage.ppm"), imageWidth(640), imageHeight(640)
{
}


PPMWriter::~PPMWriter()
{
}

void PPMWriter::writefile(unsigned char* myDataBuffer, const int rgba_amount) const
{
	FILE * pFile;

	errno_t errorCode = fopen_s(&pFile, fileName, "w");
	if (errorCode == 0)
	{

		for (int i = 0; i < rgba_amount; i + 3) {
			fprintf(pFile, "%d %d %d ", myDataBuffer[i], myDataBuffer[i + 1], myDataBuffer[i + 2]);
		}
		fflush(pFile);
		fclose(pFile);
	}
	else {
		std::cout << "Opening your file went wrong inside your main" << std::endl;
	}
}

void PPMWriter::writePPMFile()
{
	int width = 1280;
	int height = 640;
	int R = 255;
	int G = 255;
	int B = 0;

	FILE * pFile;

	errno_t errorCode = fopen_s(&pFile, "myImage.ppm", "w");
	if (errorCode == 0)
	{
		fprintf(pFile, "P3\n%d %d\n%d\n", width, height, 255);
		for (int i = 0; i < width*height; i++) {
			fprintf(pFile, "%d %d %d ", R, G, B);
		}
		fflush(pFile);
		fclose(pFile);
	}
	else {
		std::cout << "Opening your file went wrong inside your main" << std::endl;
	}

}

void PPMWriter::writePPMFile2()
{
	int width = 1280;
	int height = 640;
	int R = 255;
	int G = 255;
	int B = 0;

	std::ofstream myfile;
	myfile.open("test.ppm");
	if (myfile.is_open())
	{
		/* ok, proceed with output */
		myfile <<  "P3\n"
			<< width <<  " " << height << "\n"
			<< 255 << std::endl;
		for (int i = 0; i < width*height; i++) {
			myfile << R << " "<< G << " " << B << " ";
			
		}
		myfile.close();
	}
	else {
		std::cout << "Opening your file went wrong inside your main" << std::endl;
	}

}
#endif /*PPMWRITER_H_GUARD*/

