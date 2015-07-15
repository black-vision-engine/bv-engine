
import pickle
import traceback
import os
class SceneWriter:
    def __init__(self, scene, outputFile = None):
        self.outputFile = outputFile
        self.scene = scene

    def saveScene(self):
        return self.__serializeScene()

    def __serializeScene(self):
        assert self.outputFile
        try:

            dirName = os.path.dirname(self.outputFile)
            if not os.path.exists(dirName):
                os.makedirs(dirName)

            with open(self.outputFile, 'wb') as f:
                pickle.dump(self.scene, f, protocol = 0)
            return True
        except Exception as exc:
            print("Cannot save scene '{}' to file '{}':". format(self.scene.getName(), self.outputFile))
            print(traceback.format_exc())
            return False

    def dumpsScene(self):
        return pickle.dumps(self.scene)




class SceneReader:
    def __init__(self, sceneFileDef):
        self.sceneFileDef = sceneFileDef

    def loadScene(self):
        return self.__parseSceneFileDef()

    def __parseSceneFileDef(self):
        try:
            s = None
            with open(self.sceneFileDef, "rb") as f:
                s = pickle.load(f)
            return s
        except Exception as exc:
            print("Cannot load scene from file '{}':". format(self.sceneFileDef))
            print(exc)
            return None

class Plugin:
    def __init__(self, name):
        self.name = name
        self.resources = []

    def addResource(self, projectName, categoryName, path):
        self.resources.append((projectName, categoryName, path))

    def remapResourcesPaths(self, oldProjectName, newProjectName):
        for i in range(len(self.resources)):
            if self.resources[i][0] == oldProjectName:
                self.resources[i] = (newProjectName, self.resources[i][1], self.resources[i][2])

    def check(self, rootPath):
        for r in self.resources:
            path = os.path.join(rootPath, r[1], r[0], r[2])
            if not os.path.exists(path):
                print("Checking Plugin: Cannot find resource {}".format(path))
                return False
        return True

class Node:
    def __init__(self, name):
        self.name = name
        self.childrenNode = []
        self.plugins = []

    def addPlugin(self, plugin):
        assert isinstance(plugin, Plugin)
        self.plugins.append(plugin)

    def addChildNode(self, node):
        assert isinstance(node, Node)
        self.childrenNode.append(node)

    def remapResourcesPaths(self, oldProjectName, newProjectName):
        for pl in self.plugins:
            pl.remapResourcesPaths(oldProjectName, newProjectName)

        for n in self.childrenNode:
            n.remapResourcesPaths(oldProjectName, newProjectName)

    def check(self, rootPath):
        for pl in self.plugins:
            if not pl.check(rootPath):
                return False

        for n in self.childrenNode:
            if not n.check(rootPath):
                return False

        return True

class Scene:
    def __init__(self, name, rootNode):
        self.name = name
        self.rootNode = rootNode

    def getName(self):
        return self.name

    def __listAssetsInTree(self, node, resources):
        for pl in node.plugins:
            resources += (pl.resources)

        for n in node.childrenNode:
            self.__listAssetsInTree(n, resources)

    def listAssets(self):
        resources = []
        self.__listAssetsInTree(self.rootNode, resources)

        return resources

    def remapResourcesPaths(self, oldProjectName, newProjectName):
        self.rootNode.remapResourcesPaths(oldProjectName, newProjectName)

    def check(self, rootPath):
        self.rootNode.check(rootPath)


def saveScene(scene, outputFile):
    sr = SceneWriter(scene, outputFile)
    return sr.saveScene()

def loadScene(sceneFile):
    sr = SceneReader(sceneFile)
    return sr.loadScene()







