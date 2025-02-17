/******************************************************************************
 * topological_sort.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef TOPOLOGICAL_SORT_GB9FC2CZ
#define TOPOLOGICAL_SORT_GB9FC2CZ

#include <stack>
#include <vector>

//#include "data_structure/graph_access.h"
//#include "definitions.h"

#include "lib/definitions.h"
#include "extern/KaHIP/lib/data_structure/graph_access.h"

class topological_sort {
public:
        topological_sort();
        virtual ~topological_sort();

        void sort( KaHIP::graph_access & SG, std::vector<NodeID> & sorted_sequence);

        void sort_dfs(NodeID node, KaHIP::graph_access & G, 
                      std::vector<int>    & dfsnum, 
                      int                 & dfscount,
                      std::vector<NodeID> & sorted_sequence);
};


#endif /* end of include guard: TOPOLOGICAL_SORT_GB9FC2CZ */
