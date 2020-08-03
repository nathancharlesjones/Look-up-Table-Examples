#!/usr/bin/env python
 
from sys import argv
from io import open
# from networkx import MultiDiGraph, write_dot
from networkx import MultiDiGraph
from networkx.drawing.nx_agraph import write_dot
 
def find_cref(inmap):
    while True:
        l = inmap.readline()
        if l.strip() == 'Cross Reference Table':
            break;
        if len(l) == 0: return False
    while True:
        l = inmap.readline()
        if len(l) == 0: return False
        words = l.split()
        if len(words) == 2:
            if words[0] == 'Symbol' and words[1] == 'File':
                return True
 
def read_cref(inmap):
    modules = MultiDiGraph()
    while True:
        l = inmap.readline()
        words = l.split()
        if len(words) == 2:
            last_symbol = words[0]
            last_module = words[1]
        elif len(words) == 1:
            modules.add_edge(words[0], last_module, label=last_symbol);
        elif len(l) == 0:
            break
    return modules
 
inmap = open(argv[1], 'r')
if find_cref(inmap):
    modules = read_cref(inmap)
    write_dot(modules, argv[2])
else:
    print 'error: cross reference table not found.'