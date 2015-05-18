

class Project:

    def __init__(self):
        self.name           = ""
        self.projectAssets  = []
        self.projectScenes  = {}

    def appensScene(self, scene):
        if scene.getName() not in self.projectScenes:
            self.projectScenes[scene.getName()] = scene

    def __str__(self):
        print("Project {} \n\t".format(self.name))
        print("Scenes: {}".format(self.projectScenes.keys()))
        print("Used assets: {}".format(self.projectAssets))
