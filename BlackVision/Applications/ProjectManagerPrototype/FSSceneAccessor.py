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
    def __init__(self, projectManager):
        SceneAccessor.__init__(self, projectManager)
        self.projectManager = projectManager
        self.rootPath = os.path.join(projectManager.getRootDir(), "scenes")
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

    def addScene(self, scene, outPath):
        absPath = os.path.join(self.rootPath, outPath)
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

    def removeScene(self, path):
        absPath = os.path.join(self.rootPath, path)
        if os.path.exists(absPath) and os.path.isfile(absPath) and absPath.endswith(".scn"):
            os.remove(absPath)
        else:
            print("Cannot remove scene {}".format(path))

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

        return SceneExportDesc(s, internalPath)

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

    def listScenes(self, path):
        try:
            res = []
            for root, dirs, files in os.walk(self.rootPath, path):
                for file in files:
                    if file.endswith(".scn"):
                        absPath = os.path.join(root, file)
                        res.append(os.path.relpath(absPath, self.rootPath))

            return res
        except Exception as exc:
            print("""Cannot list all scenes data in path '{}'""".format(self.rootPath))
            print(exc)
            return []

    def listAllExportDesc(self, path):
        scenes = self.listScenes(path)

        scenesExpDescs = []

        for sf in scenes:
            absPath = os.path.join(self.rootPath, sf)
            s = SceneReader(absPath).loadScene()
            relPath = os.path.relpath(absPath, self.projectManager.getRootDir())
            scenesExpDescs.append(SceneExportDesc(s, relPath))

        return scenesExpDescs

    def getScene(self, path):
        return SceneReader(os.path.join(self.rootPath, path)).loadScene()