from XML_Class import *

filename = 'storehouse.xml'
flag = False

content = {1: [0, '<storehouse>'],
           2: [4, '<goods category = "fish">'],
           3: [8, '<title>lake fish</title>'],
           4: [8, '<name>jiyu</name>'],
           5: [8, '<amount>18</amount>'],
           6: [8, '<price>8</price>'],
           7: [4, '</goods>'],
           8: [4, '<goods category = "fruit">'],
           9: [8, '<title>wendaishuiguo</title>'],
           10: [8, '<name>mihoutao</name>'],
           11: [8, '<amount>10</amount>'],
           12: [8, '<price>10</price>'],
           13: [4, '</goods>'],
           14: [0, '</storehouse>']
           }

build_xml = BuildNewXML(filename)
try:
    build_xml.openfile()
    for get_item in content.items():
        build_xml.writeXML(get_item[1][0], get_item[1][1])

    flag = True
except:
    print("Write error, exit!")
    sys.exit()
finally:
    if flag:
        build_xml.closeXML()
        print("Write XML successfully!")