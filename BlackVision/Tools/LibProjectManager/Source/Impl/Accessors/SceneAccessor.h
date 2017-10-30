#pragma once

#include "ProjectManager.h"
#include "SceneDescriptor.h"
#include "System/Path.h"

namespace bv
{

namespace model
{
    class TimelineManager;
    class PluginsManager;
} // model

class SceneAccessor;
DEFINE_CONST_PTR_TYPE( SceneAccessor )

class SceneAccessor
{
private:

    static const std::string SceneFileExt;

    Path							m_rootDirPM;
    Path							m_rootDir;

    explicit			SceneAccessor			( const Path & path );

    //static PathVec		GetAllUsedAssetPaths	( const model::BasicNodeConstPtr & scene, const Path & relativeTo );
    static PathVec		GetAllPathsFromAssets	( const AssetDescConstPtr & assetDesc );
    static AssetDescVec ListSceneAssetsDescs    ( const model::BasicNodeConstPtr & scene );
    static AssetDescVec ListSceneAssetsDescs    ( const Path & sceneFile );
    static AssetDescVec ListSceneAssetsDescs    ( std::istream & in, SizeType numBytes );
    static AssetDescVec UnpackSimpleAssets      ( const AssetDescConstPtr & assetDesc );
    static Path         AssetDescToPath         ( const AssetDescConstPtr & desc );
    static void         ReplaceRootDir          ( const AssetDescConstPtr & p, const Path & oldPMRootDir, const Path & newPMRootDir );
    static void         ReplaceProjectName      ( const AssetDescConstPtr & p, const Path & oldProjectName, const Path & newProjectName );
    static void         ReplacePathInSimpleAsset( const AssetDescConstPtr & ad, const Path & newAssetPath );

public:

    SceneDescriptor		GetSceneDesc			( const Path & path ) const;
    LoadSceneResult     GetScene			    ( const Path & path ) const;

    void				AddSceneFromFile		( const Path & srcPath, const Path & path ) const;
    void				AddScene				( const model::SceneModelPtr & scene, const Path & path ) const;
    void				RemoveScene				( const Path & path ) const;
    ThumbnailConstPtr   GetSceneThumbnail       ( const Path & path ) const;

    void				ImportScene				( std::istream & in,  const Path & importToProject, const Path & importToPath ) const;
    void				ExportScene				( std::ostream & out, const Path & projectName, const Path & path ) const;

    void				ImportSceneFromFile		( const Path & expFilePath, const Path & importToProject, const Path & importToPath ) const;
    void				ExportSceneToFile		( const Path & projectName, const Path & outputFileName, const Path & path ) const;

    PathVec				ListScenes				( const Path & path, bool recursive ) const;

    PathVec				ListAllUsedAssets		( const Path & path ) const;

    static SceneAccessorConstPtr Create			( const Path & path );
};


} // bv