from abc import abstractmethod

class SceneAccessor:
    def __init__(self):
        pass

    @abstractmethod
    def getSceneDesc(self, path):
        pass