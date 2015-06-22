from TextureDataAccessor import TextureDataAccessor
from LoadableDataDesc import LoadableDataDesc

import os
import shutil
import pickle

class LoadableTextureDataDesc(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath):
        LoadableDataDesc.__init__(self)
        self.absPath = absPath
        #  TODO: Pewnie jeszcze duzo wiecej memberow w, h, bpp, takie tam

class FSTextureDataAccessor(TextureDataAccessor):
    def __init__(self, rootPath, supportedFileExt):
        TextureDataAccessor.__init__(self)
        self.rootPath = rootPath
        self.supportedFileExt = supportedFileExt
        self.__createDir()

    def getLoadableDataDesc(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        if os.path.exists(absPath):
            return LoadableTextureDataDesc(absPath)
        else:
            None

    def appendData(self, internalPath, loadableDataDesc):
        assert isinstance(internalPath, str)
        assert isinstance(loadableDataDesc, LoadableTextureDataDesc)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            if not os.path.exists(os.path.dirname(absPath)):
                os.makedirs(os.path.dirname(absPath))
            shutil.copyfile(loadableDataDesc.absPath, absPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def removeData(self, internalPath):
        assert isinstance(internalPath, str)
        try:
            os.remove(internalPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def renameData(self, oldPath, newPath):
        try:
            shutil.move(oldPath, newPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def copyData(self, internalPath):
        assert False  # TODO: Implement
        pass

    def importData(self, impDataFile, importToPath):

        try:
            resultFileContent = None

            with open(impDataFile, "rb") as fi:
                resultFileContent = pickle.load(fi)

            desc = resultFileContent["desc"]

            assert isinstance(desc, LoadableTextureDataDesc)
            assert isinstance(desc.absPath, str)

            dirName = os.path.join(self.rootPath, os.path.dirname(importToPath))
            if not os._exists(dirName):
                os.makedirs(dirName)

            toPath = os.path.join(self.rootPath, importToPath)

            with open(toPath, "wb") as f:
                f.write(resultFileContent["resourceData"])

            return True
        except Exception as exc:
            print("Cannot import texture from '{}'".format(impDataFile))
            print(exc)
            return False


    def exportData(self, expDataFilePath, internalPath):
        try:
            absPath = os.path.join(self.rootPath, internalPath)

            desc = self.getLoadableDataDesc(internalPath)

            resultFileContent = {}

            resultFileContent["desc"] = desc

            with open(absPath, "rb") as fi:
                resultFileContent["resourceData"] = fi.read()

            with open(expDataFilePath, "wb") as f:
                pickle.dump(resultFileContent, f)

            return True
        except Exception as exc:
            print("""Cannot export texture '{}'""".format(internalPath))
            print(exc)
            return False


    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)