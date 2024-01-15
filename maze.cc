#include "maze.hh"
#include <random>
#include <vector>
#include <cassert>
#include <iostream>
#include <exception>
// #include <initializer_list>
#include <cstdint>

#define log(level,message) std::cerr<<LogLevel[level-1]<<" From "<<__func__<<message<<std::endl;
std::string LogLevel[3] = {"\x1B[7;97;32m[LOG]\x1B[0m","\x1B[7;97;33m[WARNING]\x1B[0m","\x1B[7;97;31m[ERROR]\x1B[0m"};

#define asterisk "\x1B[7;30m*\x1B[0m"
#define space "\x1B[7;97m \x1B[0m"
#define targ "\x1B[0;34mX\x1B[0m"
using Maze::MazeGen;
;
//;=================== ~MazeGen()  =================
// MazeGen::~MazeGen(){
//       for(auto i=0;i<this->size; i++){
//             delete[] _MAZE[i];
//       }
//       delete[] _MAZE;
// }

//;=================== MazeGen(int)==================
MazeGen::MazeGen(const uint64_t size):size(size),target({(uint64_t)((rand()%this->size)+1),(uint64_t)((rand()%this->size)+1)}){
#ifndef NDEBUG
      log(2,"Initalizing with default settings")
      log(1," initialized with Paramerters: size->"<<size<<",target->"<<target)
#endif
      this->_initMaze();

#ifndef NDEBUG
      log(1," executed successfully")
#endif
}


//;================= MazeGen(Position,int)===========
MazeGen::MazeGen(Maze::Position pos, const uint64_t size):size(size),target(pos){

#ifndef NDEBUG
      log(1," initiated with parameters-> size:"<<this->size<<", Position ->"<<pos)
#endif
      
      if(pos.x<0 or pos.x>=this->size or pos.y<0 or pos.y>=this->size){
            log(3," Invalid Postion given"<<pos)
            exit(1);
      }

      this->_initMaze();

#ifndef NDEBUG
      log(1," executed successfully")
#endif
}

//;================ _initMaze()===================
void MazeGen::_initMaze(){
#ifndef NDEBUG
      log(1," initiated with parameters->size:"<<this->size<<", target:"<<target)
#endif
#ifndef NDEBUG
      log(1," allocating memory for Maze")
#endif
      try{
      this->_MAZE = new char*[this->size];
      }catch(std::bad_alloc& ba){
      log(3,":Failed to allocate Memory for maze.\n"<<ba.what()<<"\nFile:"<<__FILE__<<"Line:"<<__LINE__)
      }
#ifndef NDEBUG
      log(1," memory allocated successfully")
#endif
      for(uint64_t i =0; i<this->size; i++){
            this->_MAZE[i] = new char[this->size];
      }

      for(uint64_t i=0; i<this->size; i++){
            for(uint64_t j=0; j<this->size; j++){
                  this->_MAZE[i][j]='*';
            }
      }

      this->_MAZE[this->target.x][this->target.y] = 'X';
#ifndef NDEBUG
      log(1," executed successfully")
#endif
}


//;=============== ShowMaze()====================
void MazeGen::ShowMaze(){
#ifndef NDEBUG
      log(1," Initiated")
#endif
      for(int i=0;i<this->size; i++){
            for(int j=0; j<this->size; j++){
                  if(i==0 or i ==this->size-1 or j==0 or j == this->size-1){
                        std::cout<<"\x1B[7;90m \x1B[0m"<<"\x1B[7;90m \x1B[0m";
                  }else if(this->_MAZE[i][j]=='*'){
                        std::cout<<asterisk<<asterisk;
                  }else if(this->_MAZE[i][j]==' '){
                        std::cout<<space<<space;
                  }else if(this->_MAZE[i][j]=='X'){
                        std::cout<<targ<<space;
                  }
                  // std::cout<<"\x1B[7;30m  \x1B[0m";
            }
            std::cout<<std::endl;
      }
#ifndef NDEBUG
      log(1," executed successfully")
#endif
}


