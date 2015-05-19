import PMAssetType

class AssetContainer:
    def __init__(self):
        self.assets = {{}}

    def addAsset(self, type, assetEnt, relativePath):
        assert isinstance(type, PMAssetType)
        assert isinstance(relativePath, [str])

        self.assets[type][relativePath] = assetEnt