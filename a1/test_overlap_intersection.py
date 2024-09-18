#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest
from graph_generator import overlap_intersect
from graph_generator import Point

class OverlapIntersectionTest(unittest.TestCase):
    def test_total_overlap(self):
        p1 = Point(1, 1)
        q1 = Point(5, 3)
        p2 = Point(5, 3)
        q2 = Point(3, 2)
        res = overlap_intersect(p1, q1, p2, q2)
        self.assertIn(p2, res)
        self.assertIn(q2, res)
        self.assertNotIn(p1, res)


        p1 = Point(8, 3)
        q1 = Point(11, 0)
        p2 = Point(10, 1)
        q2 = Point(9, 2)
        res = overlap_intersect(p1, q1, p2, q2)
        self.assertIn(p2, res)
        self.assertIn(q2, res)
        self.assertNotIn(p1, res)
        self.assertNotIn(q1, res)

    def test_partially_overlap(self):
        p1 = Point(1, 1)
        q1 = Point(5, 3)
        p2 = Point(9, 5)
        q2 = Point(3, 2)
        res = overlap_intersect(p1, q1, p2, q2)
        self.assertIn(q1, res)
        self.assertIn(q2, res)
        self.assertNotIn(p1, res)
        self.assertNotIn(p2, res)

    def test_overlap_only_one_point(self):
        p1 = Point(1, 1)
        q1 = Point(5, 3)
        p2 = Point(7, 4)
        q2 = Point(5, 3)
        res = overlap_intersect(p1, q1, p2, q2)
        self.assertIn(q1, res)
        self.assertNotIn(p1, res)
        self.assertNotIn(p2, res)



if __name__ == '__main__':
    unittest.main()