/*
 *  Daniel Couch
 *  qgraph.cpp
 *  Generate system of equations in matrix representation
 *  for nerve impulse PDE using Boost library. 
 *
 * */
#include <iostream>
#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/tokenizer.hpp>

#define SUCCESS 0

using namespace boost;
const char *usage = "qgraph adjacency_matrix";


/* Since we don't need to weight the edges, we can just
 * replace edge weight with an identifier */
typedef boost::property<boost::edge_weight_t, int> EigenfunctionID;
/* Network: bidirectional graph with representation of an adjacency list */
typedef boost::adjacency_list<listS, vecS, bidirectionalS, boost::no_property, EigenfunctionID> Network;
typedef Network::edge_descriptor Edge;
typedef boost::numeric::ublas::matrix<std::string> SMat;
/* Prototypes */
Network process_input(const char *file_name);
SMat iterate_network(Network Net);


/* Function process_input
 *  Read in .txt file containing
 *  adjacency matrix and store
 *  the data in the Network. 
 *  Returns the Network object
 *  */
Network process_input(const char *file_name)
{
    /* Get the size of the adjacency matrix */
    int size;
    std::ifstream infile(file_name);
    std::string line;
    for (size = 0; std::getline(infile, line); ++size);
    Network Net = Network(size);
    /* Clear EOF, go back to beginning of file */
    infile.clear();
    infile.seekg(0, std::ios::beg);
    int id = 1;
    /* First vertex in edge */
    int from_vertex = 0;
    /* Iterate over the rows; when we come to
     * the end of a row, we are then at the next
     * from_vertex */
    while (std::getline(infile, line))
    {
        int to_vertex = 0;
        /* Tokenize line */
        boost::tokenizer<> tok(line);
        /* Iterate over the tokenized line
         * (over elements of matrix row) */
        for (boost::tokenizer<>::iterator tokit=tok.begin();
                tokit!=tok.end();
                ++tokit, ++to_vertex)
        {
            int bit;
            /* Convert each string, 0 or 1, to an int */
            bit = atoi((*tokit).c_str());
            /* If 1, then there's an edge between corresponding vertices */
            if (bit == 1)
            {
                boost::add_edge(from_vertex, to_vertex, id, Net);
                id++;
            }
        }
        from_vertex++;
    }
    return Net;
}

/* Function insert_element
 *  Process current edge,
 *  add appropriate elements
 *  to A and B matrix 
 *  */
void insert_element(Network Net, Edge edge, SMat AMat, SMat BMat)
{
    std::cout << edge << " " << "\n";
    int eigenid = get(boost::edge_weight_t(), Net, edge);

}

/* Function iterate_network
 *  Iterate over the edges of the
 *  network and do whatever is to be done
 *  Returns matrix w/ sys of equation
 */
SMat iterate_network(Network Net)
{
    int dim = boost::num_edges(Net);
    /* Output matrix contains both A and B values,
     * which we can store in two separate matrices*/
    SMat AMat (dim, dim);
    SMat BMat (dim, dim);
    SMat outputMat (2*dim, 2*dim);
    Network::vertex_iterator vertexIt, vertexEnd;
    Network::in_edge_iterator inedgeIt, inedgeEnd;
    Network::in_edge_iterator edgeIt, edgeEnd;
    boost::tie(vertexIt, vertexEnd) = boost::vertices(Net);
    for (; vertexIt != vertexEnd; ++vertexIt)
    {
        std::cout << "INCOMING FOR " << *vertexIt << ": ";
        /* in_edges provides iterators to iterate over incoming edges of 
         * the vertex referred to by vertexIt */
        boost::tie(inedgeIt, inedgeEnd) = boost::in_edges(*vertexIt, Net);
        for (; inedgeIt != inedgeEnd; ++inedgeIt)
        {
            /* We're at an edge, a tuple of vertices (V_out, V_in),
             * which is obtained by dereferencing the iterator */
            insert_element(Net, *inedgeIt, AMat, BMat);
        }
        std::cout << "\n";
    }
    return outputMat;
}


int main(int argc, char *argv[])
{
    if (argc < 2) std::cout << usage << "\n";
    char *file_name = argv[1];
    Network MyNet = process_input(file_name);
    SMat OutMatrix = iterate_network(MyNet);
    return SUCCESS;
}
