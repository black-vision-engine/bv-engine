from AssetCategory import AssetCategory
from Project import Project
from Location import Location
from FSSceneAccessor import FSSceneAccessor, SceneAccessor
from ProjectExportDesc import ProjectExportDesc
from SceneExportDesc import SceneExportDesc
from Scene import SceneWriter


import pickle
import zipfile
import uuid

import os


class ProjectManager:

    def __init__(self, rootDir):
        self.projects       = {}
        self.currentProject = None
        self.globalCategories = {}
        self.globalSceneAccessor = None
        self.rootDir        = os.path.abspath(rootDir)

        self.__initializeProjectManager()

    def __createDir(self):
        if not os.path.exists(self.rootDir):
            os.makedirs(self.rootDir)

    def __setGlobalSceneAccessor(self, sceneAccessor):
        self.globalSceneAccessor = sceneAccessor

    def __initializeProjectManager(self):
        self.__createDir()
        self.__setGlobalSceneAccessor(FSSceneAccessor(self))

        if os.path.exists(self.rootDir):
            self.__initializeProjects()

    def __initializeProjects(self):
        projectsPath = os.path.join(self.rootDir, "projects")
        if os.path.exists(projectsPath):
            for dirpath, dirnames, filenames in os.walk(projectsPath):
                for fn in filenames:
                    if fn == ".bvproj":
                        projName = os.path.normpath(os.path.relpath(dirpath, projectsPath))
                        if projName not in self.projects:
                            self.projects[projName] = Project(self, projName)

    def __isProjectName(self, projectName):
        return projectName in self.projects

    def __toRelativePath(self, projectName, path):
        if not projectName:
            return os.path.normpath(path)

        if projectName:
            if projectName == ".":
                if self.currentProject:
                    return os.path.normpath(os.path.join(self.currentProject.getName(), path))
                else:
                    print("Current project isn't set.")
            else:
                    return os.path.normpath(os.path.join(projectName, path))

        print("Wrong asset access {} : {}", projectName, path)
        return None

    def __getCategoryName(self, path):
        path = os.path.normpath(path)
        for c in self.globalCategories:
            if path.startswith(c + """\\"""):
                return c

        return None

    def getAssetDesc(self, projectName, categoryName, pathInProject):
        assert False  # TODO Rewrite using new Location model
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        loc = Location(projectName, pathInProject, self.currentProject.getName() if self.currentProject else "")

        if loc:
            if loc.getIsGlobalLocation():
                catName = loc.getCategoryName()
                if catName in self.globalCategories:
                    return self.globalCategories[catName].getAssetDesc(loc.getInternalPath())
            else:
                proj = self.getProject(loc.getProjectName())
                if proj:
                    return proj.getAssetDesc(loc.getCategoryName(), loc.getInternalPath())

        print("Cannot find asset '{}  {}'".format(projectName, pathInProject))
        return None

    def getSceneDesc(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        if len(projectName) > 0:
            proj = self.getProject(projectName)
            if not proj:
                print("Cannot find scene '{}  {}'".format(projectName, pathInProject))
                return None

        pathInScenes = self.__toRelativePath(projectName, pathInProject)

        return self.globalSceneAccessor.getSceneDesc(pathInScenes)

    def getProject(self, name):
        assert isinstance(name, str)
        if name in self.projects:
            return self.projects[name]
        else:
            print("Cannot find project '{}'".format(name))
            return None

    def listProjectsNames(self):
        return self.projects.keys()

    def listScenes(self, projectName = ""):
        if len(projectName) > 0:
            if projectName not in self.projects:
                print("Project named {} doesn't exist".format(projectName))
                return None

        return self.globalSceneAccessor.listScenes(projectName)

    def listCategories(self):
        return [c for c in self.globalCategories.keys()]

    def listAssets(self, projectName = None, categoryName = None ):
        if not projectName:
            if not categoryName:
                res = []
                for cn in self.globalCategories.keys():
                    res += [os.path.join(cn, a) for a in
                        self.globalCategories[cn].listAssets()]
                return res
            else:
                if categoryName in self.globalCategories:
                    return [os.path.join(categoryName, a) for a in
                        self.globalCategories[categoryName].listAssets()]
        else:

            if not categoryName:
                res = []
                for cn in self.globalCategories.keys():
                    res += [os.path.join(cn, a) for a in
                        self.globalCategories[cn].listAssets(projectName)]
                return res

            proj = self.getProject(projectName)

            if proj:
                return [os.path.join(categoryName, a) for a in
                        self.globalCategories[categoryName].listAssets(proj.getName())]
            else:
                print("Project named {} doesn't exist".format(projectName))

    def setCurrentProject(self, projectName):
        if projectName in self.projects:
            self.currentProject = self.projects[projectName]
        else:
            print("Cannot set current project. Project '{}' doesn't exist".format(projectName))

    def addNewProject(self, projectName):
        assert isinstance(projectName, str)
        if not projectName in self.projects:
            self.projects[projectName] = Project(self, projectName)
        else:
            print("Project '{}' already exists. Cannot add".format(projectName))

    def registerGlobalCategory(self, category):
        assert isinstance(category, AssetCategory)
        if not category.getId() in self.globalCategories:
            self.globalCategories[category.getId()] = category
        else:
             print("Cannot register global category '{}'. Already registered.".format(category.getId()))

    def getRootDir(self):
        return self.rootDir

    ###########################################################################
    # Operations inside one project manager
    def addAsset(self, projectName, categoryName, path, assetDesc):
        from AssetDesc import AssetDesc
        assert isinstance(assetDesc, AssetDesc)
        pathInCategory = self.__toRelativePath(projectName, path)
        if categoryName in self.globalCategories:
            self.globalCategories[categoryName].addAsset(pathInCategory, assetDesc)
        else:
            print("Category '{}' doesn't exist".format(categoryName))

    def copyAsset(self, inProjectName, inCategoryName, inPath, outProjectName, outPath):
        assetDesc = self.getAssetDesc(inProjectName, inCategoryName, inPath)

        if assetDesc:
            path = self.__toRelativePath(outProjectName, outPath)
            if path:
                self.globalCategories[inCategoryName].addAsset(path, assetDesc)
        else:
            print("Asset {} : {} : {} doesn't exist".format(inProjectName, inCategoryName, inPath))

    def removeAsset(self, projectName, categoryName, path):
        path = self.__toRelativePath(projectName, path)

        if categoryName == None:
            categoryName = self.__getCategoryName(path)
            if categoryName:
                path = path[len(categoryName) + 1:]

        if categoryName in self.globalCategories:
            self.globalCategories[categoryName].removeAsset(path)
        else:
            print("Category {} doesn't exist".format(categoryName))

    def moveAsset(self, inProjectName, inCategoryName, inPath, outProjectName, outPath):
        self.copyAsset(inProjectName, inCategoryName, inPath, outProjectName, outPath)
        self.removeAsset(inProjectName, inCategoryName, inPath)

    def addScene(self, scene, projectName, outPath):
        from Scene import Scene
        assert isinstance(scene, Scene)
        if len(projectName) > 0 and projectName in self.projects:
            return self.globalSceneAccessor.addScene(scene, self.__toRelativePath(projectName, outPath))
        else:
            print("Project named {} doesn't exist".format(projectName))

    def copyScene(self, inProjectName, inPath, outProjectName, outPath):
        path = self.__toRelativePath(inProjectName, inPath)
        if path:
            s = self.globalSceneAccessor.getScene(path)
        else:
            print("Scene {} : {} doesn't exist".format(inProjectName, inPath))
            return

        if s:
            opath = self.__toRelativePath(outProjectName, outPath)
            if opath:
                self.globalSceneAccessor.addScene(s, opath)
        else:
            print("Scene {} : {} doesn't exist".format(inProjectName, inPath))

    def removeScene(self, projectName, path):
        path = self.__toRelativePath(projectName, path)
        if path:
            self.globalSceneAccessor.removeScene(path)
        else:
            print("Scene {} : {} doesn't exist".format(projectName, path))

    def moveScene(self, inProjectName, inPath, outProjectName, outPath):
        self.copyScene(inProjectName, inPath, outProjectName, outPath)
        self.removeScene(inProjectName, inPath)

    def listAllUsedAssets(self):
        scenesFiles = self.globalSceneAccessor.listScenes("")

        scenesAssets = set()

        for sf in scenesFiles:
            s = self.globalSceneAccessor.getScene(sf)
            for a in s.listAssets():
                scenesAssets.add(os.path.normpath(os.path.join(a[1], a[0], a[2])))

        return [a for a in scenesAssets]

    def listAllUnusedAssets(self, projectName, categoryName):
        assets = set(self.listAssets(projectName, categoryName))
        usedAssets = set(self.listAllUsedAssets())
        assets.difference_update(usedAssets)
        return [a for a in assets]

    def removeUnusedAssets(self, projectName = None, categoryName = None):
        unusedAssets = self.listAllUnusedAssets(projectName, categoryName)
        for ua in unusedAssets:
            self.removeAsset(None, None, ua)



    ###########################################################################
    # Exporting and importing
    ###########################################################################

    ###########################################################################
    # Exporting and importing assets (textures, sequences etc.)
    def exportAssetToFile(self, projectName, categoryName, assetPath, outputFile):
        self.getProject(projectName).exportAsset(outputFile, categoryName, assetPath)

    def importAssetFromFile(self, importToProjectName, importToCategoryName, importToPath, importAssetFilePath):
        if importToProjectName == ".":
            importToProjectName == self.currentProject.getName()

        if len(importToProjectName) == 0:
            if importToCategoryName in self.globalCategories:
                self.globalCategories[importToCategoryName].importAsset(importAssetFilePath, importToPath)
        else:
            self.getProject(importToProjectName).importAsset(importAssetFilePath, importToCategoryName, importToPath)

    ###########################################################################
    # Exporting and importing whole scenes
    def exportSceneToFile(self, projectName, scenePath, outputFile):

        assert isinstance(self.globalSceneAccessor, SceneAccessor)

        if len(projectName) == 0:
            assert False  # TODO: Implement global accessor case
        else:
            proj = self.getProject(projectName)
            if proj:
                pathInScenes = self.__toRelativePath(projectName, scenePath)
                self.globalSceneAccessor.exportSceneToFile(pathInScenes, outputFile)
            else:
                print("Cannot export scene '{}' from project '{}'".format(scenePath, projectName))
                return False

    def importSceneFromFile(self, importToProjectName, importToPath, impSceneFilePath):
        self.globalSceneAccessor.importSceneFromFile(impSceneFilePath, self.__toRelativePath(importToProjectName, importToPath))

    ###########################################################################
    # Exporting and importing whole projects
    def exportProjectToFile(self, projectName, outputFilePath):
        proj = self.getProject(projectName)
        if proj:
            assetsDescs = set()
            for c in self.globalCategories.values():
                expDescs = c.accessor.listAllUniqueExportDesc(projectName)
                for ed in expDescs:
                    assetsDescs.add(ed)

            scenesDescs = []
            for sd in self.globalSceneAccessor.listAllExportDesc(projectName):
                scenesDescs.append(sd)

            expDesc = ProjectExportDesc(projectName, self.rootDir, scenesDescs, assetsDescs)
            expDesc.saveExportPackageToFile(outputFilePath)
        else:
            print("Cannot export project '{}'. It doesn't exist.".format(projectName))
            return False

    def importProjectFromFile(self, expFilePath, importToPath=None):

        # {"projectDesc": self, "assetsArchiveData": open(filename, "rb").read()}

        with open(expFilePath, "rb") as f:
            expDescDict = pickle.load(f)

        if isinstance(expDescDict, dict):
            if "projectDesc" in expDescDict:  # project importing

                projDesc = expDescDict["projectDesc"]
                assert isinstance(projDesc, ProjectExportDesc)

                if "assetsArchiveData" in expDescDict:

                    filename = "{}".format(uuid.uuid4())
                    with open(filename, "wb") as f:
                        f.write(expDescDict["assetsArchiveData"])

                    myZipFile = zipfile.ZipFile(filename, "r")

                    if importToPath:
                        for n in myZipFile.namelist():
                            assert isinstance(n, str)
                            with myZipFile.open(n, "r") as f:
                                nn = os.path.normpath(n)
                                outputFileName = os.path.join(self.rootDir, nn.replace("\\" + projDesc.name + "\\", "\\" + importToPath + "\\") )
                                dirName = os.path.dirname(outputFileName)
                                if not os.path.exists(dirName):
                                    os.makedirs(dirName)

                                with open(outputFileName, "wb") as of:
                                    of.write(f.read())
                    else:
                        myZipFile.extractall(path=self.rootDir)
                    myZipFile.close()

                    os.remove(filename)


                for scenesDesc in projDesc.projectScenesDescs:
                    assert isinstance(scenesDesc, SceneExportDesc)
                    if importToPath:
                        scenOutputFileName = os.path.join(self.rootDir, scenesDesc.relativePath.replace("\\" + projDesc.name + "\\", "\\" + importToPath + "\\"))
                        scenesDesc.scene.remapResourcesPaths(projDesc.name, importToPath)
                    else:
                        scenOutputFileName = os.path.join(self.rootDir, scenesDesc.relativePath)

                    SceneWriter(scenesDesc.scene, scenOutputFileName).saveScene()

                if not self.getProject(importToPath):
                    self.addNewProject(importToPath)


#######################################
# project manager instance
PM = ProjectManager("bv_media")