/* qgraph.cpp
 * Daniel couch
 *
 * */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/tokenizer.hpp>  // used for input 

#define RUNTIME_ERROR -1




struct Vertex {
    int id;
};


typedef boost::adjacency_matrix<boost::directedS, Vertex> Network;    // undirected graph used for network
typedef boost::graph_traits<Network>::vertex_descriptor MyVertex;

const char *usage = "\nUSAGE:\nqgraph -f [file with adjacency matrix] -r [root node]\n\n";


/* read_adjacency_matrix
 *  Process adjacency matrix in text file,
 *  read line-by-line and add vertexes to Network.
 * */

Network read_adjacency_matrix(std::string file_name, int size);

class MyVisitor : public boost::default_dfs_visitor
{
    public:
        void discover_vertex(MyVertex v, const Network &n)
        {
            std::cerr << v << std::endl;
            return;
        }
};


Network read_adjacency_matrix(std::string file_name, int size)
{
    // initialize network w/ dimension of matrix
    Network NNetwork(size);
    std::ifstream infile(file_name.c_str());
    std::string line;
    int from_vertex = 0;
    while (std::getline(infile, line))
    {
        int to_vertex = 0;
        // tokenize line (row of adjacency matrix)
        boost::tokenizer<> tok(line);
        for (boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end(); ++beg, ++to_vertex)
        {
            int bit;
            // convert each string, 0 or 1, to an int.
            bit = atoi((*beg).c_str());
            if (bit == 1) boost::add_edge(from_vertex, to_vertex, NNetwork); // 1 => add to graph
        }
        from_vertex++;
    }
    return NNetwork;
}


int main (int argc, char *argv[])
{
    int root_node, size;
    std::string file_name;
    std::ifstream infile(file_name.c_str());
    std::string line;
    // increment size while there are lines to read.
    for (size = 0; std::getline(infile, line); ++size);
    double alpha, beta;
    // network parameters... not 100% sure what to do w/ this.
    alpha = 1; beta = 1;
    Network MyTree = read_adjacency_matrix(file_name, size);
    
    MyVisitor vis;
    boost::depth_first_search(MyTree, boost::visitor(vis));
    return 0;
}
