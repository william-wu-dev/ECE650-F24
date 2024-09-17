"""
This is the graph data structure for generating graph.

author: Yulin Wu
email: y297wu@uwaterloo.ca
"""

class Graph():
    def __init__(self) -> None:
        self._cnt = 0  # vertex id indicator; indicates the id for new vertex; self-increment;
        self._vertices = dict()  # <k: vertex coordinate, v: vertex id>
        self._edges = dict()  # <k: smaller vertex id, v: list of vertex id that form a qualified edge with key vertex id>
        # note: this graph is undirected, so <u,v> will be stored once if u-id < v-id

    def add_vertex(self, coordinate):
        # check if the coordinate is already in the vertices
        # FIXME: whether you should raise exception or just return id is depend on algorithm
        if coordinate in self._vertices:
            raise Exception(f'in Graph add vertex, {coordinate} already added.')
        self._vertices[coordinate] = self._cnt  # assign this new id
        res = self._cnt  # prepare for return
        self._cnt += 1  # self-increment indicator
        return res
    
    def add_edge(self, u_id, v_id):
        # check if id exists
        if u_id not in self._vertices.values():
            raise Exception(f'in Graph add edge, u-id {u_id} not found.')
        if v_id not in self._vertices.values():
            raise Exception(f'in Graph add edge, v-id {v_id} not found.')
        # check if u-id < v-id
        if not u_id < v_id:
            raise Exception(f'in Graph add edge, u-id {u_id} is not smaller than v-id {v_id}.')
        # check if edge already added, using shortcut circuit
        # FIXME: whether you should raise exception or just return id is depend on algorithm
        if u_id in self._edges and v_id in self._edges[u_id]:
            raise Exception(f'in Graph add edge, edge <{u_id}, {v_id}> already added.')
        
        # add this edge
        if u_id in self._edges:
            self._edges[u_id].append(v_id)
        else:
            self._edges[u_id] = [v_id, ]

    def __str__(self) -> str:
        # TODO: finish this print according to the output sample.
        return ''