/******************************************************************************
 * mapping_algorithms.h
 *
 * Source of KaHIP -- Karlsruhe High Quality Graph Partitioning 
 *****************************************************************************/

#ifndef MAPPING_ALGORITHMS_W4I4JZHS
#define MAPPING_ALGORITHMS_W4I4JZHS

//#include "data_structure/graph_access.h"
//#include "data_structure/matrix/normal_matrix.h"
//#include "partition_config.h"
//#include "tools/quality_metrics.h"
//#include "tools/timer.h"

#include "extern/KaHIP/lib/data_structure/graph_access.h"
#include "extern/KaHIP/lib/data_structure/matrix/normal_matrix.h"
#include "extern/KaHIP/lib/partition/partition_config.h"
#include "extern/KaHIP/lib/tools/quality_metrics.h"
#include "lib/tools/timer.h"


class mapping_algorithms {
public:
        mapping_algorithms();
        virtual ~mapping_algorithms();

        void construct_a_mapping( KaHIP::PartitionConfig & config, KaHIP::graph_access & C, matrix & D, std::vector< NodeID > & perm_rank);
        void graph_to_matrix( KaHIP::graph_access & C, matrix & C_bar);

private:
        quality_metrics qm; 
        timer t;
};


#endif /* end of include guard: MAPPING_ALGORITHMS_W4I4JZHS */
