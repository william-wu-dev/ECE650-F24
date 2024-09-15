"""
This is the draft for developing coordinate regex
"""
import re

coordinates_line = '''(  2  2,-1) (2, 2) (5,5    ) ( 5 ,6 ) (  ,8)(1, 2)(55)(5 5)(1a, -7b)'''
coordinates_list = re.findall(r'\([^\(\)]*\)', coordinates_line)

print(coordinates_list)