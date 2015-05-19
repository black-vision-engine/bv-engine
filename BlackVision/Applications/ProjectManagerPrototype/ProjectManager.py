
class ProjectManager:

    def __init__(self):
        self.projects       = {}
        self.currentProject = None

    def getAssetEntity(self, location):
        assert isinstance(location, str)

    def getProject(self, name):
        assert isinstance(name, str)
        return self.projects[name]

    def listProjects(self):
        return self.projects


