#include "pathfind.hpp"
#include <iostream>
#include "tools/tools.hpp"

Pathfind::Pathfind(MapParse* mp)
    : _mp(mp)
{
    for(Wall* wall : _mp->getWalls())
    {
        for(Segment* segment : wall->segments)
        {
            _wallSegments.push_back(*segment);
        }
    }

    buildGridPath();
}

void Pathfind::buildGridPath()
{
    _gridPath.clear();

    int y = 0;
    while(y < _mp->getGridSize().y)
    {
        std::vector<bool> xRow;
        for(int x = 0; x < (_mp->getGridSize().x + 1); x++)
        {
            bool blocked = false;
            Point grid( ((x+1) * _mp->getTileSize().x) - (_mp->getTileSize().x / 2), ((y+1) * _mp->getTileSize().y) - (_mp->getTileSize().y / 2));
            for(Wall* wall : _mp->getWalls())
            {
                if(wall->AABB.contains(grid.x, grid.y))
                {
                    blocked = true;
                    break;
                }
            }
            xRow.push_back(blocked);
        }
        _gridPath.push_back(xRow);
        y++;
    }    
}

void Pathfind::setStart(const int x, const int y)
{
    _start.x = (x / _mp->getTileSize().x) + 1;    
    _start.y = (y / _mp->getTileSize().y) + 1;    
}

void Pathfind::setGoal(const int x, const int y)
{
    _goal.x = (x / _mp->getTileSize().x) + 1;    
    _goal.y = (y / _mp->getTileSize().y) + 1;    
}

std::vector<Point*> Pathfind::run()
{
    for(auto& point : _waypoints)
        delete point;
    _waypoints.clear();

    for(auto& point : _finalPath)
        delete point;
    _finalPath.clear();

    if(blocked(_start) || blocked(_goal))
        return _finalPath;

    if(_goal == _start)
        return _finalPath;

    PathNode* current = new PathNode(_start, nullptr, 0.f, Tools::distance(_start, _goal));
    _openList.push_back(current);

    while(!_openList.empty())
    {
        std::sort(_openList.begin(), _openList.end(), sortNodes);
        current = _openList[0];

        if(current->cell == _goal){

            while(current->parent != nullptr)
            {
                _waypoints.push_back(new Point(current->cell.x, current->cell.y));
                current = current->parent;
            }

            for(auto& node : _openList)
                delete node;
            _openList.clear();

            for(auto& node : _closedList)
                delete node;
            _closedList.clear();

            Ray ray;
            Point intersection;
            bool found = false;

            ray.start = Point(c2p(_start));
            Point done = c2p(_goal);

            while(!found)
            {
                for(Point* wp : _waypoints)
                {
                    ray.end = Point(c2p(*wp));
                    bool blocked = false;

                    for(Segment segment : _wallSegments)
                    {
                        if(Tools::getIntersection(ray, &segment, intersection))
                        {
                            blocked = true;
                            break;
                        }
                    }

                    if(!blocked)
                    {
                        _finalPath.push_back(new Point(ray.end.x, ray.end.y));
                        ray.start = ray.end;
                        if(ray.start == done) found = true;
                        break;
                    }
                }
            }

            return _finalPath;
        }

        _openList.erase(_openList.begin());
        _closedList.push_back(current);

        int x = current->cell.x;
        int y = current->cell.y;
        calcNeighbours(x, y);

        for(int i =0; i < 9; i++)
        {
            if(neighbours[i] == false)
                continue;

            int xi = (i % 3) - 1;
            int yi = (i / 3) - 1;

            Point neighbour(x + xi, y + yi);
            float G = Tools::distance(current->cell, neighbour);
            float H = Tools::manhattenDistance(neighbour, _goal);
            
            if(inVector(_closedList, neighbour) && G >= current->G)
                continue;

            if(!inVector(_openList, neighbour))
            {
                PathNode* newNode = new PathNode(neighbour, current, G, H);
                _openList.push_back(newNode);
            }
            else
            {
                for(PathNode* node : _openList)
                {
                    if((node->cell == neighbour) && G + H < node->F)
                    {
                        node->G = G;
                        node->H = H;
                        node->parent = current;
                    }
                }
            }
        }
    }

    return _finalPath;
}

bool Pathfind::sortNodes(PathNode* n0, PathNode* n1)
{
        return (n0->F < n1->F);

} 

void Pathfind::calcNeighbours(int x, int y)
{
    for(int i = 0; i < 9; i++)
        neighbours[i] = true;

    neighbours[4] = false;

    for(int i = 0; i < 9; i++)
    {
        int xi = (i % 3) - 1;
        int yi = (i / 3) - 1;

        Point neighbour(x + xi, y + yi);

        if(blocked(neighbour))
            neighbours[i] = false;
    }

    if(neighbours[1] == false)
    {
        neighbours[0] = false;
        neighbours[2] = false;
    }

    if(neighbours[3] == false)
    {
        neighbours[0] = false;
        neighbours[6] = false;
    }

    if(neighbours[5] == false)
    {
        neighbours[2] = false;
        neighbours[8] = false;
    }

    if(neighbours[7] == false)
    {
        neighbours[6] = false;
        neighbours[8] = false;
    }
}

bool Pathfind::blocked(Point cell)
{
    if(cell.x < 1 || cell.x > _mp->getGridSize().x || cell.y < 1 || cell.y > _mp->getGridSize().y) 
        return true;

    return _gridPath[cell.y-1][cell.x-1];
}

bool Pathfind::inVector(std::vector<PathNode*> vec, Point cell)
{
    for(PathNode* p : vec)
    {
        if(p->cell.x == cell.x && p->cell.y == cell.y)
            return true;
    }
    return false;
} 

Point Pathfind::c2p(Point cell)
{
    return Point((cell.x * _mp->getTileSize().x) - (_mp->getTileSize().x / 2), (cell.y * _mp->getTileSize().y) - (_mp->getTileSize().y / 2));
}
