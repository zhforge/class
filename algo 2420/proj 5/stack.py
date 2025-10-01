class Stack:
    def __init__(self):
        self._stack = []
    
    def push(self, item):
        self._stack.append(item)
    
    def pop(self):
        if self.size() == 0: raise IndexError
        return self._stack.pop()
    
    def top(self):
        if self.size() == 0: raise IndexError
        return self._stack[-1]
    
    def size(self):
        return len(self._stack)
    
    def clear(self):
        self._stack.clear()