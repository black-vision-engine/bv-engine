from ProjectManager import ProjectManager
from Project import Project
from Location import Location

# ./            == current project
# /             == global                                   TODO
# file://       == system path to a file                    TODO
# seq://        == system path to directory with a sequence TODO


class LocationParser:
    def __init__(self, location, currentProjectName):
        assert  isinstance(location, Location)
        self.currentProjectName = currentProjectName
        self.location = location

    def parseLocationString(self, string):
        assert isinstance(string, str)
        # if string.startswith("./"):  # a simple path to the current project
        #     if self.currentProject:
        #         string = string[2:]
        #         splitedString = string.split("/")
        #         p = self.pm.getProject(splitedString[ 0 ])
        #         if p:
        #             assert isinstance(p, Project)
        #             if splitedString[1] != "scenes":
        #                 return Location(p, splitedString[1], splitedString[2:])
        #     else:
        #         print("LocationParser: Current project not set")
        #         return None
        # else:
        #     assert False  # TODO: Implement for the prefixes and "global"

