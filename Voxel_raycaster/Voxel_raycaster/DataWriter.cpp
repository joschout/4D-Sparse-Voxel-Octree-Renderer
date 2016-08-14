#include "DataWriter.h"

DataWriter::DataWriter()
{
}

DataWriter::DataWriter(std::string base_filename)
{
	filename = base_filename + std::string(".data");

	outputfile.open(filename, std::ios::out | std::ios::in | std::ios::trunc);
}

DataWriter::~DataWriter()
{
	outputfile.close();
}

void DataWriter::writeToFile(std::string stringToWrite)
{
	outputfile << stringToWrite;
}

void DataWriter::writeToFile_endl(std::string stringToWrite)
{
	outputfile << stringToWrite << std::endl;
}
