import sys


class Node:
    def __init__(self, parent, val):
        self.children = []
        self.parent = parent
        self.val = val

    def add_child(self, child):
        for i in range(len(self.children)):
            if child.val < self.children[i].val:
                self.children.insert(i, child)
                return child
            if child.val == self.children[i].val:
                return self.children[i]
        self.children.append(child)
        return child
    
    def __str__(self):
        return self.get_string(0)

    def get_string(self, depth):
        ret_val = (" " * depth) + self.val + "\n"
        for child in self.children:
            ret_val += child.get_string(depth + 1)
        return ret_val

    def get_all_aliases(self, str_so_far):
        # If we're the root node, start the string so far
        if str_so_far is None:
            my_str_so_far = self.val
        # We're not the root node, so we need to keep the strings above us in the command tree
        else:
            my_str_so_far = str_so_far + self.val
        # I have multiple children, so I'm not a unique prefix
        if len(self.children) > 1:
            tmp_lst = []
            # Ask my children about unique aliases
            for child in self.children:
                tmp = child.get_all_aliases(my_str_so_far)
                # A tuple return is a flag that my immediate child is not unique, so don't try to make it into a new list
                # of aliases
                if type(tmp) is tuple:
                    tmp_lst.extend(tmp)
                # If they return a list, our immediate child is unique, so we can consider them a unique list of aliases
                else:
                    tmp_lst.append(tmp)
            # We're not unique, so let our parent know not to add us into their list of unique children
            return tuple(tmp_lst)
        # I'm unique, add my string so far to the list and return, but only if my child is unique
        elif len(self.children) == 1:
            tmp = self.children[0].get_all_aliases(my_str_so_far)
            if type(tmp) is tuple:
                return tmp
            else:
                tmp.extend([my_str_so_far])
                return tmp
        # If I'm a leaf return the string including me
        else:
            return [my_str_so_far]

HEADER_STRING = """
%option noyywrap
%{
#include "y.tab.h"
extern int yylval
"""

POST_HEADER_STRING = """
%}

%%
"""


FOOTER_STRING = """
%%

int main(){
    printf("%d", yylex());
}
"""

root = Node(None, "")
currentNode = root
for command in sys.argv[1:]:
    for c in command:
        currentNode = currentNode.add_child(Node(currentNode, c))
    currentNode = root
all_alias_lists = root.get_all_aliases(None)
all_alias_lists_sorted = []
const_definitions = []

print(HEADER_STRING)

for i in range(len(all_alias_lists)):
    all_alias_lists_sorted.append(sorted(all_alias_lists[i], key=len, reverse=False))
#     # TODO add a header file output and dump the definitions in that
#     print("const int %s = %i;" % (all_alias_lists_sorted[-1][-1].upper(), i))

print(POST_HEADER_STRING)

for alias_list in all_alias_lists_sorted:
    # The longest alias is always the command
    longest_alias = alias_list[-1]
    for alias in alias_list:
        if alias == longest_alias:
            print(alias + "\t { printf(\" TODO not yet implemented \"); }")
        else:
            print(alias + "\t|")
    print()

print(FOOTER_STRING)

