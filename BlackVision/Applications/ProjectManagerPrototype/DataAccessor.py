from abc import abstractmethod


class DataAccessor:
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
    def getLoadableDataDesc(self, location):
        pass

    @abstractmethod
    def appendData(self, internalPath, loadableDataDesc):
        pass

    @abstractmethod
    def removeData(self, internalPath):
        pass

    @abstractmethod
    def renameData(self, oldPath, newPath):
        pass

    @abstractmethod
    def importData(self, impDataFile, importToPath):
        pass

    @abstractmethod
    def exportData(self, expDataFilePath, internalPath):
        pass

