from abc import abstractmethod

class SceneAccessor:
    def __init__(self):
        pass

    @abstractmethod
    def getSceneDesc(self, path):
        pass

    @abstractmethod
    def importScene(self, impSceneFile, importToPath):
        pass

    @abstractmethod
    def exportScene(self, expSceneFilePath, internalPath):
        pass