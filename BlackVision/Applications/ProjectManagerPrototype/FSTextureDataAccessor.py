import TextureDataAccessor
import LoadableDataDesc

import os

class LoadableTextureDataDesc(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath):
        super(self)
        self.absPath = absPath
        #  TODO: Pewnie jeszcze duzo wiecej memberow w, h, bpp, takie tam


class FSTextureDataAccessor(TextureDataAccessor):
    def __init__(self, rootPath):
        super(self)
        self.rootPath = rootPath

    def getLoadableDataDesc(self, path):
        assert isinstance(path, str)

        absPath = os.path.join(self.rootPath, path)

        if os.path.exists(absPath):
            return LoadableTextureDataDesc(absPath)
        else:
            None

    def appendData(self, internalPath, loadableDataDesc):
        assert isinstance(loadableDataDesc, LoadableDataDesc)
        assert isinstance(internalPath, str)
        # TODO: kopiuj do odpiwedzniega katalogu

    def removeData(self, internalPath):
        assert isinstance(internalPath, str)
        # TODO: usuń z katalogu