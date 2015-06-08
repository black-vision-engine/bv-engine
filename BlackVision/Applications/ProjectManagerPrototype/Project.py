from DataCategory import DataCategory
from LoadableDataDesc import LoadableDataDesc
from SceneAccessor import SceneAccessor

from FSSceneAccessor import FSSceneAccessor
from FSTextureDataAccessor import FSTextureDataAccessor
from FSFontDataAccessor import FSFontDataAccessor
from FSSequenceDataAccessor import FSSequenceDataAccessor
from FSSurfaceDataAccessor import FSSurfaceDataAccessor

import os

class Project:

    def __init__(self, rootDir, name):
        assert name not in ["project", "global"]  # TODO: Make better constraints for project name
        self.name = name
        self.rootDir = os.path.abspath(rootDir)
        self.__initialize()

    def __initialize(self):
        self.__createDir()

        self.categories = DataCategory("textures", FSTextureDataAccessor(os.path.join(self.rootDir, "textures", self.name), ['jpg', 'tga']))
        self.categories = DataCategory("fonts", FSFontDataAccessor(os.path.join(self.rootDir, "fonts", self.name)))
        self.categories = DataCategory("sequences", FSSequenceDataAccessor(os.path.join(self.rootDir, "sequences", self.name), ['jpg', 'tga']))
        self.categories = DataCategory("surfaces", FSSurfaceDataAccessor(os.path.join(self.rootDir, "surfaces", self.name), ['bvsur']))

        self.sceneAccessor = FSSceneAccessor(os.path.join(self.rootDir, "scenes", self.name), self)

    def __createDir(self):
        if not os.path.exists(os.path.join(self.rootDir, "projects", self.name)):
            os.makedirs(os.path.join(self.rootDir, "projects", self.name))

    def getName(self):
        return self.name

    def registerCategory(self, category):
        assert isinstance(category, DataCategory)
        if category.getId() not in self.categories:
            self.categories[category.getId()] = category
        else:
            print("Category '{}' already registered".format(category.getId()))

    def appendData(self, categoryId, path, loadableData):
        assert isinstance(path, str)
        assert isinstance(loadableData, LoadableDataDesc)
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

    def getScene(self, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        return self.sceneAccessor.getSceneDesc()

    def setSceneAccessor(self, sceneAccessor):
        assert isinstance(sceneAccessor, SceneAccessor)
        self.sceneAccessor = sceneAccessor

    def appendScene(self, scene, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        self.sceneAccessor.addScene(path, scene)

    def copyScene(self, path):
        assert isinstance(self.sceneAccessor, SceneAccessor)
        return self.sceneAccessor.copyScene(path)


    def __str__(self):
        print("Project {} \n\t".format(self.name))
        print("Scenes: {}".format(self.projectScenes.keys()))
        print("Used assets: {}".format(self.projectAssets))
