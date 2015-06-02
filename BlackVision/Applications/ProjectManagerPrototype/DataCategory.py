from DataAccessor import DataAccessor
import LoadableDataDesc

class DataCategory:
    def __init__(self, id, dataAccessor):
        assert isinstance(dataAccessor, DataAccessor)
        self.id = id
        self.accessor = dataAccessor

    def getId(self):
        return self.id

    def appendData(self, path, loadableData):
        assert isinstance(loadableData, LoadableDataDesc)
        assert False # TODO: Implement

    def getData(self, path):
        assert isinstance(path, str)
        return self.accessor.getLoadableDataDesc(path)

    def copyData(self, path):
        assert isinstance(path, str)
        return self.accessor.copyData(path)

    def exportData(self, expDataFilePath, path):
        assert isinstance(path, str)
        return self.accessor.exportData(expDataFilePath, path)

    def importData(self, impDataFilePath, toPath):
        assert isinstance(toPath, str)
        return self.accessor.importData(impDataFilePath, toPath)