"""
This is the graph generator.

author: Yulin Wu
email: y297wu@uwaterloo.ca
"""

from Graph import Graph
from StreetDatabase import StreetDatabase

import copy

def graph_generator(sdb: StreetDatabase) -> Graph:
    # initialization
    # set of intersection
    # dictionary of added intersection street segment
    # result
    intersections_set = set()
    intermediate_sdb = copy.deepcopy(sdb._streetDatabase)
    res = Graph()

    # double pointer iterate sdb find intersection
    street_list = list(sdb._streetDatabase.keys())
    # print("==================")  # debug line
    # print(f'street len: {len(street_list)}')  # debug line
    for ii in range(0, len(street_list) - 1):  # warning, i is already used in the later part of the loop, something nasty will happen. FIX: change to ii and jj
        for jj in range(ii + 1, len(street_list)):
            # print((ii, jj))  # debug line
            street_1 = street_list[ii]
            street_2 = street_list[jj]
            # count the intersection for every segment in 1 X every segment in 2
            coordinates_list_1 = sdb._streetDatabase[street_1]
            coordinates_list_2 = sdb._streetDatabase[street_2]
            # iterate every segment in 1 and every segment in 2
            for m in range(0, len(coordinates_list_1) - 1):
                for n in range(0, len(coordinates_list_2) - 1):
                    p1 = Point(coordinates_list_1[m])
                    q1 = Point(coordinates_list_1[m + 1])
                    p2 = Point(coordinates_list_2[n])
                    q2 = Point(coordinates_list_2[n + 1])

                    # check if <p1, q1> and <p2, q2> intersect and add intersection and add these 4 end-points to the result

                    check_intersect_res = check_intersect(p1, q1, p2, q2)

                    if check_intersect_res == 1:
                        # intersect regularly, 
                        # 1. find the only intersect and 
                        # 2. add intersection to set intersections_set and 
                        # 3. add intersection and 4 endpoints to res and 
                        # 4. add intersection to intermediate db to break the line segment
                        intersection = intersect(p1, q1, p2, q2)

                        intersections_set.add(intersection.to_tuple())

                        res.add_vertex(intersection.to_tuple())
                        res.add_vertex(p1.to_tuple())
                        res.add_vertex(q1.to_tuple())
                        res.add_vertex(p2.to_tuple())
                        res.add_vertex(q2.to_tuple())

                        # add intersection to break line segment <p1, q1> in intermediate_sdb
                        # 1. determine the break position
                        # 2. insert 
                        intermediate_coordinates_list_1 = intermediate_sdb[street_1]
                        lo = intermediate_coordinates_list_1.index(p1.to_tuple())
                        hi = intermediate_coordinates_list_1.index(q1.to_tuple())
                        i = lo + 1
                        while dist(Point(intermediate_coordinates_list_1[lo]), Point(intermediate_coordinates_list_1[i])) <= \
                            dist(Point(intermediate_coordinates_list_1[lo]), intersection) and  i < hi:
                            i += 1
                        intermediate_coordinates_list_1.insert(i, intersection.to_tuple())

                        # the same for the other line segment
                        intermediate_coordinates_list_2 = intermediate_sdb[street_2]
                        lo = intermediate_coordinates_list_2.index(p2.to_tuple())
                        hi = intermediate_coordinates_list_2.index(q2.to_tuple())
                        i = lo + 1
                        while dist(Point(intermediate_coordinates_list_2[lo]), Point(intermediate_coordinates_list_2[i])) <= \
                            dist(Point(intermediate_coordinates_list_2[lo]), intersection) and i < hi:
                            i += 1
                        intermediate_coordinates_list_2.insert(i, intersection.to_tuple())

                    elif check_intersect_res == 0:
                        # intersect by overlap, intersection number may vary as they might have only overlap a vertex.
                        # so, for every intersect we must do the same as only 1 intersect
                        intersections_list = overlap_intersect(p1, q1, p2, q2)

                        for intersection in intersections_list:
                            intersections_set.add(intersection.to_tuple())

                        for intersection in intersections_list:
                            res.add_vertex(intersection.to_tuple())

                        res.add_vertex(p1.to_tuple())
                        res.add_vertex(q1.to_tuple())
                        res.add_vertex(p2.to_tuple())
                        res.add_vertex(q2.to_tuple())

                        # add intersection to break line segment <p1, q1> in intermediate_sdb
                        # 1. determine the break position
                        # 2. insert 
                        for intersection in intersections_list:
                            intermediate_coordinates_list_1 = intermediate_sdb[street_1]
                            lo = intermediate_coordinates_list_1.index(p1.to_tuple())
                            hi = intermediate_coordinates_list_1.index(q1.to_tuple())
                            i = lo + 1
                            while dist(Point(intermediate_coordinates_list_1[lo]), Point(intermediate_coordinates_list_1[i])) <= \
                                dist(Point(intermediate_coordinates_list_1[lo]), intersection) and  i < hi:
                                i += 1
                            intermediate_coordinates_list_1.insert(i, intersection.to_tuple())

                        # the same for the other line segment
                        for intersection in intersections_list:
                            intermediate_coordinates_list_2 = intermediate_sdb[street_2]
                            lo = intermediate_coordinates_list_2.index(p2.to_tuple())
                            hi = intermediate_coordinates_list_2.index(q2.to_tuple())
                            i = lo + 1
                            while dist(Point(intermediate_coordinates_list_2[lo]), Point(intermediate_coordinates_list_2[i])) <= \
                                dist(Point(intermediate_coordinates_list_2[lo]), intersection) and i < hi:
                                i += 1
                            intermediate_coordinates_list_2.insert(i, intersection.to_tuple())

    # based on intermediate database and intersections_set set
    # add edge to res
    for street in street_list:
        intermediate_coordinates_list = intermediate_sdb[street]
        for i in range(0, len(intermediate_coordinates_list)):
            # i is an intersect add edge <left, i> and <i, right> to the edge only if
            # left or right exist and 
            # left or right is not i
            if intermediate_coordinates_list[i] in intersections_set:
                intersection = intermediate_coordinates_list[i]
                intersection_id = res.get_vertex_id(intersection)
                if i > 0 and i < len(intermediate_coordinates_list) - 1:
                    left = intermediate_coordinates_list[i - 1]
                    right = intermediate_coordinates_list[i + 1]
                    if left != intersection:
                        left_id = res.get_vertex_id(left)
                        if left_id < intersection_id:
                            res.add_edge(left_id, intersection_id)
                        elif left_id > intersection_id:
                            res.add_edge(intersection_id, left_id)
                    if right != intersection:
                        right_id = res.get_vertex_id(right)
                        if intersection_id < right_id:
                            res.add_edge(intersection_id, right_id)
                        elif intersection_id > right_id:
                            res.add_edge(right_id, intersection_id)
                elif i == 0:  # no intersection left
                    right = intermediate_coordinates_list[i + 1]
                    if right != intersection:
                        right_id = res.get_vertex_id(right)
                        if intersection_id < right_id:
                            res.add_edge(intersection_id, right_id)
                        elif intersection_id > right_id:
                            res.add_edge(right_id, intersection_id)
                elif i == len(intermediate_coordinates_list) - 1:  # no intersection right
                    left = intermediate_coordinates_list[i - 1]
                    if left != intersection:
                        left_id = res.get_vertex_id(left)
                        if left_id < intersection_id:
                            res.add_edge(left_id, intersection_id)
                        elif left_id > intersection_id:
                            res.add_edge(intersection_id, left_id)

    return res

