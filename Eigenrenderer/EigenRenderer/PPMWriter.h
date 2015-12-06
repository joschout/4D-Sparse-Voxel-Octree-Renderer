#pragma once
class PPMWriter
{
public:
	char * fileName;
	int imageWidth;
	int imageHeight;

	PPMWriter();
	~PPMWriter();

	void writefile(unsigned char* myDataBuffer, const int rgba_amount) const;
	static void writePPMFile();
	static void writePPMFile2();
};

