#ifndef _PATHFIND_HPP
#define _PATHFIND_HPP

#include "structs.hpp"
#include "mapparse.hpp"

struct PathNode
{
    PathNode(){}
    PathNode(Point cell, PathNode* parent, float gCost, float hCost)
        : cell(cell), G(gCost), H(hCost), F(gCost + hCost), parent(parent)
    {}
    Point cell;
    float G{0};
    float H{0};
    float F{0};
    PathNode* parent;
};




class Pathfind
{
public:
    Pathfind(MapParse* mp);
    void buildGridPath();
    void setStart(const int x, const int y);
    void setGoal(const int x, const int y);
    std::vector<Point*> run();

    std::vector<Point*> debugPoints;

private:
    void calcNeighbours(int x, int y);
    bool blocked(Point cell);
    bool inVector(std::vector<PathNode*> vec, Point cell);
    static bool sortNodes(PathNode* n0, PathNode* n1);
    Point c2p(Point cell);

    MapParse* _mp;
    Point _start;
    Point _goal;
    std::vector<Segment> _wallSegments;
    std::vector<std::vector<bool>> _gridPath;
	bool neighbours[9];
    
    std::vector<Point*> _waypoints;
    std::vector<Point*> _finalPath;
    std::vector<PathNode*> _openList;
    std::vector<PathNode*> _closedList;
};

#endif
