#pragma once
#include <Core.hpp>
#include <blake2.h>

bool HashFile(const string & sFileName, string & sHashOut);
bool Hash(const string& sBuffer, string & sHashOut);