from SceneAccessor import SceneAccessor
from Scene import packSceneAndResources, loadScene, saveScene
import os

class SceneDesc:
    def __init__(self, absPath):
        self.absPath = absPath

class FSSceneAccessor(SceneAccessor):
    def __init__(self, rootPath, project):
        SceneAccessor.__init__(self)
        self.rootPath = rootPath
        self.project = project

    def getSceneDesc(self, path):
        absPath = os.path.join(self.rootPath, path)
        if os.path.exists(absPath):
            return SceneDesc(absPath)
        else:
            return None

    def importScene(self, impSceneFile, importToPath):

        pass

    def exportScene(self, expSceneFilePath, internalPath):
        absPath = os.path.join(self.rootPath, internalPath)
        s = loadScene(absPath)
        packSceneAndResources(s, self.project, expSceneFilePath)

    def copyScene(self, path):
