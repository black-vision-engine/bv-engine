import TextureDataAccessor
import LoadableDataDesc

import os
import shutil

class LoadableTextureDataDesc(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath):
        super(self)
        self.absPath = absPath
        #  TODO: Pewnie jeszcze duzo wiecej memberow w, h, bpp, takie tam


class FSTextureDataAccessor(TextureDataAccessor):
    def __init__(self, rootPath):
        super(self)
        self.rootPath = rootPath

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
