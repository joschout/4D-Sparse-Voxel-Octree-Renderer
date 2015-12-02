#pragma once
class PPMWriter
{
public:
	char * fileName;
	int imageWidth;
	int imageHeight;

	PPMWriter();
	~PPMWriter();

	void writefile(unsigned char* myDataBuffer) const;
	static void writePPMFile();
};

