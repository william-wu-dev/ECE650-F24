#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest
from line_parser import line_parser

class LineParserTest(unittest.TestCase):
    def test_normal(self):
        line = '''add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'add')
        self.assertEqual(street_name, 'weber street')
        self.assertEqual(coordinates_list, [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])

        line = '''mod "Weber Street" (2,1) (2,2)'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'mod')
        self.assertEqual(street_name, 'weber street')
        self.assertEqual(coordinates_list, [(2, 1), (2, 2)])

        line = '''rm "King Street S"'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'rm')
        self.assertEqual(street_name, 'king street s')
        self.assertEqual(coordinates_list, [])

        line = '''gg'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'gg')
        self.assertEqual(street_name, '')
        self.assertEqual(coordinates_list, [])

    def test_random_space(self):
        line = '''add"Weber Street"           (   2  ,  -1   )(2 ,2   )   (5  , 5)   (   5, 6)(3, 8)     '''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'add')
        self.assertEqual(street_name, 'weber street')
        self.assertEqual(coordinates_list, [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])

        line = '''mod"Weber Street"(2,1)(2,2)'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'mod')
        self.assertEqual(street_name, 'weber street')
        self.assertEqual(coordinates_list, [(2, 1), (2, 2)])

        line = '''rm"King Street S"'''
        command, street_name, coordinates_list = line_parser(line)
        self.assertEqual(command, 'rm')
        self.assertEqual(street_name, 'king street s')
        self.assertEqual(coordinates_list, [])
    
    def test_empty_input(self):
        line = '''add "" (2,-1) (2,2) (5,5) (5,6) (3,8)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''mod "" (2,1) (2,2)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''rm ""'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

    def test_double_quotation(self):
        line = '''add "Weber Street (2,-1) (2,2) (5,5) (5,6) (3,8)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)
    
    def test_missing_argument(self):
        line = '''add (2,-1) (2,2) (5,5) (5,6) (3,8)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''mod "Weber Street"   '''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''rm'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

    def test_extra_argument(self):
        line = '''rm "King Street S" (1, 2) (3, 4)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''gg "King Street S" (1, 2) (3, 4)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

        line = '''gg (1, 2) (3, 4)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)
        
    def test_coordinate_number_less_than_two(self):
        line = '''add "Weber Street" (2,-1)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

    def test_unknown_command(self):
        line = '''adda "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)'''
        with self.assertRaises(Exception) as ctx:
            line_parser(line)

            
if __name__ == '__main__':
    unittest.main()
