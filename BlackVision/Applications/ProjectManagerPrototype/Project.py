from DataCategory import DataCategory
from LoadableDataDesc import LoadableDataDesc

class Project:

    def __init__(self, name):
        self.name           = name
        self.categories     = {}

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

    def __str__(self):
        print("Project {} \n\t".format(self.name))
        print("Scenes: {}".format(self.projectScenes.keys()))
        print("Used assets: {}".format(self.projectAssets))