# The following are helpers

class Point(object):
    def __init__ (self, coordinate: tuple):
        self.x = float(coordinate[0])
        self.y = float(coordinate[1])
    def __str__ (self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'
    def to_tuple(self):
        return (self.x, self.y)

def intersect (p1: Point, q1: Point, p2: Point, q2: Point):
    x1, y1 = p1.x, p1.y
    x2, y2 = q1.x, q1.y
    x3, y3 = p2.x, p2.y
    x4, y4 = q2.x, q2.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor =  xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    return Point ((xcoor, ycoor))

def overlap_intersect(p1: Point, q1: Point, p2: Point, q2: Point):
    if max(p1.x, q1.x) >= max(p2.x, q2.x) and min(p1.x, q1.x) <= min(p2.x, q2.x):
        # seg 2 in seg 1
        return [p2, q2]
    elif max(p2.x, q2.x) > max(p1.x, q1.x) and min(p2.x, q2.x) < min(p1.x, q1.x):
        # seg 1 in seg 2
        return [p1, q1]
    elif max(p2.x, q2.x) > max(p1.x, q1.x) and max(p1.x, q1.x) > min(p2.x, q2.x):
        # seg 2 and seg 1 overlap
        res = []
        if q2.x > p2.x:  # get min in seg 2
            res.append(p2)
        else:
            res.append(q2)
        if q1.x > p1.x:  # get max in seg 1
            res.append(q1)
        else:
            res.append(p1)
        return res
    elif max(p1.x, q1.x) > max(p2.x, q2.x) and max(p2.x, q2.x) > min(p1.x, q1.x):
        # seg 2 and seg 1 overlap
        res = []
        if q1.x > p1.x:  # get min in seg 1
            res.append(p1)
        else:
            res.append(q1)
        if q2.x > p2.x:  # get max in seg 2
            res.append(p2)
        else:
            res.append(q2)
        return res
    elif max(p1.x, q1.x) == min(p2.x, q2.x):
        # seg 2 and seg 1 intersect on end-point
        if q1.x > p1.x:  # return the max in seg 1
            return  [q1]
        else:
            return [p1]
    elif max(p2.x, q2.x) == min(p1.x, q1.x):
        # seg 2 and seg 1 intersect on end-point
        if q2.x > p2.x:  # return the max in seg 2
            return [q2]
        else:
            return [p2]
    else:
        return []


def get_orientation(a: Point, b: Point, c: Point) -> int:
    """
    This function get the orientation of ordered points a(x_1, y_1), b(x_2, y_2), and c(x_3, y_3).
    The way to get orientation is to compute the slop of line segment <a, b> and line segment <b, c>
    Simplify the slop equation and avoiding zero division problem, the equation to compare is:
    (y_2 - y_1) * (x_3 - x_2) - (y_3 - y_2) * (x_2 - x_1) <, =, or > 0 ?
    =: encoded as 0, colinear(on the same line)
    >: encoded as 1, clock wise
    <: encoded as 2, counter clock wise
    """

    res = float(b.y - a.y) * float(c.x - b.x) - float(c.y - b.y) * float(b.x - a.x)

    if res == 0:
        return 0
    elif res > 0:
        return 1
    else: # res < 0
        return 2

def check_intersect(p1: Point, q1: Point, p2: Point, q2: Point) -> int:
    """
    This is the function that checks whether two line segments <p1, q1> and <p2, q2> intersect.
    Checking intersection of two line segments is important before trying to get the intersection.
    I have to know whether they intersect or not before computing their intersection vertex.

    I compare the orientation of the end-points of line segments to determine whether they are intersect.
    The correctness of this algorithm is explained on this website: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/.

    Basically, from my perspective, when two line segment have different intersection status(i.e. intersect regularly, intersect on extension, parallel, overlap, on the same line), the orientations of 3 of their end-point (i.e. (p1, q1, p2), (p1, q1, q2), (p2, q2, p1) and (p2, q2, q1)) are different. So this algorithm can utilize this feature to determine whether they are intersect or not.

    The following defines the return encoding and further action when receiving the value
    -1: not intersect.
    0: intersect by overlap, use special function to find intersection as there are 2 of them.
    1: intersect regularly, use the intersect function to find intersection.
    """
    o_p1_q1_p2 = get_orientation(p1, q1, p2)
    o_p1_q1_q2 = get_orientation(p1, q1, q2)
    o_p2_q2_p1 = get_orientation(p2, q2, p1)
    o_p2_q2_q1 = get_orientation(p2, q2, q1)

    if o_p1_q1_p2 != o_p1_q1_q2 and o_p2_q2_p1 != o_p2_q2_q1:  # intersect regularly (not overlap)
        return 1
    
    if o_p1_q1_p2 == 0 and o_p1_q1_q2 == 0 and o_p2_q2_p1 == 0 and o_p2_q2_q1 == 0:  # on the same line, need more comparison to determine intersect
        # compare the projection of x
        if min(p1.x, q1.x) > max(p2.x, q2.x) or min(p2.x, q2.x) > max(p1.x, q1.x):
            return -1  # if one segment smaller x is larger than the other segment larger x, then no intersection
        else:
            return 0
    
    return -1  # all other situations are not intersect

def dist(a: Point, b:Point):
    x1, y1 = a.x, a.y
    x2, y2 = b.x, b.y
    return (x1 - x2) ** 2 + (y1 - y2) ** 2

