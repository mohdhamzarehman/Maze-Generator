#ifndef MAZE_H
#define MAZE_H

#include <random>
#include <vector>
#include <iostream>
// #include <initializer_list>
#include <cstdint>

//std::string LogLevel[3] = {"\x1B[7;97;32m[LOG]\x1B[0m","\x1B[7;97;33m[WARNING]\x1B[0m","\x1B[7;97;31m[ERROR]\x1B[0m"};

namespace Maze
{
      struct Position
      {
            uint32_t x,y;
            Position() = default;
            inline Position(uint64_t x,uint64_t y){this->x=x;this->y=y;};            
            bool operator==(Position p2){ return (this->x==p2.x and this->y==p2.y);};
            bool operator!=(Position p2){ return !(*this==p2);};
            void operator=(std::initializer_list<int32_t>& p);
      };
      
      Maze::Position findMove(Maze::Position p,int x);
      std::ostream& operator<<(std::ostream&,Position);
      class MazeGen{
            uint64_t helper(uint64_t i) { return i; }

            public:
            //:-----------Constructors-----------
                  MazeGen(uint64_t size);
                  MazeGen(Position target_position,uint64_t size);
                  MazeGen():size(0),target(Position()) {};
                  MazeGen(const MazeGen& m)
                  :target(m.target),size((unsigned int)m.size)
                  {
                  std::cerr<<"\x1B[7;32m [LOG] \x1B[0m"<<__func__ << "Copy Constructor Invoked"<<std::endl;
                        this->_initMaze();
                        for(int i=0; i<this->size;i++){
                              for(int j=0; j<this->size;j++){
                                    this->_MAZE[i][j] = m._MAZE[i][j];
                              }
                        }
                  std::cerr<<"\x1B[7;32m [LOG] \x1B[0m"<<__func__ << "Copy Constructor Successfully Executed"<<std::endl;
                  
                  };
            //:-----------------------------------

            //:-----------Public Methods----------
                  void ShowMaze();
                  char& operator[](Position);
                  void createMaze();
                  char** GetMaze();
                  uint64_t GetSize() const;
                  Position GetTarget() const;
            //:-----------------------------------


            //:---------Operatiors----------------
            Maze::MazeGen& operator=(MazeGen m){
                  std::swap(size,m.size);
                  std::swap(target,m.target);
                  std::swap(_MAZE,m._MAZE);
                  return *this;
            };
            //:-----------------------------------

            private:
            //:----------Attrubutes---------------
                  uint64_t size;
                  Position target;
                  char **_MAZE;
            //:-----------------------------------

            //:---------Private Methods-----------
                  void _initMaze();
                  void makeMove(Position curr,Position new_);
            //:-----------------------------------


};
} // namespace Maze
#endif

