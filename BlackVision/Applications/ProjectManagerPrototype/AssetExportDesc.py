import os, zipfile

class AssetExportDesc:
    def __init__(self, relativePath):
        self.relativePath   = os.path.normpath(relativePath)

    def __eq__(self, another):
        return ( hasattr(another, 'relativePath') and self.relativePath == another.relativePath )

    def __hash__(self):
        return hash(self.relativePath)

    @staticmethod
    def packAssetsToFile(rootDir, filename, assetsDescs):

        myZipFile = zipfile.ZipFile(filename, "w")

        for ad in assetsDescs:
            path = os.path.join(rootDir, ad.relativePath)
            if os.path.isfile(path):
                name = os.path.normpath(ad.relativePath)
                myZipFile.write(path, name, zipfile.ZIP_DEFLATED )
            else:
                files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
                for f in files:
                    name = os.path.normpath(os.path.join(ad.relativePath, f))
                    myZipFile.write(os.path.join(path, f), name, zipfile.ZIP_DEFLATED )

        myZipFile.close()

# class TextureAssetExportDesc(AssetExportDesc):
#     def __init__(self, relativePath):
#         self.relativePath   = relativePath
#
#     def __eq__(self, another):
#         return ( hasattr(another, 'relativePath') and self.relativePath == another.relativePath )
#
#     def __hash__(self):
#         return hash(self.relativePath)