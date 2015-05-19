
class PMAssetEntity:

    def __init__(self, assetType, assetLocation):
        self.assetType      = assetType
        self.assetLocation  = assetLocation

    def getAssetLocation(self):
        return self.assetLocation

    @abs
    def getDescriptor(self): # returns bv::AssetDescConstPtr
        pass
