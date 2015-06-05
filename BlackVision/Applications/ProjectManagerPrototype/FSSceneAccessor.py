from SceneAccessor import SceneAccessor
from Scene import loadScene, saveScene, Scene, SceneWriter, SceneReader
from Project import Project
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

        resourcesToPack = [r for r in res if self.isProjectResource(r)]

        sw = SceneWriter(scene, outputFile)
        sStr = sw.dumpsScene()

        resultData = {'sceneJson': sStr, 'resourcesData': {}}

        for r in resourcesToPack:
            loc = Location(r)
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
        return res[1:len(projectName)] == projectName


    def unpackSceneAndResources(self, scenePackedFile, toProject, scenePath):
        assert isinstance(toProject, Project)

        sceneAndResources = json.load(scenePackedFile)

        scene = sceneAndResources['sceneJson']

        resources = sceneAndResources['resourcesData']

        toProject.appendScene(scene, scenePath)