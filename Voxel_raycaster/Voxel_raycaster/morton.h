#ifndef FILLHIERARCHYBUILDER_H_
#define FILLHIERARCHYBUILDER_H_

#include <iostream>
#include <fstream>
#include "math.h"
#include <windows.h>
#include <cstdint>
#include <limits.h>
#include <vector>

uint64_t mortonEncode(uint64_t x, uint64_t y, uint64_t z);
std::vector<uint64_t> mortonDecode(uint64_t const morton);

bool readMoctreeFile(std::string const filename, size_t& _gridlength, bool** _morton_array);
void writeMoctreeFile(std::string const filename, size_t gridlength, bool const *morton_array);

#endif
