import json
import sys


def SaveToJSON(filename, dicObject):
    flag = False

    if type(dicObject) != dict:
        return flag

    try:
        j_file = open(filename, 'w')
        json.dump(dicObject, j_file, ensure_ascii=False)
        flag = True
    except:
        print("Write error to %s" % (filename))
    finally:
        if flag:
            j_file.close()

    return flag


def GetFromJSON(filename):
    flag = False
    dicObject = {}

    try:
        j_file = open(filename, 'r')
        dicObject = json.load(j_file)
        flag = True
    except:
        print("Read error from %s" % (filename))
    finally:
        if flag:
            j_file.close()

    return dicObject


d_student = {'name': 'DingDing', 'age': 12, 'birthday': '2006_12_25'}
filename = 'student.json'
f_OK = SaveToJSON(filename, d_student)

if f_OK:
    print("student information has been saved successfully!")
else:
    sys.exit()

d_get_s = GetFromJSON(filename)
if d_get_s:
    print(d_get_s)
