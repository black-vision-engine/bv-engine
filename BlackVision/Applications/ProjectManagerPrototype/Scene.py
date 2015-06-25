
import pickle
import traceback

class SceneWriter:
    def __init__(self, scene, outputFile = None):
        self.outputFile = outputFile
        self.scene = scene

    def saveScene(self):
        return self.__serializeScene()

    def __serializeScene(self):
        assert self.outputFile
        try:
            s = None
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

def saveScene(scene, outputFile):
    sr = SceneWriter(scene, outputFile)
    return sr.saveScene()

def loadScene(sceneFile):
    sr = SceneReader(sceneFile)
    return sr.loadScene()







