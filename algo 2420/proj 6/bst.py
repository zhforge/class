'''your bst here'''
class BST:
    class Node:
        def __init__(self, key):
            self.key = key
            self.left = None
            self.right = None
        
        def __str__(self):
            return str(self.key)

    def __init__(self):
        self.root = None

    def size(self):
        return len(self.inorder())

    def is_empty(self):
        if self.root == None:
            return True
        else:
            return False

    def height(self, _node=0):
        if _node == 0: _node = self.root

        if _node == None:
            return -1
        leftHeight = self.height(_node.left)
        rightHeight = self.height(_node.right)
        return 1 + max(leftHeight, rightHeight)

    def add(self, item):
        if self.root == None:
            self.root = self.Node(item)
            return self
        
        else:
            current_node = self.root
            while current_node != None:

                if item < current_node.key: # to the left
                    if current_node.left == None:
                        current_node.left = self.Node(item)
                        return self
                    else:
                        current_node = current_node.left
                
                else: # to the right
                    if current_node.right == None:
                        current_node.right = self.Node(item)
                        return self
                    else:
                        current_node = current_node.right

    def remove(self, item):
        parent = None
        current_node = self.root

        while current_node != None:
            if current_node.key == item:

                if current_node.left == None and current_node.right == None: # leaf
                    if parent == None: # root
                        self.root = None
                    elif parent.left == current_node:
                        parent.left = None
                    else:
                        parent.right = None
                    return self
                
                elif current_node.left != None and current_node.right == None: # left child
                    if parent == None: # root
                        self.root = current_node.left
                    elif parent.left == current_node:
                        parent.left = current_node.left
                    else:
                        parent.right = current_node.left
                    return self
                elif current_node.left == None and current_node.right != None: # right child
                    if parent == None: # root
                        self.root = current_node.right
                    elif parent.left == current_node:
                        parent.left = current_node.right
                    else:
                        parent.right = current_node.right
                    return self
                
                else: # two children
                    successor = current_node.right
                    while successor.left != None:
                        successor = successor.left
                    current_node.key = successor.key
                    parent = current_node
                    current_node = current_node.right
                    item = parent.key # continue down to remove the successor

            elif current_node.key < item: # search right
                parent = current_node
                current_node = current_node.right
            else: # search left
                parent = current_node
                current_node = current_node.left

    def find(self, item):
        current_node = self.root

        while current_node != None:
            if current_node.key == item:
                return current_node.key
            
            elif item < current_node.key:
                current_node = current_node.left
            else:
                current_node = current_node.right
        
        raise ValueError

    def inorder(self, _node=0):
        traversal = []
        if _node == 0: _node = self.root # initialize to root
        if _node == None:
            return []
        
        traversal.extend(self.inorder(_node.left))
        traversal.append(_node.key)
        traversal.extend(self.inorder(_node.right))

        return traversal

    def preorder(self, _node=0):
        traversal = []
        if _node == 0: _node = self.root # initialize to root
        if _node == None:
            return []
        
        traversal.append(_node.key)
        traversal.extend(self.preorder(_node.left))
        traversal.extend(self.preorder(_node.right))

        return traversal

    def postorder(self, _node=0):
        traversal = []
        if _node == 0: _node = self.root # initialize to root
        if _node == None:
            return []
        
        traversal.extend(self.postorder(_node.left))
        traversal.extend(self.postorder(_node.right))
        traversal.append(_node.key)

        return traversal

    def _pretty_tree_helper(self, root, curr_index=0):
        if root is None:
            return [], 0, 0, 0

        line1 = []
        line2 = []
        node_repr = str(root.key)

        new_root_width = gap_size = len(node_repr)
        
        # Get the left and right sub-boxes, their widths, and root repr positions
        l_box, l_box_width, l_root_start, l_root_end = self._pretty_tree_helper(root.left, 2 * curr_index + 1)
        r_box, r_box_width, r_root_start, r_root_end = self._pretty_tree_helper(root.right, 2 * curr_index + 2)

        # Draw the branch connecting the current root to the left sub-box
        # Pad with whitespaces where necessary
        if l_box_width > 0:
            l_root = (l_root_start + l_root_end) // 2 + 1
            line1.append(' ' * (l_root + 1))
            line1.append('_' * (l_box_width - l_root))
            line2.append(' ' * l_root + '/')
            line2.append(' ' * (l_box_width - l_root))
            new_root_start = l_box_width + 1
            gap_size += 1
        else:
            new_root_start = 0

        # Draw the representation of the current root
        line1.append(node_repr)
        line2.append(' ' * new_root_width)

        # Draw the branch connecting the current root to the right sub-box
        # Pad with whitespaces where necessary
        if r_box_width > 0:
            r_root = (r_root_start + r_root_end) // 2
            line1.append('_' * r_root)
            line1.append(' ' * (r_box_width - r_root + 1))
            line2.append(' ' * r_root + '\\')
            line2.append(' ' * (r_box_width - r_root))
            gap_size += 1
        new_root_end = new_root_start + new_root_width - 1

        # Combine the left and right sub-boxes with the branches drawn above
        gap = ' ' * gap_size
        new_box = [''.join(line1), ''.join(line2)]
        for i in range(max(len(l_box), len(r_box))):
            l_line = l_box[i] if i < len(l_box) else ' ' * l_box_width
            r_line = r_box[i] if i < len(r_box) else ' ' * r_box_width
            new_box.append(l_line + gap + r_line)

        # Return the new box, its width and its root positions
        return new_box, len(new_box[0]), new_root_start, new_root_end
        
    def print_tree(self):
        lines = self._pretty_tree_helper(self.root, 0)[0]
        print('\n' + '\n'.join((line.rstrip() for line in lines)))