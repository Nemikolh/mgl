#!/usr/bin/env python

import argparse

parser = argparse.ArgumentParser(
    description='Show the list of included files of a C++ file. ')
parser.add_argument('-i', '--input', dest='input_file', help='The C++ file', default='')

def main():
    args = parser.parse_args()
    print args.input_file
    

if __name__ == '__main__':
    main()
