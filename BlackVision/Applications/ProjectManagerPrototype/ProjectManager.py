from AssetCategory import AssetCategory
from Project import Project
from Location import Location
from FSSceneAccessor import FSSceneAccessor, SceneAccessor
from ProjectExportDesc import ProjectExportDesc
from SceneExportDesc import SceneExportDesc
from Scene import SceneWriter


import pickle
import zipfile
import uuid

import os


class ProjectManager:

    def __init__(self, rootDir):
        self.projects       = {}
        self.currentProject = None
        self.globalCategories = {}
        self.globalSceneAccessor = None
        self.rootDir        = os.path.abspath(rootDir)

        self.__initializeProjectManager()

    def __createDir(self):
        if not os.path.exists(self.rootDir):
            os.makedirs(self.rootDir)

    def __setGlobalSceneAccessor(self, sceneAccessor):
        self.globalSceneAccessor = sceneAccessor

    def __initializeProjectManager(self):
        self.__createDir()
        self.__setGlobalSceneAccessor(FSSceneAccessor(self, None))

        if os.path.exists(self.rootDir):
            self.__initializeProjects()

    def __initializeProjects(self):
        projectsPath = os.path.join(self.rootDir, "projects")
        if os.path.exists(projectsPath):
            for dirpath, dirnames, filenames in os.walk(projectsPath):
                for fn in filenames:
                    if fn == ".bvproj":
                        projName = os.path.normpath(os.path.relpath(dirpath, projectsPath))
                        if projName not in self.projects:
                            self.projects[projName] = Project(self, projName)

    def getAssetDesc(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        loc = Location(projectName, pathInProject, self.currentProject.getName() if self.currentProject else "")

        if loc:
            if loc.getIsGlobalLocation():
                catName = loc.getCategoryName()
                if catName in self.globalCategories:
                    return self.globalCategories[catName].getAssetDesc(loc.getInternalPath())
            else:
                proj = self.getProject(loc.getProjectName())
                if proj:
                    return proj.getAssetDesc(loc.getCategoryName(), loc.getInternalPath())

        print("Cannot find asset '{}  {}'".format(projectName, pathInProject))
        return None

    def getSceneDesc(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        loc = Location(projectName, "scenes", pathInProject, self.currentProject.getName() if self.currentProject else "")

        if loc:
            if loc.getIsGlobalLocation():
                catName = loc.getCategoryName()

                assert catName == "scenes"

                if self.self.globalSceneAccessor:
                    return self.self.globalSceneAccessor.getSceneDesc(loc.getInternalPath())
            else:
                proj = self.getProject(loc.getProjectName())
                if proj:
                    return proj.getSceneDesc(loc.getInternalPath())

        print("Cannot find scene '{}  {}'".format(projectName, pathInProject))
        return None

    def getProject(self, name):
        assert isinstance(name, str)
        if name in self.projects:
            return self.projects[name]
        else:
            print("Cannot find project '{}'".format(name))
            return None

    def listProjectsNames(self):
        return self.projects.keys()

    def listScenes(self, projectName = None):
        if not projectName:
            return self.globalSceneAccessor.listScenes()
        else:
            assert isinstance(projectName, str)
            proj = self.getProject(projectName)
            if proj:
                return proj.listScenes()
            else:
                print("Project named {} doesn't exist".format(projectName))

    def listCategories(self, projectName = None):
        if not projectName:
            return [c for c in self.globalCategories.keys()]
        else:
            proj = self.getProject(projectName)
            if proj:
                return proj.listCategories()
            else:
                print("Project named {} doesn't exist".format(projectName))

    def listAssets(self, projectName = None, categoryName = None ):
        if not projectName:
            if not categoryName:
                res = []
                for cn in self.globalCategories.keys():
                    res.append(self.globalCategories[cn].listAssets())
                return res

        # else:
        #     proj = self.getProject(projectName)
        #     if proj:
        #         return proj.listCategories()
        #     else:
        #         print("Project named {} doesn't exist".format(projectName))


    def setCurrentProject(self, projectName):
        if projectName in self.projects:
            self.currentProject = self.projects[projectName]
        else:
            print("Cannot set current project. Project '{}' doesn't exist".format(projectName))

    def addNewProject(self, projectName):
        assert isinstance(projectName, str)
        if not projectName in self.projects:
            self.projects[projectName] = Project(self, projectName)
        else:
            print("Project '{}' already exists. Cannot add".format(projectName))

    def registerGlobalCategory(self, category):
        assert isinstance(category, AssetCategory)
        if not category.getId() in self.globalCategories:
            self.globalCategories[category.getId()] = category
        else:
             print("Cannot register global category '{}'. Already registered.".format(category.getId()))

    def getRootDir(self):
        return self.rootDir

    ###########################################################################
    # Operations inside one project manager
    # TODO: Implement
    def moveAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def moveScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    ###########################################################################
    # Exporting and importing
    ###########################################################################

    ###########################################################################
    # Exporting and importing assets (textures, sequences etc.)
    def exportAssetToFile(self, projectName, categoryName, assetPath, outputFile):
        self.getProject(projectName).exportAsset(outputFile, categoryName, assetPath)

    def importAssetFromFile(self, importToProjectName, importToCategoryName, importToPath, importAssetFilePath):
        if importToProjectName == ".":
            importToProjectName == self.currentProject.getName()

        if len(importToProjectName) == 0:
            if importToCategoryName in self.globalCategories:
                self.globalCategories[importToCategoryName].importAsset(importAssetFilePath, importToPath)
        else:
            self.getProject(importToProjectName).importAsset(importAssetFilePath, importToCategoryName, importToPath)

    ###########################################################################
    # Exporting and importing whole scenes
    def exportSceneToFile(self, projectName, scenePath, outputFile):

        assert isinstance(self.globalSceneAccessor, SceneAccessor)

        if len(projectName) == 0:
            assert False  # TODO: Implement global accessor case
        else:
            proj = self.getProject(projectName)
            if proj:
                proj.exportSceneToFile(outputFile, scenePath)
            else:
                print("Cannot export scene '{}' from project '{}'".format(scenePath, projectName))
                return False

    def importSceneFromFile(self, importToProjectName, importToPath, importAssetFilePath):
        self.getProject(importToProjectName).importSceneFromFile(importAssetFilePath, importToPath)

    ###########################################################################
    # Exporting and importing whole projects
    def exportProjectToFile(self, projectName, outputFilePath):
        proj = self.getProject(projectName)
        if proj:
            proj.exportToFile(outputFilePath)
            return True
        else:
            print("Cannot export project '{}'. It doesn't exist.".format(projectName))
            return False

    def importProjectFromFile(self, expFilePath, importToPath=None):

        # {"projectDesc": self, "assetsArchiveData": open(filename, "rb").read()}

        with open(expFilePath, "rb") as f:
            expDescDict = pickle.load(f)

        if isinstance(expDescDict, dict):
            if "projectDesc" in expDescDict:  # project importing

                projDesc = expDescDict["projectDesc"]
                assert isinstance(projDesc, ProjectExportDesc)

                if "assetsArchiveData" in expDescDict:

                    filename = "{}".format(uuid.uuid4())
                    with open(filename, "wb") as f:
                        f.write(expDescDict["assetsArchiveData"])

                    myZipFile = zipfile.ZipFile(filename, "r")

                    if importToPath:
                        for n in myZipFile.namelist():
                            assert isinstance(n, str)
                            with myZipFile.open(n, "r") as f:
                                nn = os.path.normpath(n)
                                outputFileName = os.path.join(self.rootDir, nn.replace("\\" + projDesc.name + "\\", "\\" + importToPath + "\\") )
                                dirName = os.path.dirname(outputFileName)
                                if not os.path.exists(dirName):
                                    os.makedirs(dirName)

                                with open(outputFileName, "wb") as of:
                                    of.write(f.read())
                    else:
                        myZipFile.extractall(path=self.rootDir)
                    myZipFile.close()

                    os.remove(filename)


                for scenesDesc in projDesc.projectScenesDescs:
                    assert isinstance(scenesDesc, SceneExportDesc)
                    if importToPath:
                        scenOutputFileName = os.path.join(self.rootDir, scenesDesc.relativePath.replace("\\" + projDesc.name + "\\", "\\" + importToPath + "\\"))
                        scenesDesc.scene.remapResourcesPaths(projDesc.name, importToPath)
                    else:
                        scenOutputFileName = os.path.join(self.rootDir, scenesDesc.relativePath)

                    SceneWriter(scenesDesc.scene, scenOutputFileName).saveScene()

                if not self.getProject(importToPath):
                    self.addNewProject(importToPath)


#######################################
# project manager instance
PM = ProjectManager("bv_media")