from AssetCategory import AssetCategory
from AssetDesc import AssetDesc
from SceneAccessor import SceneAccessor
from ProjectExportDesc import ProjectExportDesc

from FSSceneAccessor import FSSceneAccessor
from FSTextureAssetAccessor import FSTextureAssetAccessor
from FSSequenceAssetAccessor import FSSequenceDataAccessor

import os

class Project:

    def __init__(self, projectManager, name):
        assert name not in ["project", "global"]  # TODO: Make better constraints for project name
        self.name = name
        self.projectManager = projectManager
        self.rootDir = os.path.abspath(projectManager.getRootDir())
        self.categories = {}
        self.__initialize()

    def __initialize(self):

        self.__createDir()

        self.categories["textures"]     = AssetCategory("textures", FSTextureAssetAccessor(os.path.join(self.rootDir, "textures", self.name), ['jpg', 'tga']))
        self.categories["sequences"]    = AssetCategory("sequences", FSSequenceDataAccessor(os.path.join(self.rootDir, "sequences", self.name), ['jpg', 'tga']))

        self.projectManager.registerGlobalCategory(AssetCategory("textures", FSTextureAssetAccessor(os.path.join(self.rootDir, "textures"), ['jpg', 'tga'])))
        self.projectManager.registerGlobalCategory(AssetCategory("sequences", FSSequenceDataAccessor(os.path.join(self.rootDir, "sequences"), ['jpg', 'tga'])))

        self.sceneAccessor = FSSceneAccessor(self.projectManager, self)

    def __createDir(self):
        if not os.path.exists(os.path.join(self.rootDir, "projects", self.name)):
            projPath = os.path.join(self.rootDir, "projects", self.name)
            os.makedirs(projPath)
            with open(os.path.join(projPath, ".bvproj"), "w"):
                pass

    def getName(self):
        return self.name

    def registerCategory(self, category):
        assert isinstance(category, AssetCategory)
        if category.getId() not in self.categories:
            self.categories[category.getId()] = category
        else:
            print("Category '{}' already registered".format(category.getId()))

    def appendData(self, categoryId, path, loadableData):
        assert isinstance(path, str)
        assert isinstance(loadableData, AssetDesc)
        if categoryId in self.categories:
            self.categories[categoryId].appendData(path, loadableData)
        else:
            print("There is no category named {} in project {}".format(categoryId, self.name))

    def getData(self, categoryId, path):
        assert isinstance(path, str)
        if categoryId in self.categories:
            return self.categories[categoryId].getData(path)
        else:
            print("There is no category named {} in project {}".format(categoryId, self.name))
            return None

    def copyData(self, categoryId, path):
        assert isinstance(path, str)
        if categoryId in self.categories:
            return self.categories[categoryId].copyData(path)
        else:
            print("There is no category named {} in project {}".format(categoryId, self.name))
            return None

    def exportData(self, expDataFilePath, categoryId, path):
        assert isinstance(path, str)
        if categoryId in self.categories:
            return self.categories[categoryId].exportData(expDataFilePath, path)
        else:
            print("There is no category named {} in project {}".format(categoryId, self.name))
            return None

    def importData(self, impDataFilePath, categoryId, toPath):
        assert isinstance(toPath, str)
        if categoryId in self.categories:
            return self.categories[categoryId].importData(impDataFilePath, toPath)
        else:
            print("There is no category named {} in project {}".format(categoryId, self.name))
            return None

    def exportSceneToFile(self,  expDataFilePath, path):
        self.sceneAccessor.exportSceneToFile(path, expDataFilePath)

    def importSceneFromFile(self,  impDataFilePath, path):
        self.sceneAccessor.importSceneFromFile(impDataFilePath, path)

    def getSceneDesc(self, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        return self.sceneAccessor.getSceneDesc(path)

    def appendSceneFromFile(self, sceneFilePath, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        self.sceneAccessor.addSceneFromFile(path, sceneFilePath)

    def saveScene(self, scene, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        self.sceneAccessor.saveScene(path, scene)

    def copyScene(self, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        return self.sceneAccessor.copyScene(path)

    def getExportDesc(self):
        assetsDescs = set()
        for c in self.categories.values():
            expDescs = c.accessor.listAllUniqueExportDesc(self.rootDir)
            for ed in expDescs:
                assetsDescs.add(ed)

        scenesDescs = []
        for sd in self.sceneAccessor.listAllExportDesc():
            scenesDescs.append(sd)

        return ProjectExportDesc(self.name, self.rootDir, scenesDescs, assetsDescs)

    def exportToFile(self, outputFileName):
        desc = self.getExportDesc()
        desc.saveExportPackageToFile(outputFileName)

    def listScenes(self):
        return self.sceneAccessor.listScenes()

    def listCategories(self):
        return [c for c in self.categories.keys()]

    def __str__(self):
        print("Project {} \n\t".format(self.name))
