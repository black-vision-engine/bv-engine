#pragma once
#include<string>
#include<vector>
#include "SurfaceDescriptor.h"
#include "helpers/auxiliary.hpp"
#include "win_sock.h"
#include "Engine/Models/Plugins/Simple/VideoInput/DefaultVideoInputResourceDescr.h"
#include "../../../../Applications/BlackVision/Source/hack_videoinput/TestVideoInput.h"

namespace bv{
	namespace BB{
	using namespace std;
	class AssetManager
	{
    private:
        static  std::string CurrentProject;
        static  std::string CurrentDirectory;
		static  std::string MediaFolderPath; 
		static  SurfaceDescriptor DefaultSurface;
		
		static  std::vector<SurfaceDescriptor*> AvailableSurfaces;

        

	public:
		                            AssetManager(void);
                                    ~AssetManager(void);
		static  bool				LoadSurfaces();
		static	bool				LoadSurfaceDefinition(string file_path, string surface_path);
        static	void				SetMediaFolderPath(std::string path){MediaFolderPath=path;}
		static	string				GetAsset(string AssetName);
		static	string				GetScene(string SceneName);
        static	string				GetSolution(string SceneName);
		static	string				GetFont(string SceneName);
		static	string				GetTexture(string SceneName);

		static	string				GetBlankTexture();
        static	string				GetSequence(string SceneName);
		static	SurfaceDescriptor*	GetSurface(string SurfacePath, string SurfaceName);
        static	SurfaceDescriptor*	GetSurface(string SurfaceName);

        static TestVideoInput*      VideoInput;
		
        static void                 SetCurrentDirectory(string directoryName){CurrentDirectory = directoryName+"/";};
        static void                 SetCurrentProject(string project) {CurrentProject = project;}
		                            
	};
	}


}

