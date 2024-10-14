"""
This is the street database.

author: Yulin Wu
email: y297wu@uwaterloo.ca
"""

class StreetDatabase:
    def __init__(self) -> None:
        """
        initialize database with one dictionary
        <K: street name, V: coordinates list>
        """
        self._streetDatabase = dict()

    def add(self, street_name, coordinates_list):
        # check if the street already exists
        if street_name in self._streetDatabase:
            raise Exception(f'`add\' specified for a street name `{street_name}\' that already exists in the street database.')
        
        # check sematic error in coordinate list
        # 1. street has a duplicated segment vertex
        s = set()
        for coordinate in coordinates_list:
            s.add(coordinate)
        if len(s) < len(coordinates_list):
            raise Exception(f'`add\' specified a street with duplicated segment vertices.')
        elif len(s) > len(coordinates_list):
            raise Exception(f'street database add: unknown error.')
        
        # add street to database
        self._streetDatabase[street_name] = coordinates_list

    def mod(self, street_name, coordinates_list):
        # check if the street exists
        if street_name not in self._streetDatabase:
            raise Exception(f'`mod\' specified for a street that does not exist.')
        
        # check sematic error in coordinate list
        # 1. street has a duplicated segment vertex
        s = set()
        for coordinate in coordinates_list:
            s.add(coordinate)
        if len(s) < len(coordinates_list):
            raise Exception(f'`mod\' specified a street with duplicated segment vertices.')
        elif len(s) > len(coordinates_list):
            raise Exception(f'street database mod: unknown error.')
        
        # update street database
        self._streetDatabase[street_name] = coordinates_list

    def rm(self, street_name):
        # check if the street exists
        if street_name not in self._streetDatabase:
            raise Exception(f'`rm\' specified for a street that does not exist.')
        
        # remove street from database
        self._streetDatabase.pop(street_name)

    def __str__(self) -> str:
        return str(self._streetDatabase)

