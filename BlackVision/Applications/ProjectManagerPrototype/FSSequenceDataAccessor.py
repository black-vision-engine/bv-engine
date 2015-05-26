from SequenceDataAccessor import SequenceDataAccessor
from LoadableDataDesc import LoadableDataDesc

import os
import shutil

class LoadableSequenceDataDesc(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::SequenceAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath, frames):
        LoadableDataDesc.__init__(self)
        self.absPath = absPath
        self.frames = frames

    def getFramesNum(self):
        return len(self.frames)

    def getFrames(self):
        return self.frames

class FSSequenceDataAccessor(SequenceDataAccessor):
    def __init__(self, rootPath):
        SequenceDataAccessor.__init__(self)
        self.rootPath = rootPath
        self.__createDir()

    def getLoadableDataDesc(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        frames = [ name for name in os.listdir(absPath) if os.path.isfile(os.path.join(absPath, name)) ]

        if os.path.exists(absPath):
            return LoadableSequenceDataDesc(absPath, frames)
        else:
            None

    def appendData(self, internalPath, loadableDataDesc):
        assert isinstance(internalPath, str)
        assert isinstance(loadableDataDesc, LoadableSequenceDataDesc)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            for f in loadableDataDesc.getFrames():
                shutil.copyfile(f, os.path.join(absPath, os.path.basename(f)))
            return True
        except Exception as exc:
            print(exc)
            return False

    def removeData(self, internalPath):
        assert isinstance(internalPath, str)

        absPath = os.path.join(self.rootPath, internalPath)

        try:
            os.remove(absPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def renameData(self, oldPath, newPath):

        oldAbsPath = os.path.join(self.rootPath, oldPath)
        newAbsPath = os.path.join(self.rootPath, newPath)

        try:
            shutil.move(oldAbsPath, newAbsPath)
            return True
        except Exception as exc:
            print(exc)
            return False

    def __createDir(self):
        if not os.path.exists(self.rootPath):
            os.makedirs(self.rootPath)