# class Location:
#     def __init__(self, projectName, path):
#         self.projectName    = projectName
#         self.path           = path
#
#     def __getProjectName(self, projectName, projectManager):
#         if projectName == ".":
#             if projectManager.currentProject:
#                 return projectManager.currentProject.getName()
#         else:
#             return None
#
#
# class SceneLocation:
#     def __init__(self, projectName, path):
#         self.projectName    = projectName
#         self.path           = path
#
#     @staticmethod
#     def pathToLocation(cls, path, projectManager):
#         projectsNames   = projectManager.listProjectsNames()
#         categoriesNames = projectManager.listCategoriesNames()


class Location:
    def __init__(self, projectName, categoryName, path):
        self.projectName    = projectName
        self.categoryName   = categoryName
        self.path           = path

    @staticmethod
    def pathToLocation(cls, path, projectManager):
        assert isinstance(path, str)
        projectsNames   = projectManager.listProjectsNames()
        categoriesNames = projectManager.listCategoriesNames()

        categoryName = None

        tmpPath = path

        for cn in categoriesNames:
            path.startswith(cn)
            categoryName = cn

        if categoryName:
            tmpPath = tmpPath[len(categoryName) + 1:]

        projectName = None

        for pn in projectsNames:
            tmpPath.startswith(pn)
            projectName = pn

        if projectName:
            tmpPath = tmpPath[len(projectName) + 1:]

        return Location(projectName, categoryName, tmpPath)





