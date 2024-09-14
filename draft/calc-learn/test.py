# A simple unit test example. Replace by your own tests
# https://docs.python.org/3/library/unittest.html


import unittest

from calc import *


class MyTest(unittest.TestCase):

    # New tests go here

    # example tests
    def test_upper(self):
        """Test the upper() function of class string"""
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        """Test isupper() function of class string"""
        self.assertTrue('FOO'.isupper())
        self.assertFalse('foo'.isupper())
        self.assertFalse('foo'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_exception(self):
        with self.assertRaises(Exception):
            raise Exception('Error')


if __name__ == '__main__':
    unittest.main()
