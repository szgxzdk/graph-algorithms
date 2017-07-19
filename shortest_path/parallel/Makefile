all: shortest_path_parallel

shortest_path_parallel: shortest_path.cc graph.h graph.cc
	g++ -o shortest_path_parallel shortest_path.cc graph.cc -lpthread
