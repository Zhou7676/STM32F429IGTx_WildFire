from BoxClass1 import FishBox

big_gift_box = FishBox(60, 30, 40)
big_gift_box.price = 1000
big_gift_box.amount = 20
big_gift_box.weight = 10
print("The volume of big gift box is %d" % big_gift_box.volume())
print("The area of big gift box is %d" % big_gift_box.area())
index = big_gift_box.type.index('large box')
g_box_num = big_gift_box.CountBoxNums(200, index)
print("200 fishes need %d big gift box" % g_box_num)
print("The total value of big gift box is %d" % (g_box_num * big_gift_box.price))

small_gift_box = FishBox(50, 20, 30)
small_gift_box.price = 500
small_gift_box.amount = 10
small_gift_box.weight = 5
print("The volume of small gift box is %d" % small_gift_box.volume())
print("The area of small gift box is %d" % small_gift_box.area())
index = small_gift_box.type.index('small box')
g_box_num = small_gift_box.CountBoxNums(200, index)
print("200 fishes need %d small gift box" % g_box_num)
print("The total value of small gift box is %d" %(g_box_num * small_gift_box.price))
