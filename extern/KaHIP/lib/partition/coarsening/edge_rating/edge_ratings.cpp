/******************************************************************************
 * edge_ratings.cpp 
 *****************************************************************************/

#include <math.h>

//#include "edge_ratings.h"
//#include "partition_config.h"       
//#include "random_functions.h"

#include "extern/KaHIP/lib/partition/coarsening/edge_rating/edge_ratings.h"
#include "extern/KaHIP/lib/partition/partition_config.h"
#include "extern/KaHIP/lib/tools/random_functions.h"

edge_ratings::edge_ratings(const KaHIP::PartitionConfig & _partition_config) : partition_config(_partition_config){

}

edge_ratings::~edge_ratings() {

}

void edge_ratings::rate(KaHIP::graph_access & G, unsigned level) {
        //rate the edges
        if(level == 0 && partition_config.first_level_random_matching) {
                return;
        } else if(partition_config.matching_type == MATCHING_RANDOM_GPA && level < partition_config.aggressive_random_levels) {
                return;
        } 
        if(level == 0 && partition_config.rate_first_level_inner_outer && 
                         partition_config.edge_rating != EXPANSIONSTAR2ALGDIST ) {

                        rate_inner_outer(G);

        } else if(partition_config.matching_type != MATCHING_RANDOM) {
                switch(partition_config.edge_rating) {
                        case EXPANSIONSTAR:
                                rate_expansion_star(G);
                                break;
                        case PSEUDOGEOM:
                                rate_pseudogeom(G);
                                break;
                        case EXPANSIONSTAR2:
                                rate_expansion_star_2(G);
                                break;
                        case EXPANSIONSTAR2ALGDIST:
                                rate_expansion_star_2_algdist(G);
                                break;
                        case WEIGHT:
                                break;
                        case REALWEIGHT:
                                break;
                        case SEPARATOR_MULTX:
                                rate_separator_multx(G);
                                break;
                        case SEPARATOR_ADDX:
                                rate_separator_addx(G);
                                break;
                        case SEPARATOR_MAX:
                                rate_separator_max(G);
                                break;
                        case SEPARATOR_LOG:
                                rate_separator_log(G);
                                break;
                        case SEPARATOR_R1:
                                rate_separator_r1(G);
                                break;

                        case SEPARATOR_R2:
                                rate_separator_r2(G);
                                break;

                        case SEPARATOR_R3:
                                rate_separator_r3(G);
                                break;

                        case SEPARATOR_R4:
                                rate_separator_r4(G);
                                break;

                        case SEPARATOR_R5:
                                rate_separator_r5(G);
                                break;

                        case SEPARATOR_R6:
                                rate_separator_r6(G);
                                break;

                        case SEPARATOR_R7:
                                rate_separator_r7(G);
                                break;

                        case SEPARATOR_R8:
                                rate_separator_r8(G);
                                break;


                }
        }
}

//simd implementation is possible
void edge_ratings::compute_algdist(KaHIP::graph_access & G, std::vector<float> & dist) {
        for( unsigned R = 0; R < 3; R++) {
                std::vector<float> prev(G.number_of_nodes(), 0);
                forall_nodes(G, node) {
                        prev[node] = KaHIP::random_functions::nextDouble(-0.5,0.5); 
                } endfor

                std::vector<float> next(G.number_of_nodes(), 0);
                float w = 0.5;

                for( unsigned k = 0; k < 7; k++) {
                        forall_nodes(G, node) {
                                next[node] = 0;

                                forall_out_edges(G, e, node) {
                                        NodeID target = G.getEdgeTarget(e);
                                        next[node] += prev[target] * G.getEdgeWeight(e);
                                } endfor

                                float wdegree = G.getWeightedNodeDegree(node);
                                if(wdegree > 0) {
                                        next[node] /= (float)wdegree;

                                }
                        } endfor

                        forall_nodes(G, node) {
                                prev[node] = (1-w)*prev[node] + w*next[node];
                        } endfor

                }

                forall_nodes(G, node) {
                        forall_out_edges(G, e, node) {
                                NodeID target = G.getEdgeTarget(e);
                                //dist[e] = max(dist[e],fabs(prev[node] - prev[target]));
                                dist[e] += fabs(prev[node] - prev[target]) / 7.0;
                        } endfor
                } endfor
        }

        forall_nodes(G, node) {
                forall_out_edges(G, e, node) {
                        dist[e] += 0.0001;
                } endfor
        } endfor

}


