from Class_module import *

my_box2 = Box2(10, 10, 10)
print("The volume of my_box2 is %d" % my_box2.volume())
print("The area of my_box2 is %d" % my_box2.area())
print("The color, material and type of my_box2 are %s, %s, %s" % (my_box2.color, my_box2.material, my_box2.type))

print(StaticClass.name)
StaticClass.name = 'Jerry'
print(StaticClass.name)

print(StaticClass.a())