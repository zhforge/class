import pytest
from stack import Stack
from main import eval_postfix, in2post

class TestStack:
    def test_init(self):
        self.stack = Stack()

    def test_push(self):
        self.stack = Stack()
        self.stack.push(5)
        self.stack.push(3)
    
    def test_pop(self):
        self.stack = Stack()
        with pytest.raises(IndexError):
            self.stack.pop()
        self.stack.push(5)
        self.stack.push(3)
        assert self.stack.pop() == 3
    
    def test_top(self):
        self.stack = Stack()
        with pytest.raises(IndexError):
            self.stack.top()
        self.stack.push(1)
        self.stack.push(2)
        self.stack.push(3)
        assert self.stack.top() == 3
        self.stack.pop()
        assert self.stack.top() == 2

    def test_size(self):
        self.stack = Stack()
        assert self.stack.size() == 0
        self.stack.push(1)
        assert self.stack.size() == 1
        self.stack.push(1)
        assert self.stack.size() == 2

    def test_clear(self):
        self.stack = Stack()
        self.stack.push(1)
        self.stack.push(2)
        self.stack.push(3)
        self.stack.clear()
        assert self.stack.size() == 0


