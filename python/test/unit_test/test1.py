
import unittest

def sum(a,b):
    return a+b

def widget(a):
    print(a)
    return 0

events = []

"""
#Sungsu : to be tested later
def suite(self):
    suite = unittest.TestSuite()
    suite.addTest(LearnTest.('test_myFunc1'))
    suite.addTest(LearnTest.('test_myFunc2'))
    return suite;
"""
#@unittest.skip("skipt testing for the whole class")
class LearnTest(unittest.TestCase):
    @classmethod        # Sungsu : optional for a test class
    def setUpClass(self):
        events.append("1st event")
        print('setupClass...done')

    @classmethod        # Sungsu : optional for a test class
    def tearDownClass(self):
        countTC = self.countTestCases(self) # Sungsu : optional
        print(countTC)
        event = events.pop()
        print(event)
        print('tearDownClass...done')
        
    def setUp(self):    # Sungsu : optional for TCs
        self.widget = widget('setup')

    def tearDown(self): # Sungsu : optional for TCs
        self.widget - widget('tear down')

    @unittest.skip("demonstration of a TC skipped!")    # Sungsu : optional for TCs
   #@unittest.expectedFailure                           # Sungsu : optional for TCs
    def test_myFunc1(self):
        a = 1
        b = 2
        self.assertEqual(a, b)   

    def test_myFunc2(self):
        # Arrange
        a = 1
        b = 2
        
        # Act
        result = sum(a,b)
                
        # Assert
        self.assertEqual(result, a+b, msg="equal!")
       # self.assertNotEqual(result, a+b, msg="NOT equal!")

# Sungsu : without these two lines below, it doesn't work on the command line.
if __name__ == "__main__":
   unittest.main(verbosity=2) # Sungsu : verbosity=1 by default
 #  unittest.main()