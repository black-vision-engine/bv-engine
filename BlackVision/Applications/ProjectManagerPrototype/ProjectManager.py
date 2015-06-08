from DataCategory import DataCategory
from Project import Project
from Location import Location
from SceneAccessor import SceneAccessor

class ProjectManager:

    def __init__(self):
        self.projects       = {}
        self.currentProject = None
        self.globalCategories = {}
        self.globalSceneAccessor = None

    def getAssetDesc(self, path):
        assert isinstance(path, str)

        loc = Location(path, self.currentProject.getName() if self.currentProject else "")

        if loc:
            if loc.getIsGlobalLocation():
                catName = loc.getCategoryName()
                if catName in self.globalCategories:
                    return self.globalCategories[catName].getData(loc.getInternalPath())
            else:
                proj = self.getProject(loc.getProjectName())
                if proj:
                    return proj.getData(loc.getCategoryName(), loc.getInternalPath())

        print("Cannot find asset '{}'".format(path))
        return None

    def getSceneDesc(self, path):
        assert isinstance(path, str)

        loc = Location(path, self.currentProject.getName() if self.currentProject else "")

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

        print("Cannot find scene '{}'".format(path))
        return None

    def setGlobalSceneAccessor(self, sceneAccessor):
        assert isinstance(sceneAccessor, SceneAccessor)
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


    def moveAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyAsset(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def exportAsset(self, projectName, assetPath):
        assert False  # TODO: Implement

    def exportAssetToFile(self, projectName, assetPath, outputFile):
        assert False  # TODO: Implement

    def importAssetFromFile(self, importToProjectName, importToPath, importData):
        assert False  # TODO: Implement

    def importAssetFromFile(self, importToProjectName, importToPath, importDataFilePath):
        assert False  # TODO: Implement

    def moveScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def copyScene(self, fromProjectName, fromInternalPath, toProjectName, toInternalPath):
        assert False  # TODO: Implement

    def exportScene(self, projectName, scenePath):
        assert False  # TODO: Implement

    def exportSceneToFile(self, projectName, scenePath, outputFile):
        assert False  # TODO: Implement

    def importSceneFromFile(self, importToProjectName, importToPath, importData):
        assert False  # TODO: Implement

    def importSceneFromFile(self, importToProjectName, importToPath, importDataFilePath):
        assert False  # TODO: Implement
