
import json

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
            with open(self.outputFile, 'w') as f:
                json.dump(self.scene, f)
            return True
        except Exception as exc:
            print("Cannot save scene '{}' to file '{}':". format(self.scene.getName(), self.outputFile))
            print(exc)
            return False

    def dumpsScene(self):
        return json.dumps(self.scene)


class SceneReader:
    def __init__(self, sceneFileDef):
        self.sceneFileDef = sceneFileDef

    def loadScene(self):
        return self.__parseSceneFileDef()

    def __parseSceneFileDef(self):
        try:
            s = None
            with open(self.sceneFileDef) as f:
                s = json.load(f)
            return s
        except Exception as exc:
            print("Cannot load scene from file '{}':". format(self.sceneFileDef))
            print(exc)
            return None

class Plugin:
    def __init__(self, name):
        self.name = name
        self.resources = [()]

    def addResource(self, projectName, path):
        self.resources.append((projectName, path))

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
        self.plugins.append(node)

class Scene:
    def __init__(self, name, rootNode):
        self.name = name
        self.rootNode = rootNode

    def getName(self):
        return self.name

    @staticmethod
    def __listResourceInTree(cls, node, resources):
        for pl in node.plugins:
            resources.append(pl.resources)

        for n in node.childrenNode:
            Scene.__listResourceInTree(n, resources)

    def listResources(self):
        resources = []
        Scene.__listResourceInTree(self.rootNode, resources)

        return resources

def saveScene(scene, outputFile):
    sr = SceneWriter(scene, outputFile)
    return sr.saveScene()

def loadScene(sceneFile):
    sr = SceneReader(sceneFile)
    return sr.loadScene()







