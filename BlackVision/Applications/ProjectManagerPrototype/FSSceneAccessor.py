from SceneAccessor import SceneAccessor
from SceneExportDesc import SceneExportDesc
from AssetExportDesc import AssetExportDesc
from Scene import loadScene, Scene, Node, SceneWriter, SceneReader

import uuid, zipfile

import os, pickle, tempfile, shutil

class SceneDesc:
    def __init__(self, absPath):
        self.absPath = absPath

class FSSceneAccessor(SceneAccessor):
    def __init__(self, projectManager, project):
        SceneAccessor.__init__(self, projectManager)
        self.projectManager = projectManager
        self.rootPath = os.path.join(projectManager.getRootDir(), "scenes", project.getName() if project else "")
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

    def importSceneFromFile(self, expFilePath, importToPath):

        absSceneFilePath = os.path.join(self.rootPath, importToPath)

        with open(expFilePath, "rb") as f:
            expDescDict = pickle.load(f)

        if isinstance(expDescDict, dict):
            if "sceneDesc" in expDescDict:  # project importing

                sceneDesc = expDescDict["sceneDesc"]
                assert isinstance(sceneDesc, SceneExportDesc)

                if "assetsArchiveData" in expDescDict:

                    filename = "{}".format(uuid.uuid4())
                    with open(filename, "wb") as f:
                        f.write(expDescDict["assetsArchiveData"])

                    myZipFile = zipfile.ZipFile(filename, "r")
                    myZipFile.extractall(path=self.projectManager.getRootDir())
                    myZipFile.close()

                    os.remove(filename)

                SceneWriter(sceneDesc.scene, absSceneFilePath).saveScene()

    def exportSceneToFile(self, internalPath, outputFileName):
        expDesc = self.getExportDesc(internalPath)

        filename = "{}".format(uuid.uuid4())

        AssetExportDesc.packAssetsToFile(self.projectManager.getRootDir(), filename, expDesc.sceneAssetsDescs)

        with open(outputFileName, "wb") as f:
            pickle.dump({"sceneDesc": expDesc, "assetsArchiveData": open(filename, "rb").read()}, f)

        os.remove(filename)

    def getExportDesc(self, internalPath):
        absPath = os.path.join(self.rootPath, internalPath)
        s = loadScene(absPath)

        return SceneExportDesc(s, self.project.getName(), internalPath)

    def packSceneAndResources(self, scene, outputFile):
        assert self.project
        assert isinstance(scene, Scene)
        res = scene.listAssets()

        resultData = {"ownerProjectName": self.project.getName(), "sceneJson": scene, "resourcesData": {}}

        for r in res:
            tmp = tempfile.NamedTemporaryFile(delete=False)
            filename = tmp.name
            tmp.close()
            from ProjectManager import PM
            PM.exportAssetToFile(r[0], r[1], r[2], filename)

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
            # assert False  # TODO: Add remaping project name to the new one. toProject.getName()
            toProject.saveScene(scene, scenePath)

            resources = sceneAndResources["resourcesData"]

            ownerProjectName = sceneAndResources["ownerProjectName"]

            assert isinstance(resources, dict)
            for r in resources.keys():
                if(ownerProjectName == r[0]):
                    tmp = tempfile.NamedTemporaryFile(delete=False)
                    filename = tmp.name
                    tmp.close()

                    with open(filename, "wb") as f:
                        f.write(resources[r])
                    toProject.importData(filename, r[1], r[2])
                    os.remove(filename)
                else:
                    tmp = tempfile.NamedTemporaryFile(delete=False)
                    filename = tmp.name
                    tmp.close()

                    with open(filename, "wb") as f:
                        f.write(resources[r])
                    self.projectManager.importAssetFromFile("", r[1], r[0] + "/" + r[2], filename)
                    os.remove(filename)

            return True
        except Exception as exc:
            print("Cannot unpack scene file '{}'".format(scenePackedFile))
            print(exc)
            return False

    def listScenes(self):
        try:
            absPath = os.path.join(self.rootPath)
            res = []
            for root, dirs, files in os.walk(absPath):
                for file in files:
                    if file.endswith(".scn"):
                        res.append(os.path.join(root, file))

            return res
        except Exception as exc:
            print("""Cannot list all scenes data in path '{}'""".format(self.rootPath))
            print(exc)
            return []

    def listAllExportDesc(self):
        scenes = self.listScenes()

        scenesExpDescs = []

        for sf in scenes:
            s = SceneReader(sf).loadScene()
            relPath = os.path.relpath(sf, self.projectManager.getRootDir())
            scenesExpDescs.append(SceneExportDesc(s, self.project.getName(), relPath))

        return scenesExpDescs