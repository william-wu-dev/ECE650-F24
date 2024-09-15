import re
import sys

"""
This is the command line parser.

author: Yulin Wu
email: y297wu@uwaterloo.ca
"""

def line_parser(line):
    """
    This parser parse one line of string command into command, street name and coordinates. 
    The parser is expected to raise syntax and sematic errors.

    input: string, line of command.
    output: if input is valid:
            1. string, parsed command
            2. string, parsed street name
            3. list of tuples, parsed coordinates
    """

    line = line.strip()  # strip trailing space in the front and the end of the line

    # test count the number of double quotation mark, 0 or 2 allowed
    # double quotation mark is important, that's the flag used to separate arguments
    double_quotation_count = 0
    for ch in line:
        if ch == '"':
            double_quotation_count += 1
    if double_quotation_count != 0 and double_quotation_count != 2:
        raise Exception("double quotation mark should match and ONLY be used to enclose street name.")
    
    # split command line into arguments
    l = re.split("\"", line)
    l = [arg.strip() for arg in l]

    # check command and argument count
    command = l[0]
    if command not in ['add', 'mod', 'rm', 'gg']:
        raise Exception(f'command `{command}\' not valid.')
    
    # check syntax based on command
    if command == 'add' or command == 'mod':
        # check argument number
        if len(l) < 3:
            raise Exception(f'missing arguments for `{command}\'')
        elif len(l) > 3:
            raise Exception(f'too many arguments for `{command}\'')
        # check street name
        street_name = l[1]
        street_name = street_name.lower()  # street name is not case sensitive
        if street_name == '':
            raise Exception(f'missing street name for `{command}\'')
        # check and parse coordinates
        coordinates_line = l[2]
        if coordinates_line == '':
            raise Exception(f'missing coordinates for `{command}\'')
        coordinates_list = coordinate_parser(coordinates_line)
        if len(coordinates_list) <= 1:
            raise Exception(f'there should be more than one coordinate for `{command}\'')
        return command, street_name, coordinates_list
    elif command == 'rm':
        # check argument number
        if len(l) < 2:
            raise Exception(f'missing arguments for `{command}\'')
        elif len(l) > 2 and len(l[2]) != 0:
            raise Exception(f'too many arguments for `{command}\'')
        # check street name
        street_name = l[1]
        street_name = street_name.lower()  # street name is not case sensitive
        if street_name == '':
            raise Exception(f'missing street name for `{command}\'')
        return command, street_name, []
    elif command == 'gg':
        # check argument number
        if len(l) > 1:
            raise Exception(f'too many arguments for `{command}\'')
        return command, '', []
    else:
        raise Exception(f'{command}: Unknown error 1')

def coordinate_parser(coordinates_line):
    #   test parentheses match and some unexpected characters between coordinates
    parentheses_match_indicator = 0  # 0 means match
    for ch in coordinates_line:
        if ch == '(':
            parentheses_match_indicator += 1
        elif ch == ')':
            parentheses_match_indicator -= 1
        elif ch != ' ' and parentheses_match_indicator <= 0:
            raise Exception(f'unexpected character between coordinates in {coordinates_line}')
        if parentheses_match_indicator < -1 or parentheses_match_indicator > 1:
            raise Exception(f'nesting parentheses is not allowed in {coordinates_line}')
    if parentheses_match_indicator != 0:
        raise Exception(f'coordinates parentheses not match in {coordinates_line}')
    #   test all coordinates are valid and parse valid coordinates
    #   1. cut coordinate line into list of coordinates containing format like (a, b),
    #       a b might be invalid
    #   2. check every coordinate in the list validity: find 1 comma and 2 numbers in the coordinate, 
    #       if not, then that coordinate is invalid.
    #       if yes, then that coordinate is valid, you should parse it as int, as I am 
    #       "allowed to assume that is integer"
    coordinates_list = re.findall(r'\([^\(\)]*\)', coordinates_line)

    parse_result = []
    for coordinate in coordinates_list:
        parts = re.split(r',', coordinate)  # split x and y by comma
        if len(parts) != 2:
            raise Exception(f'one comma is expected in coordinate {coordinate}')
        
        x = parts[0]
        y = parts[1]
        x = re.sub(r'[\(\)]', '', x).strip()  # get rid of space and parenthesis
        y = re.sub(r'[\(\)]', '', y).strip()  # get rid of space and parenthesis

        # parse coordinates
        try:
            x = int(x)
            y = int(y)
            parse_result.append((x, y))
        except ValueError as inst:
            raise Exception(f'not-a-integer-number found in coordinate {coordinate}')
    return parse_result
