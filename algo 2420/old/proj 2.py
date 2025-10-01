from random import seed, sample
from math import sqrt, floor

def make_data(data_size):#DO NOT REMOVE OR MODIFY THIS FUNCTION
    '''A generator for producing data_size random values
    '''
    seed(0)
    data = sample(range(data_size * 3), k=data_size)
    data.sort()
    while True:
        yield data

def linear_search(lyst, target):
    """
    search each element of a list in order
    """
    comparisons = 0

    for item in lyst:
        comparisons += 1
        if item == target:
            return (True, comparisons)
    return (False, comparisons)

def binary_search(lyst, target, _comparisons=0):
    """
    check the middle of a sorted list - if target is lower, check the left half,
    if target is higher, check the right half. repeat until you find the target
    """
    if len(lyst) == 0: return (False, _comparisons)

    while True:
        midpoint = len(lyst) // 2
        _comparisons += 1
        if lyst[midpoint] == target:
            return (True, _comparisons)
        if len(lyst) == 1:
            return (False, _comparisons)

        if target < lyst[midpoint]:
            return binary_search(lyst[:midpoint], target, _comparisons)
        if target > lyst[midpoint]:
            return binary_search(lyst[midpoint:], target, _comparisons)
            
def jump_search(lyst, target):
    """"
    jump across a sorted list in intervals until you pass the target,
    then linear search across the previous interval
    """
    comparisons = 0
    m = floor(sqrt(len(lyst))) # jump interval size
    index = 0
    last_index = 0
    
    if len(lyst) == 0: return (False, comparisons)

    while lyst[index] <= target:
        last_index = index
        index += m
        if index >= len(lyst):
            index = None
            break

    for item in lyst[last_index:index]:
        comparisons += 1
        if item == target:
            return (True, comparisons)
        
    return (False, comparisons)


def main():
    class Tests:
        def test_linear(self, lyst, target):
            (b, c) = linear_search(lyst, target)
            if b == False:
                return f"linear test failed after {c} comparison(s)"
            elif b == True:
                return f"linear test succeeded after {c} comparison(s)"
        
        def test_linear_all(self, lyst):
            for item in lyst:
                (b, c) = linear_search(lyst, item)
                if b == False:
                    return f"linear all test failed at {item} after {c} comparison(s)"
            return f"linear all test succeeded at finding all values"
        
        def test_binary(self, lyst, target):
            (b, c) = binary_search(lyst, target)
            if b == False:
                return f"binary test failed after {c} comparison(s)"
            elif b == True:
                return f"binary test succeeded after {c} comparison(s)"
            
        def test_binary_all(self, lyst):
            for item in lyst:
                (b, c) = binary_search(lyst, item)
                if b == False:
                    return f"binary all test failed at {item} after {c} comparison(s)"
            return f"binary all test succeeded at finding all values"

        def test_jump(self, lyst, target):
            (b, c) = jump_search(lyst, target)
            if b == False:
                return f"jump test failed after {c} comparison(s)"
            elif b == True:
                return f"jump test succeeded after {c} comparison(s)"
            
        def test_jump_all(self, lyst):
            for item in lyst:
                (b, c) = jump_search(lyst, item)
                if b == False:
                    return f"jump all test failed at {item} after {c} comparison(s)"
            return f"jump all test succeeded at finding all values"

    test = Tests()
    lyst = [1, 2, 3, 4, 5, 6, 7, 8]
    print(test.test_linear(lyst, 3))
    print(test.test_binary(lyst, 1))
    print(test.test_jump(lyst, 3))
    print(test.test_linear_all(lyst))
    print(test.test_binary_all(lyst))
    print(test.test_jump_all(lyst))

if __name__ == "__main__":
    main()