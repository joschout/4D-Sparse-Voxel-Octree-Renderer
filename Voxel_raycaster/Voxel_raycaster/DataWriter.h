#ifndef DATAWRITER_H
#define DATAWRITER_H
#include <string>
#include <fstream>

class DataWriter
{
public:

	std::string filename;
	std::fstream outputfile;

	DataWriter();
	DataWriter(std::string base_filename);
	~DataWriter();

	void writeToFile(std::string stringToWrite);
	void writeToFile_endl(std::string stringToWrite);
};


#endif
