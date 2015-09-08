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
	model::TimelineManager *	    m_tm;
	const model::PluginsManager *	m_pm;

	explicit			SceneAccessor			( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm );

	static PathVec		GetAllUsedAssetPaths	( const model::BasicNodeConstPtr & scene, const Path & relativeTo );
	static PathVec		GetAllPathsFromAssets	( const AssetDescConstPtr & assetDesc, const Path & relativeTo );
    static std::vector< AssetDescConstPtr >     ListSceneAssetsDescs( const model::BasicNodeConstPtr & scene );
    static std::vector< AssetDescConstPtr >     UnpackSimpleAssets( const AssetDescConstPtr & assetDesc );
    static Path         AssetDescToPath         ( const AssetDescConstPtr & desc );
    static void         ReplaceRootDir          ( const AssetDescConstPtr & p, const Path & oldPMRootDir, const Path & newPMRootDir );
    static void         ReplaceProjectName      ( const AssetDescConstPtr & p, const Path & oldProjectName, const Path & newProjectName );
    static void         ReplacePathInSimpleAsset( const AssetDescConstPtr & ad, const Path & newAssetPath );

public:

	SceneDescriptor		GetSceneDesc			( const Path & path ) const;
	model::BasicNodeConstPtr		GetScene				( const Path & path ) const;

	void				AddSceneFromFile		( const Path & srcPath, const Path & path ) const;
	void				AddScene				( const model::BasicNodeConstPtr & scene, const Path & path ) const;
	void				RemoveScene				( const Path & path ) const;

	void				ImportScene				( std::istream & in,  const Path & importToProject, const Path & importToPath ) const;
	void				ExportScene				( std::ostream & out, const Path & path, bool withAssets ) const;

	void				ImportSceneFromFile		( const Path & expFilePath, const Path & importToProject, const Path & importToPath ) const;
	void				ExportSceneToFile		( const Path & outputFileName, const Path & path, bool withAssets ) const;

	PathVec				ListScenes				( const Path & path ) const;

	PathVec				ListAllUsedAssets		( const Path & path ) const;

	static SceneAccessorConstPtr Create			( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm );
};


} // bv