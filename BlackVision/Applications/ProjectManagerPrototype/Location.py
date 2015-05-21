import LocationParser


class Location:
    def __init__(self, path = "", currentProjectName = ""):
        self.projectName = ""
        self.categoryName = ""
        self.internalPath = ""
        self.isGlobalLocation = False
        self.prefix = ""
        self.__parseString(path, currentProjectName)

    def __parseString(self, path, currentProjectName):
        assert isinstance(path, str)

        locParser = LocationParser(self, currentProjectName)
        locParser.parseLocationString(path)

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


