from AssetCategory import AssetCategory
from AssetDesc import AssetDesc
from SceneAccessor import SceneAccessor
from ProjectExportDesc import ProjectExportDesc

from FSSceneAccessor import FSSceneAccessor
from FSTextureAssetAccessor import FSTextureAssetAccessor
from FSSequenceAssetAccessor import FSSequenceAssetAccessor

import os

class Project:

    def __init__(self, projectManager, name):
        self.name = name
        self.projectManager = projectManager
        self.rootDir = os.path.abspath(projectManager.getRootDir())
        self.__initialize()

    def __initialize(self):

        self.__createDir()

        self.projectManager.registerGlobalCategory(AssetCategory("textures", FSTextureAssetAccessor(self.projectManager, os.path.join(self.rootDir, "textures"), ['jpg', 'tga'])))
        self.projectManager.registerGlobalCategory(AssetCategory("sequences", FSSequenceAssetAccessor(self.projectManager, os.path.join(self.rootDir, "sequences"), ['jpg', 'tga'])))

    def __createDir(self):
        if not os.path.exists(os.path.join(self.rootDir, "projects", self.name)):
            projPath = os.path.join(self.rootDir, "projects", self.name)
            os.makedirs(projPath)
            with open(os.path.join(projPath, ".bvproj"), "w"):
                pass

    def getName(self):
        return self.name

    def __str__(self):
        print("Project {} \n\t".format(self.name))

