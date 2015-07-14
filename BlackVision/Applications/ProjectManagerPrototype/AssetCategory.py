from AssetAccessor import AssetAccessor
from AssetDesc import AssetDesc

class AssetCategory:
    def __init__(self, id, dataAccessor):
        assert isinstance(dataAccessor, AssetAccessor)
        self.id = id
        self.accessor = dataAccessor

    def getId(self):
        return self.id

    def appendData(self, path, assetDesc):
        assert isinstance(assetDesc, AssetDesc)
        self.accessor.appendData(path, assetDesc)

    def getData(self, path):
        assert isinstance(path, str)
        return self.accessor.getLoadableDataDesc(path)

    def copyData(self, path):
        assert isinstance(path, str)
        return self.accessor.copyData(path)

    def exportData(self, expDataFilePath, path):
        assert isinstance(path, str)
        return self.accessor.exportData(expDataFilePath, path)

    def importData(self, impDataFilePath, toPath):
        assert isinstance(toPath, str)
        return self.accessor.importData(impDataFilePath, toPath)

    def exportAll(self, expDataFilePath):
        return self.accessor.exportAll(expDataFilePath)
