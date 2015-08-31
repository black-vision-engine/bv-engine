from SceneExportDesc import SceneExportDesc
from AssetExportDesc import AssetExportDesc

import pickle

import zipfile
import os
import uuid

class ProjectExportDesc:
    def __init__(self, name, rootDir, projectScenesDescs, projectAssetsDescs):
        assert isinstance(projectScenesDescs, list)
        assert isinstance(projectAssetsDescs, set)

        self.name               = name
        self.rootDir            = rootDir
        self.projectScenesDescs = projectScenesDescs
        self.projectAssetsDescs = projectAssetsDescs

    def appendProjectSceneDesc(self, sceneDesc):
        assert isinstance(sceneDesc, SceneExportDesc)
        self.projectScenesDesc.append(sceneDesc)

    def appendProjectAssetDesc(self, assetDesc):
        assert isinstance(assetDesc, AssetExportDesc)
        self.projectAssetsDesc.add(assetDesc)

    def saveExportPackageToFile(self, outputFileName):
        assetesDescs = self.projectAssetsDescs.copy()

        for sd in self.projectScenesDescs:
            assetesDescs = assetesDescs.union(sd.sceneAssetsDescs)

        filename = "{}".format(uuid.uuid4())

        AssetExportDesc.packAssetsToFile(self.rootDir, filename, assetesDescs)

        with open(outputFileName, "wb") as f:
            pickle.dump({"projectDesc": self, "assetsArchiveData": open(filename, "rb").read()}, f)

        os.remove(filename)

