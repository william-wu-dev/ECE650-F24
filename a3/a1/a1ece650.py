#!/usr/bin/env python3
import sys

# YOUR CODE GOES HERE
from line_parser import line_parser
from StreetDatabase import StreetDatabase
from graph_generator import graph_generator

DEBUG = False

def main():
    # YOUR MAIN CODE GOES HERE
    sdb = StreetDatabase()  # instantiate street database
    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    for line in sys.stdin:
        if line == "" or line[0] == "#" or line == "\n":
            continue
    # while True:
    #     line = sys.stdin.readline()
    #     if line == "":
    #         break
        if DEBUG:
            print('A1 READ LINE:', line, file=sys.stderr)
        try:
            # parse command
            command, street_name, coordinates_list = line_parser(line)

            # switch action based on command like a compiler
            if command == 'add':
                sdb.add(street_name, coordinates_list)
            elif command == 'mod':
                sdb.mod(street_name, coordinates_list)
            elif command == 'rm':
                sdb.rm(street_name)
            elif command == 'gg':
                # NOTE: graph generator. Now we just print database for testing
                # print(str(sdb), file=sys.stdout)
                graph = graph_generator(sdb)
                print(str(graph), file=sys.stdout, flush=True)
                if DEBUG:
                    print('A1 OUT LINE:', str(graph), file=sys.stderr)
            else:
                raise Exception(f'unknown command parsed `{command}\'')
        except Exception as e:
            print('Error:', str(e), file=sys.stderr)

    # print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()
