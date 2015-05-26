from SceneAccessor import SceneAccessor
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

    def importScene(self, sceneDesc):
        assert isinstance(sceneDesc, SceneDesc)
        assert False  # TODO: Implement

    def exportScene(self, path):
        absPath = os.path.join(self.rootPath, path)
        assert False  # TODO: Implement
        if os.path.exists(absPath):
            return None
        else:
            return None

        