from AssetAccessor import AssetAccessor
from AssetDesc import AssetDesc

class AssetCategory:
    def __init__(self, id, assetAccessor):
        assert isinstance(assetAccessor, AssetAccessor)
        self.id = id
        self.accessor = assetAccessor

    def getId(self):
        return self.id

    def appendAsset(self, path, assetDesc):
        assert isinstance(assetDesc, AssetDesc)
        self.accessor.appendAsset(path, assetDesc)

    def getAssetDesc(self, path):
        assert isinstance(path, str)
        return self.accessor.getLoadableAssetDesc(path)

    def removeAsset(self, path):
        assert isinstance(path, str)
        return self.accessor.removeAsset(path)

    def exportAsset(self, expAssetFilePath, path):
        assert isinstance(path, str)
        return self.accessor.exportAsset(expAssetFilePath, path)

    def importAsset(self, impAssetFilePath, toPath):
        assert isinstance(toPath, str)
        return self.accessor.importAsset(impAssetFilePath, toPath)

    def exportAll(self, expAssetFilePath):
        return self.accessor.exportAll(expAssetFilePath)

    def listAssets(self):
        return self.accessor.listAll()
