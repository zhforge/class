class HashMap:
    class HashMapItem:
        def __init__(self, key, value):
            self.key = key
            self.value = value
            self.next = None

    
    def __init__(self):
        self._table = [None] * 7

    def _hash(self, key: tuple[int, int]): # 2-in-1 hash + index
        if isinstance(key, int): # (for testing)
            return key % len(self._table)
        
        # row^2 + column
        # gurantees a unique value for each person in the pyramid
        # (while skipping a few integers)
        return (key[0]**2 + key[1]) % len(self._table)
    
    def _rehash(self):
        old_table = self._table
        self._table = [None] * (len(old_table) * 2 - 1)

        for item in old_table:
            if item != None:
                self.set(item.key, item.value)
                next = item.next
                while next != None:
                    self.set(next.key, next.value)
                    next = next.next
        

    def get(self, key): # search
        index = self._hash(key)
        
        # Search the bucket's linked list for the key
        item = self._table[index]
        while item != None:
            if key == item.key:
                return item.value
            item = item.next
        raise KeyError("Key not found") # key not found

    def set(self, key, value): # insert
        index = self._hash(key)

        # Traverse the linked list, searching for the key. If the key exists in 
        # an item, the value is replaced. Otherwise a new item is appended.
        item = self._table[index]
        previous = None
        while item != None:
            if key == item.key:
                item.value = value
                return
            
            previous = item
            item = item.next
      
        # Append to the linked list
        if self._table[index] == None:
            self._table[index] = self.HashMapItem(key, value)
        else:
            previous.next = self.HashMapItem(key, value)

        # check if need to rehash
        f = self.size() / self.capacity() # load factor
        if f >= 0.8:
            self._rehash()

    def remove(self, key):
        index = self._hash(key)
        
        # Search the bucket's linked list for the key
        item = self._table[index]
        previous = None
        while item != None:
            if key == item.key:
                if previous == None:
                    # Remove linked list's first item
                    self._table[index] = item.next
                else:
                    previous.next = item.next
                return
            previous = item
            item = item.next
        return # key not found

    def clear(self):
        self._table = [None] * 7

    def capacity(self):
        return len(self._table)

    def size(self):
        total = 0

        for item in self._table:
            if item != None:
                total += 1
                next = item.next
                while next != None:
                    total += 1
                    next = next.next
        
        return total

    def keys(self):
        keys = []

        for item in self._table:
            if item != None:
                keys.append(item.key)
                next = item.next
                while next != None:
                    keys.append(next.key)
                    next = next.next
        
        return keys