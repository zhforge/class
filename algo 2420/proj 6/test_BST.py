import pytest
from bst import BST

class Test_BST:
    def make_tree(self):
        tree = BST()
        tree.add(5)
        tree.add(2)
        tree.add(1)
        tree.add(3)
        tree.add(7)
        tree.add(6)
        tree.add(8)
        return tree

    def test_add(self):
        # should be able to make a tree of any comparable data type
        #   but not different ones in the same tree
        #   (which will automatically raise an error after trying to compare)
        tree = BST()
        tree.add("b")
        tree.add("c")
        tree.add("a")
        assert tree.root.key == "b"
        assert tree.root.left.key == "a"
        assert tree.root.right.key == "c"

    def test_remove(self):
        # do nothing if item isn't in the tree
        tree = self.make_tree()
        tree.remove(8)
        tree.remove(2)
        tree.remove(5)
        assert tree.root.key == 6

    def test_size(self):
        # number of nodes
        tree = self.make_tree()
        assert tree.size() == 7

    def test_is_empty(self):
        tree = self.make_tree()
        assert not tree.is_empty()
        tree2 = BST()
        assert tree2.is_empty()

    def test_height(self):
        # zero nodes means height of -1
        tree = self.make_tree()
        assert tree.height() == 2
        tree.add(10)
        assert tree.height() == 3
        tree2 = BST()
        assert tree2.height() == -1
        tree2.add("a")
        assert tree2.height() == 0

    def test_find(self):
        # i guess just return the same item if found, otherwise raise ValueError
        tree = self.make_tree()
        assert tree.find(3) == 3
        with pytest.raises(ValueError):
            tree.find(4)

    def test_inorder(self):
        tree = self.make_tree()
        assert tree.inorder()[0] == 1
        assert tree.inorder()[-1] == 8

    def test_preorder(self):
        tree = self.make_tree()
        assert tree.preorder()[0] == 5
        assert tree.preorder()[-1] == 8

    def test_postorder(self):
        tree = self.make_tree()
        assert tree.postorder()[0] == 1
        assert tree.postorder()[-1] == 5

