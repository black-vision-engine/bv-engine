from DataCategory import DataCategory
from LoadableDataDesc import LoadableDataDesc
from SceneAccessor import SceneAccessor

class Project:

    def __init__(self, name):
        assert name not in ["project", "global"]  # TODO: Make better constraints for project name
        self.name = name
        self.categories = {}
        self.sceneAccessor = None

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

    def getSceneDesc(self, path):
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
