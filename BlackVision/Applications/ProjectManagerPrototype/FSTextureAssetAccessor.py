from TextureAssetAccessor import TextureAssetAccessor
from AssetDesc import AssetDesc
from AssetExportDesc import AssetExportDesc

import os
import shutil
import pickle

class TextureDesc(AssetDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableAssetDesc
    def __init__(self, absPath):
        AssetDesc.__init__(self)
        self.absPath = absPath
        #  TODO: Pewnie jeszcze duzo wiecej memberow w, h, bpp, takie tam

class FSTextureAssetAccessor(TextureAssetAccessor):
    def __init__(self, rootPath, supportedFileExt):
        TextureAssetAccessor.__init__(self)
        self.rootPath = rootPath
        self.supportedFileExt = supportedFileExt
        self.__createDir()

    def getAssetDesc(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        if os.path.exists(absPath):
            return TextureDesc(absPath)
        else:
            None

    def addAsset(self, internalPath, loadableAssetDesc):
        assert isinstance(internalPath, str)
        assert isinstance(loadableAssetDesc, TextureDesc)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            if not os.path.exists(os.path.dirname(absPath)):
                os.makedirs(os.path.dirname(absPath))
            shutil.copyfile(loadableAssetDesc.absPath, absPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def removeAsset(self, internalPath):
        assert isinstance(internalPath, str)
        try:
            os.remove(os.path.join(self.rootPath, internalPath))
            return True
        except Exception as exc:
            print(exc)
            return False

    def renameAsset(self, oldPath, newPath):
        try:
            shutil.move(oldPath, newPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def importAsset(self, impAssetFile, importToPath):

        try:
            with open(impAssetFile, "rb") as fi:
                resultFileContent = pickle.load(fi)

            desc = resultFileContent["desc"]

            assert isinstance(desc, TextureDesc)
            assert isinstance(desc.absPath, str)

            dirName = os.path.join(self.rootPath, os.path.dirname(importToPath))
            if not os._exists(dirName):
                os.makedirs(dirName)

            toPath = os.path.join(self.rootPath, importToPath)

            with open(toPath, "wb") as f:
                f.write(resultFileContent["resourceAsset"])

            return True
        except Exception as exc:
            print("Cannot import texture from '{}'".format(impAssetFile))
            print(exc)
            return False


    def exportAsset(self, expAssetFilePath, internalPath):
        try:
            absPath = os.path.join(self.rootPath, internalPath)

            desc = self.getLoadableAssetDesc(internalPath)

            resultFileContent = {}

            resultFileContent["desc"] = desc

            with open(absPath, "rb") as fi:
                resultFileContent["resourceAsset"] = fi.read()

            with open(expAssetFilePath, "wb") as f:
                pickle.dump(resultFileContent, f)

            return True
        except Exception as exc:
            print("""Cannot export texture '{}'""".format(internalPath))
            print(exc)
            return False

    def getExportDesc(self, internalPath):
        return AssetExportDesc(internalPath)

    def listAll(self, path):
        try:
            absPath = os.path.join(self.rootPath, path)
            res = []
            for root, dirs, files in os.walk(absPath):
                for file in files:
                    res.append(os.path.relpath(os.path.join(root, file), self.rootPath))

            return res
        except Exception as exc:
            print("""Cannot list all textures Asset in path '{}'""".format(self.rootPath))
            print(exc)
            return []

    def listAllUniqueExportDesc(self, path):
        textures = [os.path.normpath(os.path.join(self.rootPath, t)) for t in self.listAll(path)]
        res = set()

        for t in textures:
            res.add(self.getExportDesc(t))

        #  returns set id TextureAssetExportDesc
        return res

    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)