class BuildNewXML():
    def __init__(self, filename=None):
        self.filename = filename
        self.__get_f = None

    def openfile(self):
        if self.filename == None:
            print("filename is not provided! Pls provide filename before creating instance!")
            return False
        try:
            self.__get_f = open(self.filename, 'a', encoding='utf-8')
        except:
            print("Error to open %s" % (self.filename))
            return False

    def writeXML(self, n, element):
        try:
            if n == 0:
                self.__get_f.write(element + '\n')
            else:
                self.__get_f.write('' * n + element + '\n')
        except:
            print("Error to write %s to %s file" % (self.filename, element))

    def closeXML(self):
        if self.__get_f:
            self.__get_f.close()
