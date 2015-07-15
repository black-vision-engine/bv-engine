from LocationParser import LocationParser


class Location:
    def __init__(self, projectName, categoryName, internelPath, currentProjectName = ""):
        self.projectName = projectName
        self.categoryName = categoryName
        self.internalPath = internelPath
        self.isGlobalLocation = False
        self.prefix = ""
        self.__parseString(projectName, categoryName, internelPath, currentProjectName)

    def __parseString(self, projectName, categoryName, internelPath, currentProjectName):
        assert isinstance(projectName, str)
        assert isinstance(internelPath, str)

        locParser = LocationParser(self, currentProjectName)
        locParser.parseLocationString(projectName, categoryName, internelPath)

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


