from SceneAccessor import SceneAccessor
from Scene import loadScene, saveScene, Scene, Node, SceneWriter, SceneReader

from Location import Location

import os, pickle, tempfile, shutil

class SceneDesc:
    def __init__(self, absPath):
        self.absPath = absPath

class FSSceneAccessor(SceneAccessor):
    def __init__(self, projectManager, project):
        SceneAccessor.__init__(self)
        self.projectManager = projectManager
        self.rootPath = projectManager.getRootDir()
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
        absPath = os.path.join(self.rootPath, importToPath)
        self.unpackSceneAndResources(impSceneFile, self.project, importToPath)

    def exportScene(self, expSceneFilePath, internalPath):
        absPath = os.path.join(self.rootPath, internalPath)
        s = loadScene(absPath)
        self.packSceneAndResources(s, expSceneFilePath)

    def packSceneAndResources(self, scene, outputFile):
        assert self.project
        assert isinstance(scene, Scene)
        res = scene.listResources()

        resultData = {"ownerProjectName": self.project.getName(), "sceneJson": scene, "resourcesData": {}}

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
        try:
            with open(scenePackedFile, "rb") as f:
                sceneAndResources = pickle.load(f)

            scene = sceneAndResources["sceneJson"]
            assert False  # TODO: Add remaping project name to the new one. toProject.getName()
            toProject.saveScene(scene, scenePath)

            resources = sceneAndResources["resourcesData"]

            ownerProjectName = sceneAndResources["ownerProjectName"]

            assert isinstance(resources, dict)
            for r in resources.keys():
                if(ownerProjectName == r[0][0]):
                    loc = Location(r[0][0], r[0][1])
                    tmp = tempfile.NamedTemporaryFile(delete=False)
                    filename = tmp.name
                    tmp.close()

                    with open(filename, "wb") as f:
                        f.write(resources[r[0]])
                    toProject.importData(filename, loc.getCategoryName(), loc.getInternalPath())
                    os.remove(filename)
                else:
                    loc = Location(r[0][0], r[0][1])
                    self.projectManager

            return True
        except Exception as exc:
            print("Cannot unpack scene file '{}'".format(scenePackedFile))
            print(exc)
            return False
