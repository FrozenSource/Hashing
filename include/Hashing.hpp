#pragma once
#include <string>
#include <blake2.h>

bool HashFile(const std::string & sFileName, std::string & sHashOut);
bool Hash(const std::string& sBuffer, std::string & sHashOut);