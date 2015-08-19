#pragma once

#include "CoreDEF.h"

namespace bv
{

class ProjectManagerImpl;
class Project;
class AssetDesc;

class ProjectManager
{
public:

	// listing
	StringVector					ListProjectsNames	() const;
	StringVector					ListScenesNames		( const std::string & projectName = "" ) const;
	StringVector					ListCategoriesNames	() const;
	StringVector					ListAssetsPaths		( const std::string & projectName,  const std::string & categoryName = ""  ) const;

	/*
	def listAllUsedAssets(self)
	def listAllUnusedAssets(self, projectName, categoryName)
	*/

	std::string						GetRootDir			() const;

	// projects
	void							AddNewProject		( const std::string & projectName );
	const Project *					GetProject			() const;
	void							SetCurrentProject	( const std::string & projectName );

	// assets
	void							AddAsset			( const std::string & projectName, const std::string & categoryName, const std::string & path, const AssetDesc & assetDesc );

	// 
	/*
	def copyAsset(self, inProjectName, inCategoryName, inPath, outProjectName, outPath)
	def removeAsset(self, projectName, categoryName, path)
	def moveAsset(self, inProjectName, inCategoryName, inPath, outProjectName, outPath)

	def removeUnusedAssets(self, projectName = None, categoryName = None)

	def addScene(self, scene, projectName, outPath)
	def copyScene(self, inProjectName, inPath, outProjectName, outPath)
	def removeScene(self, projectName, path)
	def moveScene(self, inProjectName, inPath, outProjectName, outPath)
	*/

	// categories
	/*
	def registerGlobalCategory(self, category)
	*/


	// *********************************
	// exporting importing

	// assets
	/*
	def exportAssetToFile(self, projectName, categoryName, assetPath, outputFile)
	def importAssetFromFile(self, importToProjectName, importToCategoryName, importToPath, importAssetFilePath)
	*/

	// scenes
	/*
	def exportSceneToFile(self, projectName, scenePath, outputFile)
	def importSceneFromFile(self, importToProjectName, importToPath, impSceneFilePath)
	*/

	// projects
	/*
	def exportProjectToFile(self, projectName, outputFilePath)
	def importProjectFromFile(self, expFilePath, importToPath=None)
	*/

	// *********************************
	// getting scenes and assets descriptors
	/*
	def getAssetDescLoc(self, loc)
	def getAssetDesc(self, projectName, categoryName, pathInProject)

	def getSceneDescLoc(self, loc)
	def getSceneDesc(self, projectName, pathInProject)
	*/

	static ProjectManager *			GetInstance		();

private:
	ProjectManagerImpl * m_impl;

	ProjectManager	();
	~ProjectManager	();
};

} // bv