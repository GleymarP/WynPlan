#ifndef NETGRAPH_H
#define NETGRAPH_H
#include<graph.hpp>
#include<vector>

template<typename NodeInfo = Designar::EmptyClass, typename ArcInfo = int, typename GraphInfo = Designar::EmptyClass>

class NetGraph : public Designar::Digraph<NodeInfo,ArcInfo, GraphInfo >
{
    using Base = Designar::Digraph<NodeInfo, ArcInfo, GraphInfo>;
    using Node = typename Base::Node;
    using Arc = typename Base::Arc;


public:
    using Base::Base;

private:

};

#endif // NETGRAPH_H
