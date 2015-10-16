#pragma once
#include <fstream>

using namespace std;
//pomocnicze
std::string get_file_contents(const char *filename);
size_t GetSizeOfFileUTF8( const std::wstring& path );
std::wstring LoadFileToUTF8String(const std::wstring& filename);