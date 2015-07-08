from DataCategory import DataCategory
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
        self.__createDir()
        self.setGlobalSceneAccessor(FSSceneAccessor(self, None))

    def __createDir(self):
        if not os.path.exists(self.rootDir):
            os.makedirs(self.rootDir)

    def getAssetDesc(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        loc = Location(projectName, pathInProject, self.currentProject.getName() if self.currentProject else "")

        if loc:
            if loc.getIsGlobalLocation():
                catName = loc.getCategoryName()
                if catName in self.globalCategories:
                    return self.globalCategories[catName].getData(loc.getInternalPath())
            else:
                proj = self.getProject(loc.getProjectName())
                if proj:
                    return proj.getData(loc.getCategoryName(), loc.getInternalPath())

        print("Cannot find asset '{}  {}'".format(projectName, pathInProject))
        return None

    def getSceneDesc(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        loc = Location(projectName, pathInProject, self.currentProject.getName() if self.currentProject else "")

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

    def setGlobalSceneAccessor(self, sceneAccessor):
        self.globalSceneAccessor = sceneAccessor

    def getProject(self, name):
        assert isinstance(name, str)
        if name in self.projects:
            return self.projects[name]
        else:
            print("Cannot find project '{}'".format(name))
            return None

    def listProjectsNames(self):
        return self.projects.keys()

    def setCurrentProject(self, projectName):
        if projectName in self.projects:
            self.currentProject = self.projects[projectName]
        else:
            print("Cannot set current project. Project '{}' doesn't exist".format(projectName))

    def addProject(self, project):
        assert isinstance(project, Project)
        if not project.getName() in self.projects:
            self.projects[project.getName()] = project
        else:
            print("Project '{}' already exists. Cannot add".format(project.getName()))

    def registerGlobalCategory(self, category):
        assert isinstance(category, DataCategory)
        if not category.getId() in self.globalCategories:
            self.globalCategories[category.getId()] = category
        else:
             print("Cannot register global category '{}'. Already registered.".format(category.getId()))


    def getRootDir(self):
        return self.rootDir

    def moveAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def exportAsset(self, projectName, assetPath):
        assert False  # TODO: Implement

    def exportAssetToFile(self, projectName, categoryName, assetPath, outputFile):
        self.getProject(projectName).exportData(outputFile, categoryName, assetPath)

    def exportProjectToFile(self, projectName, outputFileName):
        proj = self.getProject(projectName)
        if proj:
            return proj.exportToFile(outputFileName)
        else:
            print("Cannot export project '{}'".format(projectName))
            return False

    def importAsset(self, importToProjectName, importToPath, importData):
        assert False  # TODO: Implement

    def importAssetFromFile(self, importToProjectName, importToCategoryName, importToPath, importDataFilePath):
        if importToProjectName == ".":
            importToProjectName == self.currentProject.getName()

        if len(importToProjectName) == 0:
            if importToCategoryName in self.globalCategories:
                self.globalCategories[importToCategoryName].importData(importDataFilePath, importToPath)
        else:
            self.getProject(importToProjectName).importData(importDataFilePath, importToCategoryName, importToPath)

    def moveScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def exportScene(self, projectName, scenePath):
        assert False  # TODO: Implement

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

    def importSceneFromFile(self, importToProjectName, importToPath, importData):
        assert False  # TODO: Implement

    def importSceneFromFile(self, importToProjectName, importToPath, importDataFilePath):
        self.getProject(importToProjectName).importScene(importDataFilePath, importToPath)

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
                        # TODO: Add changing names of assets in scene.
                    else:
                        scenOutputFileName = os.path.join(self.rootDir, scenesDesc.relativePath)

                    SceneWriter(scenesDesc.scene, scenOutputFileName).saveScene()

                if not self.getProject(importToPath):
                    self.addProject(Project(self, importToPath))



PM = ProjectManager("bv_media")