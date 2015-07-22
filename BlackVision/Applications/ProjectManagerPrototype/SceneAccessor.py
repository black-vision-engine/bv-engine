from abc import abstractmethod

class SceneAccessor:
    def __init__(self, projectManager):
        self.projectManager = projectManager

    @abstractmethod
    def getScene(self, path):
        pass

    @abstractmethod
    def createScene(self, name, path):
        pass

    @abstractmethod
    def addSceneFromFile(self, path, sceneFilePath):
        pass

    @abstractmethod
    def addScene(self, scene, outPath):
        pass

    @abstractmethod
    def removeScene(self, path):
        pass

    @abstractmethod
    def importScene(self, impSceneFile, project, importToPath):
        pass

    @abstractmethod
    def exportSceneToFile(self, internalPath, outputFileName):
        pass

    @abstractmethod
    def exportScene(self, expSceneFilePath, project, internalPath):
        pass

    @abstractmethod
    def listScenes(self, projectName):
        pass

    @abstractmethod
    def listAllUniqueExportDesc(self):
        pass