from SceneAccessor import SceneAccessor
from Scene import Scene, SceneReader, SceneWriter
import os

class SceneDesc:
    def __init__(self, absPath):
        self.absPath = absPath

class FSSceneAccessor(SceneAccessor):
    def __init__(self, rootPath):
        SceneAccessor.__init__(self)
        self.rootPath = rootPath

    def getSceneDesc(self, path):
        absPath = os.path.join(self.rootPath, path)
        if os.path.exists(absPath):
            return SceneDesc(absPath)
        else:
            return None

    def importScene(self, impSceneFile, importToPath):
        SceneReader
        pass

    def exportScene(self, expSceneFilePath, internalPath):

        pass

        