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

    def add_vertex(self, coordinate: tuple):
        # check if the coordinate is already in the vertices
        # FIX: change this logic, duplicate add will not raise error
        if coordinate in self._vertices:
            return self._vertices[coordinate]
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
        # FIX: if already added, nothing will happen
        if u_id in self._edges and v_id in self._edges[u_id]:
            return 
        
        # add this edge
        if u_id in self._edges:
            self._edges[u_id].append(v_id)
        else:
            self._edges[u_id] = [v_id, ]

    def check_vertex(self, coordinate: tuple):
        return coordinate in self._vertices
    
    def get_vertex_id(self, coordinate: tuple):
        if coordinate not in self._vertices:
            raise Exception(f'in Graph get vertex id, {coordinate} not exist')
        return self._vertices[coordinate]
    
    def check_edge(self, u_id, v_id):
        # check if id exists
        if u_id not in self._vertices.values():
            raise Exception(f'in Graph check edge, u-id {u_id} not found.')
        if v_id not in self._vertices.values():
            raise Exception(f'in Graph check edge, v-id {v_id} not found.')
        # check if u-id < v-id
        if not u_id < v_id:
            raise Exception(f'in Graph check edge, u-id {u_id} is not smaller than v-id {v_id}.')
        return u_id in self._edges and v_id in self._edges[u_id]

    def __str__(self) -> str:
        # TODO: finish this print according to the output sample.
        # sort vertices based on the id
        vertices_sorted = {k: v for k, v in sorted(self._vertices.items(), key= lambda item: item[1])}
        # sort edge based on u_id
        edges_sorted = {k: v for k, v in sorted(self._edges.items(), key=lambda item: item[0])}
        # print vertices first
        res = 'V = {'
        for coordinate, id in vertices_sorted.items():
            res += '\n'
            res += '  '
            res += str(id)
            res += ': '
            res += f'({coordinate[0]:.2f},{coordinate[1]:.2f})'
        res += '\n'
        res += '}'

        res += '\n'

        # print edges
        res += 'E = {'
        flag = False
        for u_id, v_id_list in edges_sorted.items():
            for v_id in v_id_list:
                res += '\n'
                res += '  '
                res += f'<{u_id},{v_id}>,'
                if not flag:
                    flag = True
        if flag:
            res = res[:-1] # get rid of trailing comma if printed
        res += '\n'
        res += '}'
        return res
