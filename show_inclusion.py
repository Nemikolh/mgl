#!/usr/bin/env python

import argparse
import os
import sys
import os.path
import tempfile

from contextlib import closing

# Parser Arguments
parser = argparse.ArgumentParser(description='Show the list of included files from a C++ file. ')
parser.add_argument('-i', '--input', dest='input_file', help='The C++ file', default='')
parser.add_argument('-V', '--visualizer', dest='visua', help='The Visualizer program', default='eog')
parser.add_argument('-d', '--dot-parser', dest='dotpa', help='The dot parser', default='dot')
parser.add_argument('-s', '--show-system', dest='show_system', help='If this parameter is set then the system nodes are shown', action='store_true')
parser.add_argument('-LRO', '--LR-orientation', dest='is_lr_orientation', help='If this parameter is set then the graph is oriented left to right istead of up to down', action='store_true')

# Class to have a ref on an integer
class IntRef:
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return str(self.value)
    def __repr__(self):
        return str(self.value)

#--------------------------------------------------------------------------------------------------#
# Top function that is based on list_files
def find_includes(file_path, show_system):
    root = (False, IntRef(0), file_path, [])
    output_list = []
    list_files(root, '#include', {}, output_list, show_system)
    return output_list

# Auxiliary function to filter between system includes and non-system includes
def extract_file(s, current_folder):
    if s.startswith("<") and s.endswith(">"):
        return (True, IntRef(0), s, [])
    path = s.strip('"')
    if path.startswith("/"):
        return (False, IntRef(0), current_folder + path, [])
    else:
        return (False, IntRef(0), current_folder + '/' + path, [])

def is_not_commented(line):
    return not line.strip().startswith('//')

def get_name(element):
    return os.path.basename(element[2])
    
def is_not_system(element):
    return not element[0]

def rank(element):
    return element[1]

# List the files Recursive function
def list_files(current_element, guard, map_name_place, output_list, show_system):
    output_list.append(current_element)
    index = output_list.index(current_element)
    map_name_place[get_name(current_element)] = index
    file_path = current_element[2]
    with open(file_path) as content:
        for line in content:
            pos = line.find(guard)
            if pos != -1 and is_not_commented(line):
                element = extract_file(line[pos + len(guard):].strip(), os.path.dirname(file_path))
                rank(element).value = rank(current_element).value + 1
                if get_name(element) in map_name_place:
                    index = map_name_place[get_name(element)]
                else:
                    if is_not_system(element):
                        list_files(element, guard, map_name_place, output_list, show_system)
                        index = output_list.index(element)
                    else:
                        output_list.append(element)
                        index = output_list.index(element)
                        map_name_place[get_name(element)] = index
                if show_system or is_not_system(element):
                    current_element[3].append(index)



#--------------------------------------------------------------------------------------------------#
# Generate dot
def generate_dot(root, table, show_system, is_lr_orientation):
    output = "digraph graphname {\n"
    if is_lr_orientation:
        output += "rankdir=LR; \n"
    output += 'root [shape=box fontsize=15 color=blue label="' + os.path.basename(root) + '"];\n'
    for i in range(0, len(table)):
        el = table[i]
        el_id = get_node_name(i)
        edges = link_to_neighbors(el, i, show_system)
        if edges != '':
            output += edges
    # Add layers:
    output += define_layers(table)
    return output + "\n}"

# Link node to neighbors
def link_to_neighbors(element, el_id, show_system):
    # Extract information from element
    is_system, nb_included, path, neighbors = element
    # Node Aspect
    output = ''
    if el_id > 0:
        output = get_node_name(el_id) + node_aspect(is_system, path)
    if is_system:
        if show_system:
            return output
        return ''
    # Add links with neighbors
    for n in neighbors:
        output += get_node_name(el_id) + ' -> ' + get_node_name(n) + ";\n"
    return output

# Generate name for node
def get_node_name(i):
    if i == 0:
        return "root"
    return "n" + str(i)

# Define the layers.
def define_layers(table):
    map_ = {}
    for i in range(0, len(table)):
        element = table[i]
        r = rank(element).value
        if r in map_:
            map_[r].append(get_node_name(i))
        else:
            map_[r] = [get_node_name(i)]
    return reduce(lambda x,y: x + y, map(lambda l: '{rank = same; ' + reduce(lambda x,y: x + ' ' + y, l) + '}\n' , map_.values()))

# Return the visual aspect for the node
def node_aspect(is_filled, node_name):
    if is_filled:
        name = node_name
        color = '.7 .3 1.0'
        style = 'style=filled'
        fontsize = str(14)
    else:
        name = os.path.basename(node_name)
        color = 'red'
        style = ''
        fontsize = str(15)
    return ' [shape=box fontsize='+ fontsize +' label="' + name + '" ' + style +  ' color="' + color + '"];\n'

    
#--------------------------------------------------------------------------------------------------#
# main function
def main():
    args = parser.parse_args()
    input_file = args.input_file
    cwd = os.getcwd()
    
    if not input_file.startswith(cwd):
        print >> sys.stderr, "Error %s isn't in the working directory %s" % (input_file, cwd)
        return
    
    # Then we look for includes.
    table = find_includes(args.input_file, args.show_system)
    
    # Convert it to dot format.
    content_dot = generate_dot(input_file, table, args.show_system, args.is_lr_orientation)
    
    # Creating temp files    
    fd_dot, output_dot = tempfile.mkstemp()
    fd_svg, output_svg = tempfile.mkstemp()
    
    # Write the dot content
    with closing(os.fdopen(fd_dot, 'w')) as f:
        f.write(content_dot);
    
    os.system(args.dotpa + " -Tsvg -o" + output_svg + " " + output_dot)
    
    # To run eog to see the graph:
    os.system(args.visua + " " + output_svg)

    # Clean temporaries
    os.remove(output_dot)
    os.remove(output_svg)
    print 'End of Visualization'


if __name__ == '__main__':
    main()




