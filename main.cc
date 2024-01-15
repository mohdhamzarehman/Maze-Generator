#include "maze.hh"
#include "solver.hh"
#include <cstdlib>
#include <iostream>

using Maze::MazeGen;

//"\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"
// "Green", "32"
// "Yellow", "33"
// "Red", "31"


int main(int argc, char const *argv[])
{
      int size=21;
      char mode = '1',AlgoIndex = '1';
      auto tarFlag = false;
      Maze::Position targt;
      for(int i=0;i<argc-1;i++){
            // std::cout<<((std::string)(argv[i])=="--size")<<std::endl;
            if((std::string)argv[i]=="--size" or (std::string)argv[i]=="-s"){
                  auto arg = argv[i+1];
            #ifndef NDEBUG
                  std::cerr<<"\x1B[7;32m[LOG] \x1B[0m Setting Size to :"<<arg<<std::endl;
            #endif
                  try{
                        size = atoi(arg);
                  }catch(...){
                        std::cout<<"[ERROR] Unexpected Error"<<std::endl;
                  }
            }if((std::string)argv[i]=="--mode" or (std::string)argv[i]=="-m"){
                  auto arg2 = argv[i+1];
            #ifndef NDEBUG
                  std::cerr<<"\x1B[7;32m[LOG] \x1B[0m Setting Mode to :"<<arg2<<std::endl;
            #endif
            }if((std::string)argv[i]=="--algo" or (std::string)argv[i]=="-a"){
                  auto arg3 = argv[i+1];
            #ifndef NDEBUG
                  std::cerr<<"\x1B[7;32m[LOG] \x1B[0m Setting AlgoIndex to :"<<arg3<<std::endl;
            #endif
            }if((std::string)argv[i]=="--target" or (std::string)argv[i]=="-t"){
                  tarFlag = true;
                  auto x = atoi(argv[i+1]);
                  auto y = atoi(argv[i+2]);

                  targt.x = x;
                  targt.y = y;
                  std::cerr<<"\x1B[7;32m[LOG] \x1B[0m Setting target to :"<<targt<<std::endl;
            }
      }
      if (tarFlag){
            MazeGen m(targt,size);
            srand(time(0));
            m.createMaze();
            m.ShowMaze();
      // std::cerr<<"\x1B[7;32m[LOG]\x1B[0m Bellman Ford Executed Successfully"<<std::endl;
            SOLVER::Solver sol(m);
            sol.ShowSolution(mode,AlgoIndex);
      }else{
            MazeGen m(size);
            srand(time(0));
            m.createMaze();
            m.ShowMaze();
      // std::cerr<<"\x1B[7;32m[LOG]\x1B[0m Bellman Ford Executed Successfully"<<std::endl;
            SOLVER::Solver sol(m);
            sol.ShowSolution(mode,AlgoIndex);
      }
      return 0;

}
