import pytest
from hashmap import HashMap

class Test_Hashmap:
    def make_hashmap(self) -> HashMap:
        h = HashMap()
        h.set(1, 1)
        h.set(2, 2)
        h.set(3, 3)
        h.set(4, "a")
        h.set(5, "b")
        return h

    def test_get(self):
        h = self.make_hashmap()
        assert h.get(1) == 1
        assert h.get(3) == 3
        assert h.get(5) == "b"

    def test_set(self):
        h = self.make_hashmap()
        h.set(6, "c")
        h.set(3, "d")

    def test_remove(self):
        h = self.make_hashmap()
        h.remove(2)
        with pytest.raises(KeyError):
            h.get(2)
    
    def test_clear(self):
        h = self.make_hashmap()
        h.clear()
        with pytest.raises(KeyError):
            h.get(2)

    def test_capacity(self):
        h = self.make_hashmap()
        assert h.capacity() == 7
        h.set(6, "c")
        assert h.capacity() == 13
    
    def test_size(self):
        h = self.make_hashmap()
        assert h.size() == 5
        h.remove(2)
        assert h.size() == 4
        h.set(2, 2)
        h.set(6, "c")
        assert h.size() == 6

    def test_keys(self):
        h = self.make_hashmap()
        keys = h.keys()
        keys.sort()
        assert keys == [1, 2, 3, 4, 5]