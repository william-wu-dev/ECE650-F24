#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest
from graph_generator import check_intersect
from graph_generator import Point

class CheckIntersectTest(unittest.TestCase):
    def test_intersect_regularly(self):
        p1 = Point((1, 1))
        q1 = Point((8, 3))
        p2 = Point((5, 0))
        q2 = Point((3, 6))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, 1)

        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((5, 0))
        q2 = Point((3, 2))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, 1)

        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((5, 0))
        q2 = Point((5, 3))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, 1)

    def test_extend_intersect(self):
        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((5, 0))
        q2 = Point((4, 1))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, -1)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, -1)

        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((5, 0))
        q2 = Point((5, 1))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, -1)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, -1)

    def test_parallel(self):
        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((3, 4))
        q2 = Point((1, 3))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, -1)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, -1)

    def test_on_the_same_line(self):
        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((9, 5))
        q2 = Point((7, 4))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, -1)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, -1)

    def test_partially_overlap(self):
        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((9, 5))
        q2 = Point((3, 2))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, 0)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, 0)

    def test_total_overlap(self):
        p1 = Point((1, 1))
        q1 = Point((5, 3))
        p2 = Point((5, 3))
        q2 = Point((3, 2))
        res = check_intersect(p1, q1, p2, q2)
        self.assertEqual(res, 0)

        res = check_intersect(p2, q2, p1, q1)
        self.assertEqual(res, 0)

if __name__ == '__main__':
    unittest.main()