

class PMAssetType:

    def __init__(self):
        pass

    @abs
    def getID(self):
        return ""

    @abs
    def getLoader(self):
        return None

    def __key(self):
        return (self.getID())

    def __eq__(x, y):
        return x.__key() == y.__key()

    def __hash__(self):
        return hash(self.__key())

