//BLENDER.H
#define MAXS   80	// size of filename string
#define LEN_GRASSHEADER 12
#define LEN_ARCHEADER	12 

#define MIN_RISE 0.001
#define DtoR	    0.01745329 

// STRUCTURE DEFINITIONS
struct ID_struct {
	int	patch;
};

struct	adj_struct {
	int	 patchID;
	int  streamID;
	int  landID;
	int	 inx;
	double perimeter;
	double slope;
	double gamma;
	double z;
	struct adj_struct *next;
};

struct	flow_struct	{
	int	patchID;
	int	ID_order;
	int area;
	int land;
	double x;
	double y;
	double z;
	double flna;
	double total_gamma;
	double acc_area;
	double road_dist;
	double slope;
	double internal_slope;
	double path_length;
	int	   inflow_cnt;
	int    num_adjacent;
	int	   stream_inx;
	double stream_gamma;
	struct ID_struct stream_ID;
	struct adj_struct *adj_list;
	struct adj_struct *adj_ptr;
};
