from abc import abstractmethod

class SceneAccessor:
    def __init__(self):
        pass

    @abstractmethod
    def getScene(self, path):
        pass

    @abstractmethod
    def addSceneFromFile(self, path, sceneFilePath):
        pass

    @abstractmethod
    def saveScene(self, path, scene):
        pass

    @abstractmethod
    def importScene(self, impSceneFile, project, importToPath):
        pass

    @abstractmethod
    def exportScene(self, expSceneFilePath, project, internalPath):
        pass