#include "auxiliary.hpp"

// *********************************
//
std::string get_file_contents( const char *filename )
{
    std::ifstream in( filename, std::ios::in | std::ios::binary );

    if( in )
    {
        std::string contents;

        in.seekg( 0, std::ios::end );
        contents.resize( (unsigned int) in.tellg() );
        in.seekg( 0, std::ios::beg );
        in.read( &contents[0], contents.size() );
        in.close();

        return(contents);
    }

    throw( errno );

}


size_t GetSizeOfFileUTF8( const std::wstring& path )
{
	struct _stat fileinfo;
	_wstat(path.c_str(), &fileinfo);
	return fileinfo.st_size;
}

std::wstring LoadFileToUTF8String(const std::wstring& filename)
{
	std::wstring buffer;            // stores file contents
	FILE* f = nullptr;
    _wfopen_s(&f, filename.c_str(), L"rtS, ccs=UTF-8");

	// Failed to open file
	if (f == NULL)
	{
		// ...handle some error...	
		return buffer;
	}

	size_t filesize = GetSizeOfFileUTF8(filename);

	// Read entire file contents in to memory
	if (filesize > 0)
	{
		buffer.resize(filesize);
		size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
		buffer.resize(wchars_read);
		buffer.shrink_to_fit();
	}

	fclose(f);

	return buffer;
}
