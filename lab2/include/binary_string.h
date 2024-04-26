#pragma once

#include <string>

const int BYTE_SIZE = 5;

int GetBitSize(const std::string& str);

int BitDifference(const std::string& a, const std::string& b);

bool GetBitByIndex(const std::string& str, int index);
