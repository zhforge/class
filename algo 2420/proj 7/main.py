# import whatever you need here
import sys
import time
from hashmap import HashMap

hashmap = HashMap()

# Part 1 -- Write weight_on_cacheless() method
def weight_on_cacheless(r,c):
    """
    a person's shouldered weight is half the weight of the people on top of them
    including body weight (200) and their shouldered weight

    take half the weight of the person on top to their left
    and then to their right (both recursive calls)
    base case is if there is no person there
    return accumulated weight
    """
    global function_calls
    function_calls += 1
    weight_left = 0.0
    weight_right = 0.0

    if c != 0:
        weight_left = (200 + weight_on_cacheless(r-1, c-1)) / 2
    
    if r != c:
        weight_right = (200 + weight_on_cacheless(r-1, c)) / 2

    return weight_left + weight_right

# Part 3 -- Write weight_on_with_caching() method
def weight_on_with_caching(r,c):
    global function_calls
    function_calls += 1

    try:
        weight = hashmap.get((r, c))
        global cache_hits
        cache_hits += 1
        return weight
    except KeyError:
        pass

    weight_left = 0.0
    weight_right = 0.0

    if c != 0:
        weight_left = (200 + weight_on_with_caching(r-1, c-1)) / 2
    
    if r != c:
        weight_right = (200 + weight_on_with_caching(r-1, c)) / 2

    hashmap.set((r, c), weight_left + weight_right)

    return weight_left + weight_right
    

def main():
    # Part 2 -- Use weight_on_cacheless() method
    # Cacheless
    global function_calls
    function_calls = 0
    print("Cacheless:")
    start = time.perf_counter()
    i = 0
    num = int(sys.argv[1])
    f = open("cacheless.txt","w")
    while i < num:
        j = 0
        row = ""
        while j <= i:
            row += '{:.2f}'.format((weight_on_cacheless(i,j))) + " "
            j+=1
        print(row)
        f.write(row + '\n')
        i+=1
    elapsed = time.perf_counter() - start
    print("\nElapsed time: " + str(elapsed) + " seconds.")
    print("Number of function calls: " + str(function_calls))
    f.write("\nElapsed time: " + str(elapsed) + " seconds." + '\n')
    f.write(("Number of function calls: " + str(function_calls)) + '\n')
    f.close()
    
    # Part 3 -- Use weight_on_with_caching() method, with your HashMap ADT
    global cache_hits
    cache_hits = 0
    function_calls = 0
    print("\nWith caching:")
    start = time.perf_counter()
    i = 0
    num = int(sys.argv[1])
    f = open("with_caching.txt","w")
    while i < num:
        j = 0
        row = ""
        while j <= i:
            row += '{:.2f}'.format((weight_on_with_caching(i,j))) + " "
            j+=1
        print(row)
        f.write(row + '\n')
        i+=1
    elapsed = time.perf_counter() - start
    print("\nElapsed time: " + str(elapsed) + " seconds.")
    print("Number of function calls: " + str(function_calls))
    print("Number of cache hits: " + str(cache_hits))
    f.write("\nElapsed time: " + str(elapsed) + " seconds." + '\n')
    f.write(("Number of function calls: " + str(function_calls)) + '\n')
    f.write(("Number of cache hits: " + str(cache_hits)) + '\n')
    f.close()

if __name__=="__main__":
    main()

