from FontDataAccessor import FontDataAccessor
from LoadableDataDesc import LoadableDataDesc

import os
import shutil
import json

class LoadableFontDataDesc(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath):
        LoadableDataDesc.__init__(self)
        self.absPath = absPath
        #  TODO: Pewnie jeszcze duzo wiecej memberow w, h, bpp, takie tam


class FSTextureDataAccessor(FontDataAccessor):
    def __init__(self, rootPath):
        FontDataAccessor.__init__(self)
        self.rootPath = rootPath
        self.__createDir()

    def getLoadableDataDesc(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        if os.path.exists(absPath):
            return LoadableFontDataDesc(absPath)
        else:
            None

    def appendData(self, internalPath, loadableDataDesc):
        assert isinstance(internalPath, str)
        assert isinstance(loadableDataDesc, LoadableFontDataDesc)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
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

    def importData(self, impDataFile, importToPath):

        try:
            resultFileContent = None

            with open(impDataFile, "r") as fi:
                resultFileContent = json.load(fi)

            desc = resultFileContent["desc"]

            assert isinstance(desc, LoadableFontDataDesc)
            assert(desc.absPath. str)
            filename = desc.absPath.split('/')[-1]

            toPath = os.path.join(self.rootPath, importToPath, filename)

            with open(toPath, "w") as f:
                f.write(resultFileContent["resourceData"])

            return True
        except Exception as exc:
            print("Cannot import font from '{}'".format(impDataFile))
            print(exc)
            return False


    def exportData(self, expDataFilePath, internalPath):
        try:
            absPath = os.path.join(self.rootPath, internalPath)

            desc = self.getLoadableDataDesc(internalPath)

            resultFileContent = {}

            resultFileContent["desc"] = json.dumps(desc)

            with open(absPath, "r") as fi:
                resultFileContent["resourceData"] = fi.read()

            with open(expDataFilePath, "w") as f:
                json.dump(resultFileContent, f)

            return True
        except Exception as exc:
            print("""Cannot export font '{}'""".format(internalPath))
            print(exc)
            return False

    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)