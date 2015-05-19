from AssetAccessor import AssetAccessor

import glob, os

class FSAssetAccessor(AssetAccessor): # This class implements accessing to the assets stored in FS
    def __init__(self, rootPath):
        self.rootPath = rootPath

    def __listFiles(self, dir):
        return [ name for name in os.listdir(self.rootPath) if os.path.isfile(os.path.join(self.rootPath, name)) ]

    def __listDirs(self, dir):
        return [ name for name in os.listdir(self.rootPath) if os.path.isdir(os.path.join(self.rootPath, name)) ]

    def listEntries(self, path = []):
        assert isinstance(path, [str])
