
class Location:
    def __init__(self, projectName, categoryName, path):
        self.projectName    = projectName  # If None == All projects
        self.categoryName   = categoryName # If None == All categories
        self.path           = path

    def __repr__(self):
        return "{} : {} : {}".format(self.projectName if self.projectName else "*", self.categoryName if self.categoryName else "*", self.path)

    @staticmethod
    def pathToLocation(path, projectManager):
        assert isinstance(path, str)
        projectsNames   = projectManager.listProjectsNames()
        categoriesNames = projectManager.listCategoriesNames() + ["scenes"]

        categoryName = None

        tmpPath = path

        for cn in categoriesNames:
            if tmpPath.startswith(cn + "\\"):
                categoryName = cn

        if categoryName:
            tmpPath = tmpPath[len(categoryName) + 1:]

        projectName = None

        for pn in projectsNames:
            if tmpPath.startswith(pn + "\\"):
                projectName = pn

        if projectName:
            tmpPath = tmpPath[len(projectName) + 1:]

        return Location(projectName, categoryName, tmpPath)





