import os

class AssetExportDesc:
    def __init__(self, relativePath):
        self.relativePath   = os.path.normpath(relativePath)

    def __eq__(self, another):
        return ( hasattr(another, 'relativePath') and self.relativePath == another.relativePath )

    def __hash__(self):
        return hash(self.relativePath)

# class TextureAssetExportDesc(AssetExportDesc):
#     def __init__(self, relativePath):
#         self.relativePath   = relativePath
#
#     def __eq__(self, another):
#         return ( hasattr(another, 'relativePath') and self.relativePath == another.relativePath )
#
#     def __hash__(self):
#         return hash(self.relativePath)