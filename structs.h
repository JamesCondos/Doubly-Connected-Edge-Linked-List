typedef struct Face face;
typedef struct Vertex vertex;
typedef struct Half_Edge half_edge;
typedef struct Edge edge;

typedef struct Watchtower_info {
    char *postcode;
    int pop_served;
    char *ID;
    char *name;
    double watchtower_x;
    double watchtower_y;

}   Watchtower;


struct Vertex {
    double x;
    double y;
};

struct Edge {
    half_edge *half_edge;
};

struct Face {
    half_edge *origin;
};

struct Half_Edge {
    half_edge *prev;
    half_edge *next;
    half_edge *twin;
    int start_vertex;
    int end_vertex;
    int face_index;    /* prev->left == left && next->left == left */
    int edge_index;
};


int right(Watchtower *towers, half_edge *h_edge, vertex **vertices);
int prepare_output(Watchtower *towers, half_edge *origin2, half_edge *traverse, vertex **vertices);
int read_csv(Watchtower **towers, FILE *fp);