//;============= createMaze()===================
void MazeGen::createMaze(){

      std::default_random_engine generator(time(0));
#ifndef NDEBUG
      log(1," Starting to create maze...")
#endif

      try{
      Position pos[] = {{1,1},{1,this->size-2},{this->size-2,1},{this->size-2,this->size-2}};
      for(ushort i=0; i<4;i++){

            auto curr = pos[i];
            unsigned short moveDecider = 0;
            double moveCounter = (this->size*this->size)/3;
            double moveWeightage[4] = {moveCounter,moveCounter,moveCounter,moveCounter};
            while(moveCounter){

                  int whereToMove[4];
                  std::discrete_distribution<int> distribution {moveWeightage[0]/3,moveWeightage[1],moveWeightage[2]/3,moveWeightage[3]};
                  for(int i=0;i<4;i++)
                        whereToMove[i] = distribution(generator);

                  auto newPos = findMove(curr,whereToMove[moveDecider]);
                  if(0<newPos.x and newPos.x<this->size-1 and 0<newPos.y and newPos.y<this->size-1){
                        if(this->operator[](newPos)==' ' and moveDecider!=3) {moveDecider++;continue;}
                        this->makeMove(curr,newPos);
                        moveWeightage[whereToMove[moveDecider]]--;
                        moveCounter--;
                        curr = newPos;
                        moveDecider=rand()%4;
                        #ifndef NVERBOSE
                        std::cout<<"Current Position:"<<curr<<std::endl;
                        #endif
                  }
                  if (curr==target) continue;
            }
      }
      }catch(...){
            log(3,"Unexpected Error Occured at File:"<<__FILE__<<"\nLine:"<<__LINE__)
      }
      this->operator[](target)='X';
#ifndef NDEBUG
log(1," Maze Created succesfully")
#endif
}


//;============== moveCursor(Position,int)======
Maze::Position Maze::findMove(Maze::Position p,int x){
/*
! Possible value of x
: 0== UP
: 1== DOWN
: 2== LEFT
: 3== RIGHT
*/
#ifndef NVERBOSE
log(1,"[VERBOSE] Initialized with parameters: position->"<<p<<", x->"<<x)
#endif
if(x==0){
      p.y+=2;
}else if(x==1){
      p.y-=2;
}else if(x==2){
      p.x-=2;
}else if(x==3){
      p.x+=2;
}else{
      std::cerr<<"[ERROR] From "<<__func__<<" Invalid Move {"<<x<<"}. Must be in Range 0<=x<=3."<<std::endl;
      exit(1);
}
#ifndef NVERBOSE
log(1,"[VERBOSE] Executed Successfully.")
#endif
return p;
}
//;====================== GetMaze()========================

char** MazeGen::GetMaze(){
      return this->_MAZE;
}


//;====================== GetSize()========================
uint64_t MazeGen::GetSize() const {
      return const_cast<uint64_t&>(this->size);
}

//;====================== GetTarget()========================
Maze::Position MazeGen::GetTarget() const {
      return this->target;
}

//;====================== MazeGen::operator[]==============
char &MazeGen::operator[](Maze::Position p){
      auto &ref = (this->_MAZE[p.x][p.y]);
      return ref;
}


//;====================== MazeGen::operator<< ==============
std::ostream& Maze::operator<<(std::ostream& os,Maze::Position p){
      os<<'{'<<p.x<<','<<p.y<<'}';
      return os;
}

//;===================== MazeGen::makeMove(Position,Position)==
void MazeGen::makeMove(Maze::Position curr,Maze::Position new_){

#ifndef NVERBOSE
log(1,"[VERBOSE] initialized with parameters: curr->"<<curr<<" new_->"<<new_)
#endif 
      if(!(curr.x-new_.x)){
            auto start = std::min(curr.y,new_.y),
                   end = std::max(curr.y,new_.y);
            for(int i=start;i<=end;i++){
                  this->_MAZE[curr.x][i]=' ';
            }
      }else if(!(curr.y-new_.y)){
            auto start = std::min(curr.x,new_.x),
                   end = std::max(curr.x,new_.x);
            for(int i=start;i<=end;i++){
                  this->_MAZE[i][curr.y]=' ';
            }
      }else{
            std::cerr<<"[ERROR]: From "<<__func__<<" Invalid move"<<new_<<". \nMove must be in 1 direction."<<std::endl;
            exit(1);
      }
#ifndef NVERBOSE
log(1,"[VERBOSE] From executed successfully.")
#endif
}

//;================= Position::operator=(initializer_list<Position>&)============

void Maze::Position::operator=(std::initializer_list<int32_t>& p){
      std::vector<int> v;
      for(auto &i:p){
            v.push_back(i);
      }
      x = v[0];
      y = v[1];
}
