from BoxClass1 import FishBox
import os


def save_file(file_name, L_newRecord):
    flag = False
    if type(L_newRecord) != list:
        print("The type of context must be list!")
        return flag

    cur_path = os.path.abspath(os.path.curdir)
    cur_path = cur_path + '\\' + file_name

    try:
        f1 = open(cur_path, 'w')
        f1.writelines(L_newRecord)
        flag = True
    except:
        flag = False
    finally:
        if flag:
            f1.close()

    return flag


L_SaveData = []

big_gift_box = FishBox(60, 30, 40)
big_gift_box.price = 1000
big_gift_box.amount = 20
big_gift_box.weight = 10
L_SaveData.append("The volume of big gift box is " + str(big_gift_box.volume()))
L_SaveData.append("The area of big gift box is" + str(big_gift_box.area()))
# print("The volume of big gift box is %d" % big_gift_box.volume())
# print("The area of big gift box is %d" % big_gift_box.area())
index = big_gift_box.type.index('large box')
g_box_num = big_gift_box.CountBoxNums(200, index)
L_SaveData.append("200 fishes need " + str(g_box_num) + "big gift box")
L_SaveData.append("The total value of big gift box is " + str(g_box_num * big_gift_box.price))
# print("200 fishes need %d big gift box" % g_box_num)
# print("The total value of big gift box is %d" % (g_box_num * big_gift_box.price))

small_gift_box = FishBox(50, 20, 30)
small_gift_box.price = 500
small_gift_box.amount = 10
small_gift_box.weight = 5
L_SaveData.append("The volume of small gift box is " + str(small_gift_box.volume()))
L_SaveData.append("The area of small gift box is " + str(small_gift_box.area()))
# print("The volume of small gift box is %d" % small_gift_box.volume())
# print("The area of small gift box is %d" % small_gift_box.area())
index = small_gift_box.type.index('small box')
g_box_num = small_gift_box.CountBoxNums(200, index)
L_SaveData.append("200 fishes need " + str(g_box_num) + "small gift box")
L_SaveData.append("The total value of small gift box is " + str(g_box_num * small_gift_box.price))
# print("200 fishes need %d small gift box" % g_box_num)
# print("The total value of small gift box is %d" % (g_box_num * small_gift_box.price))

if save_file('fish_records.txt', L_SaveData):
    print("The data has been saved successfully!")
else:
    print("The operation of data saving fails!!")
