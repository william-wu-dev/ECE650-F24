"""
This is the graph generator.

author: Yulin Wu
email: y297wu@uwaterloo.ca
"""

from Graph import Graph
from StreetDatabase import StreetDatabase

def graph_generator(sdb: StreetDatabase) -> Graph:
    # initialization
    # set of intersection
    # dictionary of added intersection street segment
    # result
    intersections = set()
    intermediate_sdb = dict()
    res = Graph()

    # double pointer iterate sdb find intersection

    return res

# The following are helpers

class Point(object):
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'

class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)

def intersect (l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor =  xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    return Point (xcoor, ycoor)

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



