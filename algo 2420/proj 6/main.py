'''
Project: 6 - Trees
Author: Zachary Carter
Course: CS-2420
Date: yes please

Description: like & subscribe

Lessons Learned: all of the above

'''
from pathlib import Path
from string import whitespace, punctuation
from bst import BST
import string


class Pair:
    ''' Encapsulate letter,count pair as a single entity.
    
    Relational methods make this object comparable
    using built-in operators. 
    '''
    def __init__(self, letter, count = 1):
        self.letter = letter
        self.count = count
    
    def __eq__(self, other):
        return self.letter == other.letter
    
    def __hash__(self):
        return hash(self.letter)

    def __ne__(self, other):
        return self.letter != other.letter

    def __lt__(self, other):
        return self.letter < other.letter

    def __le__(self, other):
        return self.letter <= other.letter

    def __gt__(self, other):
        return self.letter > other.letter

    def __ge__(self, other):
        return self.letter >= other.letter

    def __repr__(self):
        return f'({self.letter}, {self.count})'
    
    def __str__(self):
        return f'({self.letter}, {self.count})'

def make_tree():
    f = open("around-the-world-in-80-days-3.txt", "r")
    tree = BST()

    for line in f:
        for char in line:
            try: # find letter in the tree, update it
                pair = tree.find(Pair(char.lower()))
                pair.count += 1

            except ValueError: # new letter, add it to the tree
                if not char.isspace() and char not in string.punctuation:  # FIXME: if it fails the test, check for "punctuation and whitespace" only instead
                    tree.add(Pair(char.lower()))
        
    return tree


def main():
    tree = make_tree()
    
if __name__ == "__main__":
    main()