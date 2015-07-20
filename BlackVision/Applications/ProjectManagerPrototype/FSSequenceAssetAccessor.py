from SequenceAssetAccessor import SequenceAssetAccessor
from AssetDesc import AssetDesc
from AssetExportDesc import AssetExportDesc

import os
import shutil
import pickle

class SequenceDesc(AssetDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::SequenceAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableAssetDesc
    def __init__(self, absPath, frames):
        AssetDesc.__init__(self)
        self.absPath = absPath
        self.frames = frames

    def getFramesNum(self):
        return len(self.frames)

    def getFrames(self):
        return [os.path.join(self.absPath, f) for f in self.frames]

class FSSequenceAssetAccessor(SequenceAssetAccessor):
    def __init__(self, rootPath, supportedFileExt):
        SequenceAssetAccessor.__init__(self)
        self.rootPath = rootPath
        self.supportedFileExt = supportedFileExt
        self.__createDir()

    def getLoadableAssetDesc(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        frames = [ name for name in os.listdir(absPath) if os.path.isfile(os.path.join(absPath, name)) ]

        if os.path.exists(absPath):
            return SequenceDesc(absPath, frames)
        else:
            None

    def appendAsset(self, internalPath, sequenceDesc):
        assert isinstance(internalPath, str)
        assert isinstance(sequenceDesc, SequenceDesc)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            if not os.path.exists(absPath):
                os.makedirs(absPath)
            for f in sequenceDesc.getFrames():
                shutil.copyfile(f, os.path.join(absPath, os.path.basename(f)))
            return True
        except Exception as exc:
            print(exc)
            return False

    def removeAsset(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            os.remove(absPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def renameAsset(self, oldPath, newPath):

        oldAbsPath = os.path.join(self.rootPath, oldPath)
        newAbsPath = os.path.join(self.rootPath, newPath)

        try:
            shutil.move(oldAbsPath, newAbsPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def importAsset(self, impAssetFile, importToPath):

        try:

            with open(impAssetFile, "rb") as fi:
                resultFileContent = pickle.load(fi)

            desc = resultFileContent["desc"]

            assert isinstance(desc, SequenceDesc)
            assert isinstance(desc.absPath, str)

            dirName = os.path.join(self.rootPath, importToPath)

            if not os.path.exists(dirName):
                os.makedirs(dirName)

            for i , frame in enumerate(desc.getFrames()):
                assert isinstance(frame, str)
                filename = os.path.basename(frame)
                absPath = os.path.join(dirName, filename)
                with open(absPath, "wb") as f:
                    f.write(resultFileContent["resourceAsset"][i])

            return True
        except Exception as exc:
            print("Cannot import sequence from '{}'".format(impAssetFile))
            print(exc)
            return False


    def exportAsset(self, expAssetFilePath, internalPath):
        try:
            desc = self.getLoadableAssetDesc(internalPath)

            resultFileContent = {}

            resultFileContent["desc"] = desc

            resultFileContent["resourceAsset"] = []
            for frame in desc.getFrames():
                with open(frame, "rb") as fi:
                    resultFileContent["resourceAsset"].append(fi.read())

            with open(expAssetFilePath, "wb") as f:
                pickle.dump(resultFileContent, f)

            return True
        except Exception as exc:
            print("""Cannot export sequence '{}'""".format(internalPath))
            print(exc)
            return False

    def getExportDesc(self, internalPath):
        return AssetExportDesc(internalPath)

    def listAll(self):
        try:
            absPath = os.path.join(self.rootPath)
            res = []
            for root, dirs, files in os.walk(absPath):
                if len(files) > 0:  # TODO: Add better checking if it's a sequence.
                    res.append(root)

            return res
        except Exception as exc:
            print("""Cannot list all textures Asset in path '{}'""".format(self.rootPath))
            print(exc)
            return []

    def listAllUniqueExportDesc(self, relativeTo):
        sequences = self.listAll()
        res = set()

        for t in sequences:
            res.add(self.getExportDesc(os.path.relpath(t, relativeTo)))

        return res

    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)