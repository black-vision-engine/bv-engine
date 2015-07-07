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

        myZipFile = zipfile.ZipFile(filename, "w")

        for ad in assetesDescs:
            path = os.path.join(self.rootDir, ad.relativePath)
            if os.path.isfile(path):
                name = os.path.normpath(ad.relativePath)
                myZipFile.write(path, name, zipfile.ZIP_DEFLATED )
            else:
                files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
                for f in files:
                    name = os.path.normpath(os.path.join(ad.relativePath, f))
                    myZipFile.write(os.path.join(path, f), name, zipfile.ZIP_DEFLATED )

        myZipFile.close()

        with open(outputFileName, "wb") as f:
            pickle.dump({"projectDesc": self, "assetsArchiveData": open(filename, "rb").read()}, f)

        os.remove(filename)

