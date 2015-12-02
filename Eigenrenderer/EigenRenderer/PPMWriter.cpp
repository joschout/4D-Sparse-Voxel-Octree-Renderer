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

void PPMWriter::writefile(unsigned char* myDataBuffer) const
{
	FILE * pFile;

	errno_t errorCode = fopen_s(&pFile, fileName, "w");
	if (errorCode == 0)
	{
		int R = 0;
		int G = 0;
		int B = 255;
		fprintf(pFile, "P3\n%d %d\n%d\n", imageWidth, imageHeight, 255);
		for (int i = 0; i < 640*640; i++) {
			fprintf(pFile, "%d %d %d ", R, G, B);
		}
		/*for (int i = 0; i < imageWidth*imageHeight*3; i + 3) {
			fprintf(pFile, "%d %d %d ", myDataBuffer[i], myDataBuffer[i + 1], myDataBuffer[i + 2]);
		}*/
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
#endif /*PPMWRITER_H_GUARD*/