void edge_ratings::rate_expansion_star_2_algdist(KaHIP::graph_access & G) {

        std::vector<float> dist(G.number_of_edges(), 0);
        compute_algdist(G, dist);

        forall_nodes(G,n) {
                NodeWeight sourceWeight = G.getNodeWeight(n);
                forall_out_edges(G, e, n) {
                        NodeID targetNode = G.getEdgeTarget(e);
                        NodeWeight targetWeight = G.getNodeWeight(targetNode);
                        EdgeWeight edgeWeight = G.getEdgeWeight(e);

                        EdgeRatingType rating = 1.0*edgeWeight*edgeWeight / (targetWeight*sourceWeight*dist[e]);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}


void edge_ratings::rate_expansion_star_2(KaHIP::graph_access & G) {
        forall_nodes(G,n) {
                NodeWeight sourceWeight = G.getNodeWeight(n);
                forall_out_edges(G, e, n) {
                        NodeID targetNode = G.getEdgeTarget(e);
                        NodeWeight targetWeight = G.getNodeWeight(targetNode);
                        EdgeWeight edgeWeight = G.getEdgeWeight(e);

                        EdgeRatingType rating = 1.0*edgeWeight*edgeWeight / (targetWeight*sourceWeight);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_inner_outer(KaHIP::graph_access & G) {
        forall_nodes(G,n) {
#ifndef WALSHAWMH
                EdgeWeight sourceDegree = G.getWeightedNodeDegree(n);
#else
                EdgeWeight sourceDegree = G.getNodeDegree(n);
#endif
                if(sourceDegree == 0) continue;

                forall_out_edges(G, e, n) {
                        NodeID targetNode = G.getEdgeTarget(e);
#ifndef WALSHAWMH
                        EdgeWeight targetDegree = G.getWeightedNodeDegree(targetNode);
#else
                        EdgeWeight targetDegree = G.getNodeDegree(targetNode);
#endif
                        EdgeWeight edgeWeight = G.getEdgeWeight(e);
                        EdgeRatingType rating = 1.0*edgeWeight/(sourceDegree+targetDegree - edgeWeight);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_expansion_star(KaHIP::graph_access & G) {
        forall_nodes(G,n) {
                NodeWeight sourceWeight = G.getNodeWeight(n);
                forall_out_edges(G, e, n) {
                        NodeID targetNode       = G.getEdgeTarget(e);
                        NodeWeight targetWeight = G.getNodeWeight(targetNode);
                        EdgeWeight edgeWeight   = G.getEdgeWeight(e);

                        EdgeRatingType rating = 1.0 * edgeWeight / (targetWeight*sourceWeight);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_pseudogeom(KaHIP::graph_access & G) {
        forall_nodes(G,n) {
                NodeWeight sourceWeight = G.getNodeWeight(n);
                forall_out_edges(G, e, n) {
                        NodeID targetNode       = G.getEdgeTarget(e);
                        NodeWeight targetWeight = G.getNodeWeight(targetNode);
                        EdgeWeight edgeWeight   = G.getEdgeWeight(e);
                        double random_term      = KaHIP::random_functions::nextDouble(0.6,1.0);
                        EdgeRatingType rating   = random_term * edgeWeight * (1.0/(double)sqrt((double)targetWeight) + 1.0/(double)sqrt((double)sourceWeight));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_separator_addx(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  1.0 / (G.getNodeDegree(node) + G.getNodeDegree(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor

}

void edge_ratings::rate_separator_multx(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  pow( G.getNodeDegree(node) * G.getNodeDegree(target), -0.5);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor

}

void edge_ratings::rate_separator_max(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating = 1.0/std::max(G.getNodeDegree(node),G.getNodeDegree(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_separator_log(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating = 1.0/log(G.getNodeDegree(node)*G.getNodeDegree(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}


void edge_ratings::rate_separator_r1(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  1.0/(G.getNodeDegree(node) * G.getNodeDegree(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_separator_r2(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  1.0/(G.getNodeDegree(node) * G.getNodeDegree(target)*G.getNodeWeight(node)*G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_separator_r3(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  1.0/(G.getNodeDegree(node) + G.getNodeDegree(target)+G.getNodeWeight(node)+G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor

}

void edge_ratings::rate_separator_r4(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  ((EdgeRatingType)G.getNodeDegree(node) * G.getNodeDegree(target))/(G.getNodeWeight(node)*G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor

}

void edge_ratings::rate_separator_r5(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  ((EdgeRatingType)G.getNodeDegree(node) + G.getNodeDegree(target))/(G.getNodeWeight(node)+G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor


}

void edge_ratings::rate_separator_r6(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  1.0/((G.getNodeDegree(node) + G.getNodeDegree(target))*(G.getNodeWeight(node)+G.getNodeWeight(target)));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor

}

void edge_ratings::rate_separator_r7(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  G.getEdgeWeight(e)*1.0/(G.getNodeDegree(node) * G.getNodeDegree(target)*G.getNodeWeight(node)*G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

void edge_ratings::rate_realweight(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        EdgeRatingType rating =  G.getEdgeWeight(e);
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}
void edge_ratings::rate_separator_r8(KaHIP::graph_access & G) {
        forall_nodes(G,node) {
                forall_out_edges(G, e, node) {
                        NodeID target = G.getEdgeTarget(e);

                        EdgeRatingType rating =  G.getEdgeWeight(e)*1.0*(G.getNodeDegree(node) * G.getNodeDegree(target))/(G.getNodeWeight(node)*G.getNodeWeight(target));
                        G.setEdgeRating(e, rating);
                } endfor
        } endfor
}

