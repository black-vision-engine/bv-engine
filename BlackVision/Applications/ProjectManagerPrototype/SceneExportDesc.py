from AssetExportDesc import AssetExportDesc

class SceneExportDesc:
    def __init__(self, scene, relativePath):
        self.scene              = scene
        self.relativePath       = relativePath
        self.sceneAssetsDescs   = set()
        self.__initializeAssetsDescs()

    def __initializeAssetsDescs(self):
        sceneAssets = self.scene.listAssets()
        for sa in sceneAssets:
            aed = AssetExportDesc(sa[1] + "/" + sa[0] + "/" + sa[2])
            self.sceneAssetsDescs.add(aed)
