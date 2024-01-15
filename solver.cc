#include "solver.hh"
#include "maze.hh"
#include <exception>
#include <algorithm>
#include <queue>
using namespace SOLVER;

std::string LOGLevel[3] = {"\x1B[7;97;32m[LOG]\x1B[0m","\x1B[7;97;33m[WARNING]\x1B[0m","\x1B[7;97;31m[ERROR]\x1B[0m"};
#define LOG(level,message) std::cerr<<LOGLevel[level-1]<<" From "<<__func__<<message<<std::endl;

#define asterisk "\x1B[7;30m*\x1B[0m"
#define space "\x1B[7;97m \x1B[0m"
#define targ "\x1B[0;34mX\x1B[0m"
#define path "\x1B[7;95m \x1B[0m"


//;===================BellmanFord(MazeGen,Position)========
solution SOLVER::BellmanFord(Maze::MazeGen &m,Maze::Position &target){
      std::cout<<m.GetSize()<<std::endl;
      auto size = m.GetSize();
      auto maze = m.GetMaze();
      int** distance;
      Maze::Position** parent;
      LOG(1," Allocating space for distace and ancestor matrix with size:"<<size<<'x'<<size)
      try{
            distance = new int*[size];
            parent = new Maze::Position*[size];
            for(int32_t i=0; i<size;i++){
                  distance[i] = new int[size];
                  parent[i]   = new Maze::Position[size];
            }
      }catch(std::bad_alloc& ba){
            LOG(3," Failed to allocated Memory!")
      }
      LOG(1," Space Successfully Allocated.")
      LOG(1," Initializing The Distance and Ancestor matrix")
      try{
            for(int32_t i=0; i<size; i++){
                  for(int32_t j=0;j<size; j++){
                        distance[i][j]=__INT32_MAX__;
                        parent[i][j] = {0,0};
                  }
            }
      }catch(std::exception& e){
            LOG(3,"Unhandled Exception:"<<e.what()<<" ")
      }
      LOG(1," The Distance and Ancestor matrix successfully initialized")

      distance[1][1]=0;

      LOG(1," Calculating Shortest Path")
      try{
            uint32_t i,j;
            for(int32_t n=0;n<(size*size)-1;n++)
            {      for(i=1; i<size;i++){
                        for(j=1; j<size; j++){
                              if((i-1)>=0 and (j-1)>=0 and (j+1)<size and (i+1)<size){
                                    // LEFT
                                    if((maze[i][j]==' ' or maze[i][j]=='X') and distance[i][j]!=__INT32_MAX__ and \
                                    (maze[i][j-1]==' '  or maze[i][j-1]=='X') and \
                                    distance[i][j]+1<distance[i][j-1]){
                                          distance[i][j-1] = distance[i][j]+1;
                                          parent[i][j-1] = Maze::Position(i,j);
                                    }
                                    //RIGHT
                                    if((maze[i][j]==' ' or maze[i][j]=='X') and distance[i][j]!=__INT32_MAX__ and \
                                    (maze[i][j+1]==' ' or  maze[i][j+1]=='X') and \
                                    distance[i][j]+1<distance[i][j+1]){
                                          distance[i][j+1] = distance[i][j]+1;
                                          parent[i][j+1] = Maze::Position(i,j);
                                    }      
                                    

                                    //DOWN
                                    if((maze[i][j]==' ' or maze[i][j]=='X') and distance[i][j]!=__INT32_MAX__ and \
                                    (maze[i+1][j]==' ' or maze[i+1][j]=='X') and \
                                    distance[i][j]+1<distance[i+1][j]){
                                          distance[i+1][j] = distance[i][j]+1;
                                          parent[i+1][j] = Maze::Position(i,j);

                                    }
                                    
                                    //UP
                                    if((maze[i][j]==' ' or maze[i][j]=='X') and distance[i][j]!=__INT32_MAX__ and \
                                    (maze[i-1][j]==' ' or maze[i-1][j]=='X') and \
                                    distance[i][j]+1<distance[i-1][j]){
                                          distance[i-1][j] = distance[i][j]+1;
                                          parent[i-1][j] = Maze::Position(i,j);
                                    }

                              }
                        
                        }
                  #ifndef NVERBOSE
                        for(int i=0; i<size; i++){
                              for(int j=0; j<size; j++){
                                    if(parent[i][j]==Maze::Position(0,0)){
                                          std::cout<<"-"<<"     ";continue;
                                    }
                                    std::cout<<parent[i][j]<<" ";
                              }
                              std::cout<<std::endl;
                        }
                  #endif
                  }
            }
      }catch(std::exception& oor){
            LOG(3," Unexpected Error Occured:"<<oor.what()<<" ")
      }
      LOG(1," Shortest Path Calculated")
      if(parent[target.x][target.y]==Maze::Position(0,0)){
            LOG(1," No path to target Found")
            std::cout<<target<<"->Target Parents:"<<parent[target.x][target.y]<<std::endl;
            for(int i=0; i<size; i++){
      std::cout<<std::endl;
}
            return solution();
            }
      std::vector<Maze::Position> PathToTarget;
      std::vector<int> Distance;
      auto curr = target;
      PathToTarget.push_back(curr);
      while (curr!=Maze::Position(1,1)){
            curr = parent[curr.x][curr.y];
            PathToTarget.push_back(curr);
#ifndef NVERBOSE
            std::cout<<"Test:"<<curr<<parent[curr.x][curr.y]<<std::endl;
#endif
            // Distance.push_back(distance[curr.x][curr.y]);
      }
      std::reverse(PathToTarget.begin(),PathToTarget.end());
      std::reverse(Distance.begin(),Distance.end());
#ifndef NVERBOSE
      LOG(1,"[VERBOSE] Path to target:")
      for(auto i: PathToTarget){
            std::cout<<i<<' ';
      }
      std::cout<<std::endl;
      LOG(1,"[VERBOSE] Total cost:"<<distance[target.x][target.y])
      std::cout<<"Parent Mat:"<<std::endl;
#endif

#ifdef PARENTMAT
for(int i=0; i<size; i++){
      for(int j=0; j<size; j++){
            if(parent[i][j]==Maze::Position(0,0)){
                  std::cout<<"-"<<"  ";continue;
            }
            std::cout<<parent[i][j]<<" ";
      }
      std::cout<<std::endl;
}
#endif

      LOG(1," Returning the Path")
      solution s = {Distance,PathToTarget};
      for(int i=0; i<size; i++){
            delete[] distance[i];
            delete[] parent[i];
      }
      delete[] distance;
      delete[] parent;
      LOG(1," Returned")
      return s;
}


