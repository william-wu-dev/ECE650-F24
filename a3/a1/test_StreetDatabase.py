#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests
import unittest

from StreetDatabase import StreetDatabase

class StreetDatabaseTest(unittest.TestCase):
    def test_normal(self):
        sdb = StreetDatabase()
        sdb.add('weber street', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])
        sdb.add('king street s', [(4, 2), (4, 8)])
        sdb.add('davenport road', [(1, 4), (5, 8)])
        expected_db = {
            'weber street': [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)],
            'king street s': [(4, 2), (4, 8)],
            'davenport road': [(1, 4), (5, 8)]
        }
        self.assertEqual(sdb._streetDatabase, expected_db)

        sdb.mod('weber street', [(2, 1), (2, 2)])
        expected_db = {
            'weber street': [(2, 1), (2, 2)],
            'king street s': [(4, 2), (4, 8)],
            'davenport road': [(1, 4), (5, 8)]
        }
        self.assertEqual(sdb._streetDatabase, expected_db)

        sdb.rm('king street s')
        expected_db = {
            'weber street': [(2, 1), (2, 2)],
            'davenport road': [(1, 4), (5, 8)]
        }
        self.assertEqual(sdb._streetDatabase, expected_db)

    def test_duplicated_segment_vertex(self):
        sdb = StreetDatabase()
        with self.assertRaises(Exception) as ctx:
            sdb.add('weber street', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8), (5, 6)])

        with self.assertRaises(Exception) as ctx:
            sdb.mod('weber street', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8), (5, 6)])

    def test_mod_rm_not_exist_street(self):
        sdb = StreetDatabase()
        sdb.add('weber street', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])
        sdb.add('king street s', [(4, 2), (4, 8)])
        sdb.add('davenport road', [(1, 4), (5, 8)])

        with self.assertRaises(Exception) as ctx:
            sdb.mod('weber street ', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8), (5, 6)])
        
        with self.assertRaises(Exception) as ctx:
            sdb.rm('weber  street')

    def test_add_street_already_exists(self):
        sdb = StreetDatabase()
        sdb.add('weber street', [(2, -1), (2, 2), (5, 5), (5, 6), (3, 8)])
        sdb.add('king street s', [(4, 2), (4, 8)])
        sdb.add('davenport road', [(1, 4), (5, 8)])
        with self.assertRaises(Exception) as ctx:
            sdb.add('weber street', [(2, -1), (2, 2), (5, 5), (5, 6)])



if __name__ == '__main__':
    unittest.main()
