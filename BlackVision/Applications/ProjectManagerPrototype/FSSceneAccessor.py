from SceneAccessor import SceneAccessor
from Scene import loadScene, saveScene, Scene, Node, SceneWriter, SceneReader

import os, pickle, tempfile, shutil

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

    def addSceneFromFile(self, path, sceneFilePath):
        absPath = os.path.join(self.rootPath, path)
        dirname = os.path.dirname(absPath)
        try:
            if not os.path.exists(dirname):
                os.makedirs(dirname)

            shutil.copyfile(sceneFilePath, absPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def saveScene(self, path, scene):
        absPath = os.path.join(self.rootPath, path)
        dirname = os.path.dirname(absPath)
        try:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
            sw = SceneWriter(scene, absPath)
            sw.saveScene()
            return True
        except Exception as exc:
            print(exc)
            return False

    def createScene(self, name, path):
        absPath = os.path.join(self.rootPath, path)

        if os.path.exists(absPath):
            print("Cannot create. Scene '{}' already exists.".format(absPath))
            return False

        dirname = os.path.dirname(absPath)
        try:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
            sw = SceneWriter(Scene(name, Node(name)), absPath)
            sw.saveScene()
            return True
        except Exception as exc:
            print(exc)
            return False

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
            tmp = tempfile.NamedTemporaryFile(delete=False)
            filename = tmp.name
            tmp.close()
            from ProjectManager import PM
            PM.exportAssetToFile(r[0], r[1], filename)

            resultData['resourcesData'][r] = open(filename, "rb").read()
            os.remove(filename)

        with open(outputFile, "wb") as f:
            pickle.dump(resultData, f)


    def isProjectResource(self, res):
        assert isinstance(res, str)
        assert self.project
        projectName = self.project.getName()
        return res[0] == projectName


    def unpackSceneAndResources(self, scenePackedFile, toProject, scenePath):

        sceneAndResources = pickle.load(scenePackedFile)

        scene = sceneAndResources['sceneJson']

        resources = sceneAndResources['resourcesData']

        toProject.appendScene(scene, scenePath)