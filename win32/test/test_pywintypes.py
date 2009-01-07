import unittest
import pywintypes
import time
from pywin32_testutil import str2bytes, ob2memory

class TestCase(unittest.TestCase):
    def testPyTimeFormat(self):
        struct_current = time.localtime()
        pytime_current = pywintypes.Time(struct_current)
        # try and test all the standard parts of the format
        format_string = "%a %A %b %B %c %d %H %I %j %m %M %p %S %U %w %W %x %X %y %Y %Z"
        self.assertEquals(pytime_current.Format(format_string), time.strftime(format_string, struct_current))

    def testPyTimePrint(self):
        # This used to crash with an invalid, or too early time.
        # We don't really want to check that it does cause a ValueError
        # (as hopefully this wont be true forever).  So either working, or 
        # ValueError is OK.
        t = pywintypes.Time(-2)
        try:
            t.Format()
        except ValueError:
            return

    def testPyTimeCompare(self):
        t1 = pywintypes.Time(100)
        t1_2 = pywintypes.Time(100)
        t2 = pywintypes.Time(101)

        self.failUnlessEqual(t1, t1_2)
        self.failUnless(t1 <= t1_2)
        self.failUnless(t1_2 >= t1)

        self.failIfEqual(t1, t2)
        self.failUnless(t1 < t2)
        self.failUnless(t2 > t1 )

    def testGUID(self):
        s = "{00020400-0000-0000-C000-000000000046}"
        iid = pywintypes.IID(s)
        iid2 = pywintypes.IID(ob2memory(iid), True)
        self.assertEquals(iid, iid2)
        self.assertRaises(ValueError, pywintypes.IID, str2bytes('00'), True) # too short
        self.assertRaises(TypeError, pywintypes.IID, 0, True) # no buffer

if __name__ == '__main__':
    unittest.main()

