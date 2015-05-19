
class PMLocation:
    def __init__(self, project, firstLevel, path, prefix = ""):
        self.project    = project       # if None get from global
        self.firstLevel = firstLevel    # textures, sequences, scenes itc.
        self.prefix     = prefix        # file:/ or seq:/
        self.path       = path
