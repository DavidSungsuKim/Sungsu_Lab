
import unittest

def sum(a,b):
    return a+b

class LearnTest(unittest.TestCase):

    def test_myFunc1(self):
        pass

    def test_myFunc2(self):
        # arrange
        a = 1
        b = 2
        
        # act
        result = sum(a,b)
        
        # assert
        self.assertEqual(result, a+b, msg="equal!")
       # self.assertNotEqual(result, a+b, msg="NOT equal!")

if __name__ == "__main__":
    unittest.main()