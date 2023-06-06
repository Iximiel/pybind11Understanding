import unittest
import callEmbed


class TestFoolishCalls(unittest.TestCase):
    def test_callFoolishSUM(self):
        for a, b in [(2, 3), (3, 2), (100, 255), (1039, -558), (0, -1)]:
            self.assertEqual(callEmbed.foolishCallADD(a, b), (a + b))


if __name__ == "__main__":
    unittest.main()
