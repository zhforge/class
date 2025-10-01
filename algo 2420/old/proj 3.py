from random import *

"""
function output: (sorted list, comparisons, swaps) -> tuple

test cases:
make sure list is sorted, comparisons and swaps are correct
empty case
lyst is not a list
lyst contains non-integer items
"""

class Test:
    def __init__(self, lyst):
        self.lyst = lyst

    def return_outputs(self, lyst):
        outputs = []
        outputs.append(quicksort(lyst.copy()))
        outputs.append(selection_sort(lyst.copy()))
        outputs.append(insertion_sort(lyst.copy()))
        outputs.append(mergesort(lyst.copy()))
        return outputs
    
    def empty_case(self):
        # if list to sort is empty, return empty list, 0 comparisons, 0 swaps
        for o in self.return_outputs([]):
            assert o == ([], 0, 0)
    
    def not_list(self):
        # if lyst is not a list, function should raise TypeError
        try: quicksort(1)
        except TypeError: assert True
        else: assert False

        try: selection_sort(1)
        except TypeError: assert True
        else: assert False

        try: insertion_sort(1)
        except TypeError: assert True
        else: assert False

        try: mergesort(1)
        except TypeError: assert True
        else: assert False

    def non_int_items(self):
        # if list contains non-integer items, function should raise TypeError
        x = [1, 2, "buckle my shoe"]
        try: quicksort(x)
        except TypeError: assert True
        else: assert False

        try: selection_sort(x)
        except TypeError: assert True
        else: assert False

        try: insertion_sort(x)
        except TypeError: assert True
        else: assert False

        try: mergesort(x)
        except TypeError: assert True
        else: assert False

    def general_case(self):
        # make sure list gets sorted correctly
        outputs = self.return_outputs(self.lyst)
        for o in outputs:
            print(o)
            assert is_sorted(o[0])

    def test_cases(self):
        self.empty_case()
        self.not_list()
        self.non_int_items()
        self.general_case()
        print("All tests passed")

def is_sorted(lyst):
    if not isinstance(lyst, list): raise TypeError
    for item in lyst:
        if not isinstance(item, int): raise TypeError
    
    for i in range(len(lyst)-1):
        if lyst[i] > lyst[i+1]:
            return False
    return True
    
def quicksort(lyst, low_end_index=0, high_end_index=-1, comparisons=0, swaps=0):
    if not isinstance(lyst, list): raise TypeError
    for item in lyst:
        if not isinstance(item, int): raise TypeError
    
    if high_end_index == -1: high_end_index = len(lyst)-1
    if low_end_index >= high_end_index: return (lyst, comparisons, swaps)

    # partition
    low_index = low_end_index
    high_index = high_end_index
    midpoint = (low_index + high_index) // 2
    pivot = lyst[midpoint]

    done = False
    while not done:
        while lyst[low_index] < pivot:
            low_index += 1
            comparisons += 1
        comparisons += 1
        while pivot < lyst[high_index]:
            high_index -= 1
            comparisons += 1
        comparisons += 1

        if low_index >= high_index:
            done = True
        else:
            lyst[low_index], lyst[high_index] = lyst[high_index], lyst[low_index]
            swaps += 1
            low_index += 1
            high_index -= 1
    

    # quicksort algorithm
    x, comp_add, swap_add = quicksort(lyst, low_end_index, high_index)
    comparisons += comp_add
    swaps += swap_add
    x, comp_add, swap_add = quicksort(lyst, high_index+1, high_end_index)
    comparisons += comp_add
    swaps += swap_add

    return (lyst, comparisons, swaps)
    
def selection_sort(lyst):
    if not isinstance(lyst, list): raise TypeError
    for item in lyst:
        if not isinstance(item, int): raise TypeError

    comparisons = 0
    swaps = 0
    for i in range(len(lyst)):
        min_index = i
        for j in range(i+1, len(lyst)):
            comparisons += 1
            if lyst[j] < lyst[min_index]:
                min_index = j
        if lyst[i] != lyst[min_index]:
            swaps += 1
            lyst[i], lyst[min_index] = lyst[min_index], lyst[i]
    return (lyst, comparisons, swaps)
    
def insertion_sort(lyst):
    if not isinstance(lyst, list): raise TypeError
    for item in lyst:
        if not isinstance(item, int): raise TypeError

    comparisons = 0
    swaps = 0
    for i in range(1, len(lyst)):
        j = i
        while j > 0 and lyst[j] < lyst[j-1]:
            comparisons += 1
            swaps += 1
            lyst[j], lyst[j-1] = lyst[j-1], lyst[j]
            j -= 1
        comparisons += 1
    return (lyst, comparisons, swaps)
    
def mergesort(lyst, i=0, k=-1, comparisons=0, swaps=0):
    if not isinstance(lyst, list): raise TypeError
    for item in lyst:
        if not isinstance(item, int): raise TypeError

    if k == -1: k = len(lyst)-1
    j = 0

    if i < k:
        j = (i + k) // 2

        x, comp_add, swap_add = mergesort(lyst, i, j)
        comparisons += comp_add
        swaps += swap_add
        x, comp_add, swap_add = mergesort(lyst, j+1, k)
        comparisons += comp_add
        swaps += swap_add

        # merge
        merged_size = k-i+1
        merge_pos = 0
        left_pos = i
        right_pos = j+1
        merged_numbers = [0]*merged_size

        while (left_pos <= j) and (right_pos <= k):
            if lyst[left_pos] <= lyst[right_pos]:
                merged_numbers[merge_pos] = lyst[left_pos]
                left_pos += 1
            else:
                merged_numbers[merge_pos] = lyst[right_pos]
                right_pos += 1
            merge_pos += 1
            comparisons += 1
        
        while left_pos <= j:
            merged_numbers[merge_pos] = lyst[left_pos]
            left_pos += 1
            merge_pos += 1
        
        while right_pos <= k:
            merged_numbers[merge_pos] = lyst[right_pos]
            right_pos += 1
            merge_pos += 1

        for merge_pos in range(merged_size):
            lyst[i+merge_pos] = merged_numbers[merge_pos]
            swaps += 0.5

    return (lyst, comparisons, int(swaps))

def main():
    DATA_SIZE = 100
    #seed(0)
    DATA = sample(range(DATA_SIZE * 3), k=DATA_SIZE)
    test = DATA.copy() # don’t sort DATA, sort a copy of DATA

    t = Test(test)
    t.test_cases()
    

if __name__ == "__main__":
    main()