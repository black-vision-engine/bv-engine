import TextureDataAccessor
import LoadableDataDesc

import os

class LoadableTextureData(LoadableDataDesc): # Cos tu z nazwa mogloby byc lepiej. To chyba będzie to samo co bv::TextureAssetDesc, które podziedziczymo po czymś co nazwiemy LoadableDataDesc
    def __init__(self, absPath):
        super(self)
        self.absPath = absPath


class FSTextureDataAccessor(TextureDataAccessor):
    def __init__(self, rootPath):
        super(self)
        self.rootPath = rootPath

    def GetLoadableData(self, path):
        assert isinstance(path, str)

        absPath = os.path.join(self.rootPath, path)

        if os.path.exists(absPath):
            return LoadableTextureData(absPath)
        else:
            None