class SList:
    class SListNode:
        def __init__ (self, value = None):
            self.value = value
            self.next = None

        def __str__ (self):
            return str(self.value)

    def __init__ (self):
        self._head = None
        self._size = 0
        self._iter_index = 0

    '''Insert a new value in the list. Maintain nondecreasing ordering of elements'''
    def insert(self, value):
        new_node = self.SListNode(value)

        if self._size == 0:
            self._head = new_node

        else:
            curr_node = self._head
            prev_node = None

            while curr_node != None and curr_node.value <= value:
                prev_node = curr_node
                curr_node = curr_node.next

            if prev_node == None: # insert as first element
                self._head = new_node
                new_node.next = curr_node
            
            else: # insert between prev_node and curr_node
                prev_node.next = new_node
                new_node.next = curr_node

        self._size += 1
    
    '''Search for a value in the list, return it if found, None otherwise'''
    def find(self, value):
        for node in self:
            if node.value == value:
                return node.value
        return None

    '''Remove the first occurance of value.'''
    def remove(self, value):
        to_remove = None
        prev_node = None
        next_node = None

        for node in self:
            if node.value == value:
                to_remove = node
                next_node = to_remove.next
                break
            prev_node = node
        
        if to_remove == None:
            return False
        
        if prev_node == None: # to_remove is the first element
            self._head = next_node
        else:
            prev_node.next = next_node
        del to_remove
        self._size -= 1
        return True

    '''Remove all instances of value'''
    def remove_all(self, value):
        removing = True
        while removing:
            removing = self.remove(value)

    '''Return the number of elements in the list'''
    def size(self):
        return self._size

    '''Convert the list to a string and return it'''
    def __str__(self):
        out = ""
        for element in self:
            out += str(element)
            if element.next != None:
                out += ", "
        return f"[{out}]"

    '''Return an iterator for the list'''
    def __iter__(self):
        self._iter_index = 0
        return self
    
    def __next__(self):
        if self._head == None or self._iter_index == self._size:
            self._iter_index = 0
            raise StopIteration
        
        if self._iter_index == 0:
            self._iter_index += 1
            return self._head
        
        node = self._head
        for i in range(self._iter_index):
            node = node.next
        self._iter_index += 1
        return node

    '''Return the item at the given index, or throw an exception if invalid index'''
    def __getitem__(self, index):
        if index < 0 or index >= len(self):
            raise IndexError("Out of index")
        
        node = self._head
        if index == 0:
            return node.value
        
        for i in range(index):
            node = node.next

            if node == None: # should never happen but just in case
                raise IndexError("Out of index")

        return node.value


    def __len__(self):
        length = 0
        node = self._head

        while node != None:
            node = node.next
            length += 1

        return length