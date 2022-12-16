from FishDB_class import FishDB

content1 = {1: [0, '<fish day = "2018-1-1">'],
            2: [4, '<goods>'],
            3: [8, '<name>JiYu</name>'],
            4: [8, '<amount>17</amount>'],
            5: [8, '<price>10.5</price>'],
            6: [4, '</goods>'],
            7: [4, '<goods>'],
            8: [8, '<name>LiYu</name>'],
            9: [8, '<amount>8</amount>'],
            10: [8, '<price>6.2</price>'],
            11: [4, '</goods>'],
            12: [4, '<goods>'],
            13: [8, '<name>LianYu</name>'],
            14: [8, '<amount>7</amount>'],
            15: [8, '<price>4.7</price>'],
            16: [4, '</goods>'],
            17: [0, '</fish>']
            }

content2 = {1: [0, '<fish day = "2018-1-2">'],
            2: [4, '<goods>'],
            3: [8, '<name>JiYu</name>'],
            4: [8, '<amount>17</amount>'],
            5: [8, '<price>10.5</price>'],
            6: [4, '</goods>'],
            7: [4, '<goods>'],
            8: [8, '<name>LiYu</name>'],
            9: [8, '<amount>8</amount>'],
            10: [8, '<price>6.2</price>'],
            11: [4, '</goods>'],
            12: [4, '<goods>'],
            13: [8, '<name>LianYu</name>'],
            14: [8, '<amount>7</amount>'],
            15: [8, '<price>4.7</price>'],
            16: [4, '</goods>'],
            17: [0, '</fish>']
            }

content3 = {1: [0, '<fish day = "2018-1-3">'],
            2: [4, '<goods>'],
            3: [8, '<name>JiYu</name>'],
            4: [8, '<amount>17</amount>'],
            5: [8, '<price>10.5</price>'],
            6: [4, '</goods>'],
            7: [4, '<goods>'],
            8: [8, '<name>LiYu</name>'],
            9: [8, '<amount>8</amount>'],
            10: [8, '<price>6.2</price>'],
            11: [4, '</goods>'],
            12: [4, '<goods>'],
            13: [8, '<name>LianYu</name>'],
            14: [8, '<amount>7</amount>'],
            15: [8, '<price>4.7</price>'],
            16: [4, '</goods>'],
            17: [0, '</fish>']
            }

new_xml = FishDB()
DBRecord = []
DBRecord.append([0, '<DBrecord>'])