//;================Solver::Solver(MazeGen)============
SOLVER::Solver::Solver(Maze::MazeGen &m){
      LOG(2," "<<m.GetSize()<<" Copying the content of the maze")
      
      LOG(1," Initializing Class Arttibutes")
      // Maze::MazeGen m2;
      // m2=m;
      this->maze = m;
      auto maze = this->maze.GetMaze();
      auto size = this->maze.GetSize();
      auto start = Maze::Position(1,1);
      this->target = Maze::Position();
      LOG(1,"Attributes Initialized Successfully:\nsize->"<<this->maze.GetSize()<<"\nTarget-->"<<this->maze.GetTarget())
      
      bool **visited = new bool*[size];
      for(int i=0; i<size; i++){
            visited[i] = new bool[size];
      }
      std::queue<Maze::Position> q;
      q.push(start);
      while(!q.empty()){
            auto node = q.front();
            q.pop();
            
      #ifndef NVERBOSE
      LOG(1,"[VERBOSE] BFS currently on "<<node)
      #endif
            if(this->maze[node]=='X') {this->target=node;std::cout<<"---->"<<node<<std::endl; break;}
            if(visited[node.x][node.y]){
                  continue;
            }else{
                  visited[node.x][node.y] = true;
                  if(maze[node.x][node.y-1]==' ' or maze[node.x][node.y-1]=='X'){
                        q.push(Maze::Position(node.x,node.y-1));
                  }if(maze[node.x][node.y+1]==' ' or maze[node.x][node.y+1]=='X'){
                        q.push(Maze::Position(node.x,node.y+1));
                  }if(maze[node.x-1][node.y]==' ' or maze[node.x-1][node.y]=='X'){
                        q.push(Maze::Position(node.x-1,node.y));
                  }if(maze[node.x+1][node.y]==' ' or maze[node.x+1][node.y]=='X'){
                        q.push(Maze::Position(node.x+1,node.y));
                  }else{
      #ifndef NVERBOSE
      LOG(1,"[VERBOSE] BFS currently on "<<node)
      #endif
                  }
            }
      }
      if(this->target==Maze::Position(0,0)){
            LOG(1," Target Not Found!")
      }else
            LOG(1," Target Found at "<<this->target);
};


