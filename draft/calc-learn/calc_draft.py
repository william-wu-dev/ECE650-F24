import sys

class Register:
    def __int__(self, val=0):
        self._currentVal = val

    def add(self, num):
        self._currentVal += num

    def sub(self, num):
        self._currentVal -= num
    
    def get(self):
        return self._currentVal

class MyTest(unittest.TestCase):

    def test_register(self):
        r = Register(0)
        self.assertEqual(r.get(), 0)
        r.add(5)
        self.assertEqual(r.get(), 5)

    def test_parser(self):
        line = '+ 3'
        cmd, val = parse_line(line)
        self.assertEqual(cmd, '+')
        self.assertEqual(val, '3')

    def test_parser_assert(self):
        with

def parse_line(line):
    sp = line.strip().split()  # strip all the leading or trailing spaces
    
    if len(sp) > 2:
        raise Exception('Too many arguments')
    elif len(sp) == 2:
        cmd = sp[0]
        val = sp[1]
    elif len(sp) == 1:
        cmd = sp[0]
        val = None
    else:
        raise Exception('Incorrect Command')
    return cmd, val


