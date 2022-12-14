# Test for class
class Box1():
    def __init__(self, length1, width1, height1):
        self.length = length1
        self.width = width1
        self.height = height1

    def volume(self):
        return self.length * self.width * self.height


# my_box1 = Box1(10, 11, 12)
# print("The volume of the box1 is %d" % my_box1.volume())
#
# my_box2 = Box1(5, 15, 10)
# print("The volume of the box2 is %d" % my_box2.volume())
#
# my_box3 = Box1(3, 5, 6)
# print("The volume of the box3 is %d" % my_box3.volume())

class Box2(Box1):
    def __init__(self, length1, width1, height1):
        super().__init__(length1, width1, height1)
        self.color = 'white'
        self.material = 'paper'
        self.type = 'fish'

    def area(self):
        re = self.length * self.width + self.width * self.height + self.length * self.height
        return re * 2


# my_box4 = Box2(10, 10, 10)
# print("The volume of the box4 is %d" % my_box4.volume())
# print("The area of the box4 is %d" % my_box4.area())
# print('The color, material and type of the box4 are %s, %s, %s' % (my_box4.color, my_box4.material, my_box4.type))

class TreatPrivate():
    def __init__(self):
        self.say = 'OK'

    def p(self):
        print(self.say)

    def __p1(self):
        print(self.say)


show = TreatPrivate()
show.p()  # Only p() function is shown, p1() function is private
