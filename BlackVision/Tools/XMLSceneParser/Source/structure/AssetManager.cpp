#include "AssetManager.h"

#include <string>
#include "rapidxml.hpp" //"rapidxml.hpp"
#include "UseLoggerXMLModule.h"
#include <boost/algorithm/string.hpp>

using namespace rapidxml;

namespace bv{
	namespace BB{
	void GetFileListing(std::list<std::wstring>& listing, std::wstring directory, std::wstring fileFilter, bool recursively = true)
	{
		// If we are going to recurse over all the subdirectories, first of all
		// get all the files that are in this directory that match the filter
		if (recursively)
			GetFileListing(listing, directory, fileFilter, false);

		directory += L"\\";

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		// Setup the filter according to whether we are getting the directories
		// or just the files
		std::wstring filter = directory + (recursively ? L"*" : fileFilter);

		// Find the first file in the directory.
		hFind = FindFirstFile(filter.c_str(), &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();
			if (dwError != ERROR_FILE_NOT_FOUND)
			{
//				std::cout << "Invalid file handle for filter " << filter << ". Error is " << GetLastError() << std::endl;
			}
		}
		else
		{
			// Add the first file found to the list
			if (!recursively)
			{
				listing.push_back(directory + std::wstring(FindFileData.cFileName));
			}

			// List all the other files in the directory.
			while (FindNextFile(hFind, &FindFileData) != 0)
			{
				if (!recursively)
				{
					listing.push_back(directory + std::wstring(FindFileData.cFileName));
				}
				else
				{
					// If we found a directory then recurse into it
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)>0 && FindFileData.cFileName[0] != '.')
					{
						GetFileListing(listing, directory + std::wstring(FindFileData.cFileName), fileFilter);
					}
				}
			}

