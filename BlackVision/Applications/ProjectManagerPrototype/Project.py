import PMAssetType
import PMLocation

class Project:

    def __init__(self):
        self.name           = ""
        self.projectAssets  = {}
        self.projectScenes  = {}

    def appendScene(self, scene):
        if scene.getName() not in self.projectScenes:
            self.projectScenes[scene.getName()] = scene

    def appendAsset(self, type, path):
        assert isinstance(type, PMAssetType)
        assert isinstance(path, [str])
        assert False  # TODO: Implement

    def __str__(self):
        print("Project {} \n\t".format(self.name))
        print("Scenes: {}".format(self.projectScenes.keys()))
        print("Used assets: {}".format(self.projectAssets))

    def getAssetEntity(self, location):
        assert isinstance(location, PMLocation)
        assert False  # TODO: Implement

