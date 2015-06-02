from abc import abstractmethod

class SceneAccessor:
    def __init__(self):
        pass

    @abstractmethod
    def getSceneDesc(self, path):
        pass

    @abstractmethod
    def importScene(self, impSceneFile, project, importToPath):
        pass

    @abstractmethod
    def exportScene(self, expSceneFilePath, project, internalPath):
        pass