			DWORD dwError = GetLastError();
			FindClose(hFind);
			if (dwError != ERROR_NO_MORE_FILES)
			{
				//std::cout << "FindNextFile error. Error is " << dwError << std::endl;
				
			}
		}
	}


    std::string AssetManager::CurrentProject;
    std::string AssetManager::CurrentDirectory;
    std::string AssetManager::MediaFolderPath;
	SurfaceDescriptor AssetManager::DefaultSurface;
	std::vector<SurfaceDescriptor*> AssetManager::AvailableSurfaces;
    TestVideoInput* AssetManager::VideoInput = new TestVideoInput( 1920, 1080, 1.f );
    //AssetManager::VideoInput = new TestVideoInput( 1920, 1080, 1.f );

	AssetManager::AssetManager(void)
	{
		
	}


	AssetManager::~AssetManager(void)
	{

	}

	bool AssetManager::LoadSurfaceDefinition(string file_path, string surface_path)
	{
		ifstream f(file_path.c_str());
		if (f.good()) {

			string temp = get_file_contents(file_path.c_str());

			xml_document<>          doc;    // character type defaults to char
			char *buffer = new char[temp.size() + 1];
			buffer[temp.size()] = 0;

			memcpy(buffer, temp.c_str(), temp.size());
			doc.parse<0>(buffer);

			

			xml_node<> *Surfaces = doc.first_node("surfaces");

			if (Surfaces == NULL)return false;
			xml_node<> *surface = Surfaces->first_node("surface");
			while (surface != NULL)
			{
				string isNameOk = surface->name();
				if (isNameOk != "surface")
				{
					surface = surface->next_sibling();
					continue;
				}
				xml_attribute<> *name = surface->first_attribute("name");

				SurfaceDescriptor *new_surface = new SurfaceDescriptor();
				new_surface->name = name->value();
				new_surface->path = surface_path;
				xml_node<> *color = surface->first_node("color");
				while (color != NULL)
				{
					string isNameOk = color->name();
					if (isNameOk != "color")
					{
						color = color->next_sibling();
						continue;
					}
					xml_attribute<> *name = color->first_attribute("name");
					
					ColorDescriptor *desc = &new_surface->color_main;
					
					if (std::string(name->value()) == "main")
					{
						desc = &new_surface->color_main;
					}

					xml_node<> *component = color->first_node("component");
					while (component != NULL)
					{
						string isNameOk = component->name();
						if (isNameOk != "component")
						{
							component = component->next_sibling();
							continue;
						}
						float* component_value = nullptr;
						xml_attribute<> *component_name = component->first_attribute("name");
						xml_attribute<> *component_val = component->first_attribute("value");
						string component_name_str = component_name->value();
						if (component_name_str == "r")
							component_value = &desc->R;
						if (component_name_str == "g")
							component_value = &desc->G;
						if (component_name_str == "b")
							component_value = &desc->B;
						if (component_name_str == "a")
							component_value = &desc->A;
						string val = component_val->value();
						*component_value = (float)atof(val.c_str());


						component = component->next_sibling();
					}

					color = color->next_sibling();

				}
				
				AvailableSurfaces.push_back(new_surface);

				
				surface = surface->next_sibling();
			}
			
			delete[] buffer;
			return true;
		}
		else{
			LOG_MESSAGE( SeverityLevel::error ) << "LoadSurfaceDefinition" + surface_path;
			return false;
		}
		
	}

	bool AssetManager::LoadSurfaces()
	{
		std::wstring SurfacesPath = std::wstring(MediaFolderPath.begin(),MediaFolderPath.end()) + L"surfaces/";

		std::list<std::wstring> listing;
		GetFileListing(listing, SurfacesPath, L"*.xml");
		for (std::list<std::wstring>::iterator it = listing.begin(); it != listing.end(); ++it)
		{
			
			
			wstring path = wstring(*it);
			wstring real_path = wstring(*it);
			
			boost::replace_all(path, L"\\", L"/");
			boost::replace_all(path, SurfacesPath, L"");
			boost::replace_all(path, "surfaces_definition.xml", L"");

			wprintf(L"œcie¿ka :) %s", path.c_str());
			LoadSurfaceDefinition(string(real_path.begin(),real_path.end()), string(path.begin(), path.end()));

		}

		return true;
	}


	SurfaceDescriptor* AssetManager::GetSurface(string SurfacePath, string SurfaceName)
	{
		SurfaceDescriptor descriptor = DefaultSurface;

		for (unsigned int i = 0; i < AvailableSurfaces.size(); i++)
		{
			if (AvailableSurfaces[i]->path == SurfacePath && AvailableSurfaces[i]->name == SurfaceName)
			{
				return AvailableSurfaces[i];
			}
		}
		return &DefaultSurface;

	}
    SurfaceDescriptor* AssetManager::GetSurface(string SurfaceName)
	{
		SurfaceDescriptor descriptor = DefaultSurface;

		for (unsigned int i = 0; i < AvailableSurfaces.size(); i++)
		{
			if (AvailableSurfaces[i]->path == "/"+CurrentProject+"/" && AvailableSurfaces[i]->name == SurfaceName)
			{
				return AvailableSurfaces[i];
			}
		}
		return &DefaultSurface;

	}

	string AssetManager::GetAsset(string AssetName)
	{

		return MediaFolderPath+AssetName;
	}

	string AssetManager::GetScene(string SceneName)
	{

		return MediaFolderPath+"scenes/"+CurrentProject+"/"+SceneName;
	}
    string AssetManager::GetSolution(string SolutionName)
	{

		return MediaFolderPath+"solutions/"+SolutionName;
	}
	string AssetManager::GetFont(string FontName)
	{
		return MediaFolderPath+"fonts/"+FontName;
	}
	string AssetManager::GetTexture(string TextureName)
	{

		return MediaFolderPath+"textures/"+CurrentProject+"/"+CurrentDirectory+TextureName;
	}
	string AssetManager::GetBlankTexture()
	{

		return MediaFolderPath+"textures/blank.png";
	}
	string AssetManager::GetSequence(string directory)
	{

		return MediaFolderPath+"sequences/"+CurrentProject+"/"+directory;
	}
	}
}