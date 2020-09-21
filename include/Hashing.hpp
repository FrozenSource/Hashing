#pragma once
#include <Core.hpp>
#include <blake2.h>

bool HashFile(const string & sFileName, string & sHashOut);