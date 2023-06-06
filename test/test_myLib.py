import unittest
import mypylib


class TestMypylib(unittest.TestCase):
    def test_Constructor(self):
        temp = mypylib.SimpleIO()
        self.assertEqual(temp.getI(), 0)

    def test_GetSet(self):
        temp = mypylib.SimpleIO()

        for setNum in [1, 56, 33, 99, 869, -597]:
            temp.setI(setNum)
            self.assertEqual(temp.getI(), setNum)


if __name__ == "__main__":
    unittest.main()
