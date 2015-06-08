import LocationParser


class Location:
    def __init__(self, projectName, pathInProject, currentProjectName = ""):
        self.projectName = projectName
        self.categoryName = ""
        self.internalPath = ""
        self.isGlobalLocation = False
        self.prefix = ""
        self.__parseString(projectName, pathInProject, currentProjectName)

    def __parseString(self, projectName, pathInProject, currentProjectName):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)

        locParser = LocationParser(self, currentProjectName)
        locParser.parseLocationString(projectName, pathInProject)

    def getProjectName(self):
        return self.projectName;

    def getCategoryName(self):
        return self.categoryName

    def getInternalPath(self):
        return self.internalPath

    def getPrefix(self):
        return self.prefix

    def getIsGlobalLocation(self):
        return self.isGlobalLocation


