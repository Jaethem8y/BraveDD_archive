#include "../src/brave_dd.h"

#include <random>
#include <iostream>
#include <cstdint>

using namespace BRAVE_DD;

Edge buildSetEdge(Forest* forest,
                uint16_t lvl)
{
    std::vector<Edge> child(2);
    EdgeLabel label = 0;
    packRule(label, RULE_X);
    child[0].setEdgeHandle(makeTerminal(INT, 0));
    child[1].setEdgeHandle(makeTerminal(INT, 1));
    child[0].setRule(RULE_X); 
    child[1].setRule(RULE_X);
    Edge edge =  forest->reduceEdge(1, label, 1, child);
    std::vector<Edge> child1(2);
    std::vector<Edge> child2(2);
    child1[0] = edge;
    child2[0] = edge;
    edge.setSwap(1, 0);
    child1[1] = edge;
    child2[1] = edge;
    
    packRule(label, RULE_00);
    std::vector<Edge> child3(2);
    child3[0] = forest->reduceEdge(7, label, 2, child1);
    packRule(label, RULE_11);
    child3[1] = forest->reduceEdge(7, label, 2, child2);
    packRule(label, RULE_X);
    return forest->reduceEdge(7, label, 7, child3);
}


bool buildSetForest(uint16_t num, PredefForest bdd)
{
    // test up to 5 variables
    ForestSetting setting(bdd, num);
    Forest* forest = new Forest(setting);
    forest->getSetting().output(std::cout);
    bool ans = 0;
    // variables
    std::vector<std::vector<bool> > vars(1<<num, std::vector<bool>(num+1, false));
    for (int i=0; i<(1<<num); i++) {
        for (uint16_t k=1; k<=num; k++){
            vars[i][k] = i & (1<<(k-1));
        }
    }
    // function values
    Func function(forest);
    // build function
    Edge edge = buildSetEdge(forest, num);
    function.setEdge(edge);

    ForestSetting setting1(PredefForest::REXBDD, 7);
    Forest* forest1 = new Forest(setting1);
    Func temp(forest1);

    apply(COPY, function, temp);

    DotMaker dot(forest, "1");
    dot.buildGraph(function);
    dot.runDot("pdf");

    DotMaker dot1(forest1, "2");
    dot1.buildGraph(temp);
    dot1.runDot("pdf");
    return 0;
}


int main(int argc, char** argv)
{
    uint16_t num = 7;
    PredefForest bdd = PredefForest::ROAR;

    std::cout<< "ReduceEdge test." << std::endl;

    ForestSetting setting(bdd, num);
    Forest* forest = new Forest(setting);
    forest->getSetting().output(std::cout);
    delete forest;

    buildSetForest(num, bdd);
    return 0;
}
