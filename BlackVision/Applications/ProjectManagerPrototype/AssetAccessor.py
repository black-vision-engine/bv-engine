from abc import abstractmethod


class AssetAccessor:
    def __init__(self):
        self.internalPathAliases = {}

    def addAlias(self, src, dst):
        assert isinstance(src, str)
        assert isinstance(dst, str)
        if not src in self.internalPathAliases:
            self.internalPathAliases[src] = dst
        else:
            print("Alias '{}' already exist and points to '{}'".format(src, self.internalPathAliases[src]))

    @abstractmethod
    def getLoadableAssetDesc(self, location):
        pass

    @abstractmethod
    def addAsset(self, internalPath, loadableAssetDesc):
        pass

    @abstractmethod
    def removeAsset(self, internalPath):
        pass

    @abstractmethod
    def renameAsset(self, oldPath, newPath):
        pass

    @abstractmethod
    def importAsset(self, impAssetFile, importToPath):
        pass

    @abstractmethod
    def exportAsset(self, expAssetFilePath, internalPath):
        pass

    @abstractmethod
    def exportAll(self, expAssetFilePath):
        pass

    @abstractmethod
    def listAll(self):
        pass

    @abstractmethod
    def listAllUniqueExportDesc(self, path):
        pass