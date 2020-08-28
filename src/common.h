#pragma once
#include <string>

std::string GetDataURI(const std::string& data, const std::string& mime_type);
std::wstring StringToWString(std::string str);
std::string WStringToString(std::wstring wstr);