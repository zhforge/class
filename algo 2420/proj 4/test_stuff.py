import pytest
from slist import SList
from course import Course

"""
slist tests
- SList objects always stored in ascending order
- Perform operations on any* data type
    - .insert(item)     # automatically inserts item in the appropriate sorted location
    - .remove(key)      # removes first occurence of 'key', RETURNS TRUE if found, false if not
    - .remove_all(key)  # removes ALL occurences of 'key', no return value
    - .size()           # returns the number of items/nodes
    - .find(key)        # returns... uh... "If found, the item is returned" the value?? object?? index??
    - .__str__()        # same format as python's lists, recursive to allow nested lists
- SList objects are iterable
- Able to access items by index with square brackets: my_slist[3]

- should probably add random inputs to the tests

course tests
- constructor accepts (self, number, name, credit_hour, grade) all with DEFAULTS, 
validate types and values or RAISE ValueError with a "meaningful error message"
    - .number()     # returns course number as INTEGER
    - .name()       # returns course name as STRING
    - .credit_hr()  # returns number of credit hours as a FLOAT
    - .grade()      # returns grade as a NUMERIC VALUE in range 4.0 - 0.0
    - .__str__()    # format: "cs<course_number> <course_name> Grade: <grade> Credit Hours: <hours>"
- course overloads magic comparison methods to compare COURSE NUMBERS
    - eq, lt, le AT MINIMUM, but might as well include gt, ge, ne
    - each returns TRUE as appropriate, false otherwise
"""

class TestSlist:
    def test_insert(self):
        self.slyst = SList()
        self.slyst.insert(0)

    def test_str(self):
        self.slyst = SList()
        assert str(self.slyst) == "[]"
        self.slyst.insert(1)
        assert str(self.slyst) == "[1]"
        self.slyst.insert(2)
        assert str(self.slyst) == "[1, 2]"

    def test_ascending(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        assert str(self.slyst) == "[2, 5, 7]"

    def test_remove(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        t = self.slyst.remove(2)
        f = self.slyst.remove(0)
        assert t == True
        assert f == False
        assert str(self.slyst) == "[5, 7]"
    
    def test_remove_all(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        self.slyst.insert(2)
        self.slyst.remove_all(2)
        assert str(self.slyst) == "[5, 7]"

    def test_size(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        assert self.slyst.size() == 3
    
    def test_find(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        assert self.slyst.find(5) == 5 # really?
        assert self.slyst.find(0) == None

    def test_iterate(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        test = [2, 5, 7]
        iter = 0
        for i in self.slyst:
            assert i.value == test[iter]
            iter += 1

    def test_brackets(self):
        self.slyst = SList()
        self.slyst.insert(5)
        self.slyst.insert(2)
        self.slyst.insert(7)
        assert self.slyst[0] == 2
        assert self.slyst[1] == 5
        assert self.slyst[2] == 7


class TestCourse:
    def test_invalid_params(self):
        # Course(number: int, name: str, credit_hr: float, grade: [numeric value in range 4.0 - 0.0])
        with pytest.raises(ValueError):
            a = Course('h')
        with pytest.raises(ValueError):
            a = Course(1, 2)
        with pytest.raises(ValueError):
            a = Course(1, 'h', 'b')
        with pytest.raises(ValueError):
            a = Course(1, 'h', 3.5, 5)
        
    def test_valid_params(self):
        a = Course(1, 'h', 3.5, 3)
        assert a.number() == 1
        assert a.name() == 'h'
        assert a.credit_hr() == 3.5
        assert a.grade() == 3.0

    def test_str(self):
        a = Course(1, 'h', 3.5, 3)
        assert str(a) == "cs1 h Grade: 3.0 Credit Hours: 3.5"

    def test_compare(self):
        a = Course(1, 'h', 3.5, 3)
        b = Course(2, 'h', 3.5, 3)
        c = Course(1, 'h', 3.5, 3)

        assert not a == b
        assert a == c

        assert a != b
        assert not a != c

        assert a < b
        assert not b < a
        assert not a < c

        assert not a > b
        assert b > a
        assert not a > c

        assert a <= b
        assert not b <= a
        assert a <= c

        assert not a >= b
        assert b >= a
        assert a >= c