//;=======Solver::Solve(solution alog(MazeGen,Position))====
void Solver::Solve(solution (algo)(Maze::MazeGen &m,Maze::Position &target)){
      if(this->target==Maze::Position(0,0)){
            LOG(1, "\x1B[0,31 mTarget Unreachable!\x1B[0m")
      }else{
      #ifndef NDEBUG      
      LOG(1," Saving the Solution")
      #endif
            auto sol = algo(this->maze,this->target);
            this->sol = sol;
      #ifndef NDEBUG
      LOG(1,"Solution Saved")
      #endif
      }
}


//;================Solver::ShowSolution(char mode,char AlgoIndex)====
void Solver::ShowSolution(char mode,char AlgoIndex){
      LOG(1,"Initiated with Parameters: mode->"<<mode)
#ifndef NDEBUG
LOG(1," Solution Size:"<<this->sol.parent.size())
#endif
      if(mode=='0'){
            //: Path only mode

            for(int i=0; i<this->sol.parent.size();i++){
                  std::cout<<this->sol.parent[i]<<'-'<<"Cost Expanded:"<<this->sol.distance[i]<<std::endl;
            }
      }else if(mode=='1'){
            switch (AlgoIndex)
            {
            case '1':
                  LOG(1,"Opted For BellmanFord Algorithm")
                  this->Solve(BellmanFord); 
                  break;
            case '2':
                  LOG(1,"Opted For Dijkstras Algorithm")
                  LOG(2,"Method Not Implemeted Yet")
                  exit(1);
                  this->Solve(DisjkstrasAlgo); 
                  break;
            default:
                  break;
            }
            #ifndef NDEBUG
                  LOG(1,"Process Initiated")
            #endif
            for(auto i: this->sol.parent){
                  this->maze[i]='|';
            }
                  auto size = this->maze.GetSize();
                  for(int i=0;i<size; i++){
                        for(int j=0; j<size; j++){
                              auto pos = Maze::Position(i,j);
                              if(i==0 or i ==size-1 or j==0 or j == size-1){
                                    std::cout<<"\x1B[7;90m \x1B[0m"<<"\x1B[7;90m \x1B[0m";
                              }else if(this->maze[pos]=='*'){
                                    std::cout<<asterisk<<asterisk;
                              }else if(this->maze[pos]==' '){
                                    std::cout<<space<<space;
                              }else if(this->maze[pos]=='X'){
                                    std::cout<<targ<<space;
                              }else if(this->maze[pos]=='|'){
                                    std::cout<<path<<path;
                              }
                              // std::cout<<"\x1B[7;30m  \x1B[0m";
                        }
                        std::cout<<std::endl;
                  }
            #ifndef NDEBUG
                  LOG(1," executed successfully")
            #endif
            }

      LOG(1,"Executed Successfully")
}

//;===============solution::operator=(solution)==========
SOLVER::solution SOLVER::solution::operator=(SOLVER::solution agr2){
 LOG(1,"In solution")
 std::swap(this->distance,agr2.distance);
 std::swap(this->parent,agr2.parent);
 LOG(1,"Assignement operator excuted successfully")

 return *this;
}