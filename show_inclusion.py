#!/usr/bin/env python

import argparse
import os
import sys
import os.path
import tempfile

from contextlib import closing

parser = argparse.ArgumentParser(
    description='Show the list of included files from a C++ file. ')
parser.add_argument('-i', '--input', dest='input_file', help='The C++ file', default='')

def extract_file(s, current_folder):
    if s.startswith("<") and s.endswith(">"):
        return (True, s)
    path = s.strip('"')
    if path.startswith("/"):
        return (False, current_folder + path)
    else:
        return (False, current_folder + '/' + path)


def list_files(file_path, guard):
    output_list = []
    with open(file_path) as content:
        for line in content:
            pos = line.find(guard)
            if pos != -1:
                element = extract_file(line[pos + len(guard):].strip(), os.path.dirname(file_path))
                output_list.append(element)
    return output_list

def find_includes(content):
    return list_files(content, '#include')

def generate_dot_aux(element, el_id):
    _, path = element
    name = os.path.basename(path)
    return el_id + ' [shape=box label="' + name + '"];\n'

def generate_dot(root, table):
    output = "digraph graphname {\n"
    output += 'root [shape=box label="' + os.path.basename(root) + '"];\n'
    i = 0
    for el in table:
        el_id = "d" + str(i)
        i += 1
        tree = generate_dot_aux(el, el_id)
        output += tree
        output += "root -> " + el_id + ";\n"
    return output + "\n}"

def main():
    args = parser.parse_args()
    input_file = args.input_file
    cwd = os.getcwd()
    
    if not input_file.startswith(cwd):
        print >> sys.stderr, "Error %s isn't in the working directory %s" % (input_file, cwd)
        return
    
    # Then we look for includes.
    table = find_includes(args.input_file)
    
    # Convert it to dot format.
    content_dot = generate_dot(input_file, table)

    # Creating temp files    
    fd_dot, output_dot = tempfile.mkstemp()
    fd_svg, output_svg = tempfile.mkstemp()
    
    # Write the dot content
    with closing(os.fdopen(fd_dot, 'w')) as f:
        f.write(content_dot);
    
    os.system("dot -Tsvg -o" + output_svg + " " + output_dot)
    
    # To run eog to see the graph:
    os.system("eog " + output_svg)

    # Clean temporaries
    os.remove(output_dot)
    os.remove(output_svg)
    print 'End of Visualization'


if __name__ == '__main__':
    main()




