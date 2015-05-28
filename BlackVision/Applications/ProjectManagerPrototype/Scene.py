from Project import Project
from DataAccessor import DataAccessor
from Location import Location
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
        self.resources = []

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

def isProjectResource(projectName, res):
    assert isinstance(res, str)
    return res[1:len(projectName)] == projectName

def packSceneAndResources(scene, project, outputFile):
    assert isinstance(project, Project)
    assert isinstance(scene, Scene)
    res = scene.listResources()

    resourceToPack = [r for r in res if isProjectResource(project.getName(), r)]

    sw = SceneWriter(scene, outputFile)
    sStr = sw.dumpsScene()

    resultData = { 'sceneJson' : sStr, 'resourcesData' : {} }

    for r in resourceToPack:
        loc = Location(r)
        resultData['resourcesData'][r] = project.copyData(loc.getCategoryName(), loc.getInternalPath())

    json.dump(resultData, outputFile)







