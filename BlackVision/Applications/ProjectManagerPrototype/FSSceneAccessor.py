from SceneAccessor import SceneAccessor
from Scene import loadScene, saveScene, Scene, SceneWriter, SceneReader
from Location import Location

import os, json, tempfile, shutil

class SceneDesc:
    def __init__(self, absPath):
        self.absPath = absPath

class FSSceneAccessor(SceneAccessor):
    def __init__(self, rootPath, project):
        SceneAccessor.__init__(self)
        self.rootPath = rootPath
        self.project = project
        self.__createDir()

    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)

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
        self.packSceneAndResources(s, expSceneFilePath)

    def packSceneAndResources(self, scene, outputFile):
        assert self.project
        assert isinstance(scene, Scene)
        res = scene.listResources()

        sw = SceneWriter(scene, outputFile)
        sStr = sw.dumpsScene()

        resultData = {'sceneJson': sStr, 'resourcesData': {}}

        for r in res:
            loc = Location(r[0], r[1])
            tmp = tempfile.NamedTemporaryFile(delete=False)
            filename = tmp.name
            tmp.close()
            self.project.exportData(filename, loc.getCategoryName(), loc.getInternalPath())

            resultData['resourcesData'][r] = open(filename, "r").read()
            os.remove(filename)

        json.dump(resultData, outputFile)


    def isProjectResource(self, res):
        assert isinstance(res, str)
        assert self.project
        projectName = self.project.getName()
        return res[0] == projectName


    def unpackSceneAndResources(self, scenePackedFile, toProject, scenePath):

        sceneAndResources = json.load(scenePackedFile)

        scene = sceneAndResources['sceneJson']

        resources = sceneAndResources['resourcesData']

        toProject.appendScene(scene, scenePath)