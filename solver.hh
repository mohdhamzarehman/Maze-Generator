#ifndef SOLVER_H
#define SOLVER_H

#include "maze.hh"
#include <vector>

namespace SOLVER
{
      struct solution
      {
            std::vector<int> distance;
            std::vector<Maze::Position> parent;

            solution()=default;
            solution(std::vector<int> dist,std::vector<Maze::Position> parent):distance(dist),parent(parent){};
            solution(const solution& s){
            #ifndef NDEBUG
            std::cerr<<"\x1B[7;32m [LOG] \x1B[0m"<<__func__ << "Copy Constructor Invoked"<<std::endl;
            #endif
                  for(auto i:s.distance){
                        this->distance.push_back(i);
                  }
                  for(auto i:s.parent){
                        this->parent.push_back(i);
                  };
            #ifndef NDEBUG
            std::cerr<<"\x1B[7;32m [LOG]\x1B[0m"<<__func__<<" A copy with "<<this->distance.size()<<" created."<<std::endl;
            #endif
            }
            solution operator=(solution);
      };
      
      class Solver{
            public:
                  Solver(Maze::MazeGen &Maze);
                  void ShowSolution(char Mode,char AlgoIndex);
            private:
                  void Solve(solution (algo)(Maze::MazeGen&,Maze::Position&));
                  Maze::Position target;
                  Maze::MazeGen maze;
                  solution sol;

      };
      solution DisjkstrasAlgo(Maze::MazeGen&,Maze::Position &target);
      solution BellmanFord(Maze::MazeGen&,Maze::Position &target);
} // namespace Solver

#endif