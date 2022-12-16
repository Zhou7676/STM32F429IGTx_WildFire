import os
from XML_Class import BuildNewXML


class FishDB(BuildNewXML):
    def __init__(self, filename=None):
        super().__init__(self, filename=None)
        self.path = ""

    def check_path(self):
        try:
            if self.path == ":
                print("Pls set the right path before operation!")
                return
            elif not os.path.isdir(self.path):
                os.makedirs(self.path)
            self.filename = self.path + self.filename
        except:
            print("Failt to create %s folder" % (self.path))
