#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest
from line_parser import coordinate_parser

class ParserTest(unittest.TestCase):
    def test_normal(self):  
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) (3,8)'''
        self.assertEqual(coordinate_parser(coordinates_line), [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])
        coordinates_line = '''(4,2) (4,8)'''
        self.assertEqual(coordinate_parser(coordinates_line), [(4, 2), (4, 8)])
        coordinates_line = '''(2,-1)'''
        self.assertEqual(coordinate_parser(coordinates_line), [(2, -1)])

    def test_random_space(self):
        coordinates_line = '''           (   2  ,  -1   )(2 ,2   )   (5  , 5)   (   5, 6)(3, 8)     '''
        self.assertEqual(coordinate_parser(coordinates_line), [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])

    def test_mismatch_parenthesis(self):
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) (3,8'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) 3,8)'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

    def test_nesting_parenthesis(self):
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) ((3,8))'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

    def test_unexpected_char_between_coordinate(self):
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) 5,4 (3,8)'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

    def test_comma_error(self):
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) (3 8)'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) (3,, 8)'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

    def test_nan_error(self):
        coordinates_line = '''(2,-1) (2,2) (5,5) (5,6) (3, 8a)'''
        with self.assertRaises(Exception) as ctx:
            coordinate_parser(coordinates_line)

if __name__ == '__main__':
    unittest.main()
