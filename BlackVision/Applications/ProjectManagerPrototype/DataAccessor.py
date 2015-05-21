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

    @abs
    def getLoadableDataDesc(self, location):
        pass

    @abs
    def appendData(self, internalPath, loadableDataDesc):
        pass

    @abs
    def removeData(self, internalPath):
        pass

    @abs
    def renameData(self, oldPath, newPath):
        pass
