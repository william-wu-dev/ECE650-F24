#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest
from graph_generator import get_orientation
from graph_generator import Point

class GetOrientationTest(unittest.TestCase):
    def test_counter_clock_wise(self):
        a = Point(1, 1)
        b = Point(3, 2)
        c = Point(1, 3)
        res = get_orientation(a, b, c)
        self.assertEqual(res, 2)

    def test_clock_wise(self):
        a = Point(6, 2)
        b = Point(7, 1)
        c = Point(5, 1)
        res = get_orientation(a, b, c)
        self.assertEqual(res, 1)

    def test_colinear(self):
        a = Point(0, 0)
        b = Point(1, 1)
        c = Point(5, 5)
        res = get_orientation(a, b, c)
        self.assertEqual(res, 0)


if __name__ == '__main__':
    unittest.main()