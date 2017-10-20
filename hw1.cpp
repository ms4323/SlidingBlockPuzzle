#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <ctime>


int iterDeep = 0;

class Move {

public:
    int piece;
    int piece_x;
    int piece_y;
    int direction; // 1 for right, 2 for left, 3 for up, 4 for down
    Move(int p, int dir);
    Move(int px,int py, int dir);
    
};

Move::Move(int p, int dir){
    piece = p;
    direction = dir;
}

Move::Move(int px , int py, int dir){
    piece_x = px;
    piece_y = py;
    direction = dir;
}

class MoveMove {
    
public:
    std::vector<Move> moves_ij;
    std::vector<Move> moves_p;
    
    MoveMove(std::vector<Move> m1,std::vector<Move> m2){ moves_ij = m1; moves_p = m2; }
    
};

class State {
    

public:
    int state_width;
    int state_height;
    int** state_matrix;
    State();
    State(int** state,int H, int W);
    int** genState_matrix(int H, int W);
    bool isSolved(State s);
    bool isIdentical(State s1 , State s2);
    State cloneState_matrix(State s);
    State normalizeState_matrix(State s);
    void printState_matrix(State s);
    MoveMove genMoves(State s, int p);
    MoveMove genAllMoves(State s);
    void applyMove(State s, Move m);
    State applyMoveCloning(State s, Move m);

   
};

State::State(){
    state_matrix = NULL;
}


State::State(int** state,int H, int W){
    state_matrix = state;
    state_height = H;
    state_width = W;
}

int** State::genState_matrix(int H, int W){
    

    int** state_matrix = 0;
    state_matrix = new int*[H];
    state_height = H;
    state_width = W;
    
    for(int i = 0; i < H; i++){
        state_matrix[i] = new int[W];
        for(int j = 0; j < W; j++){
            state_matrix[i][j] =  0;
        }
    }
    return state_matrix;
}

State State::cloneState_matrix(State s){
    
    int H = s.state_height;
    int W = s.state_width;
    int** mat = s.state_matrix;
    
    int** clone_state_matrix;
    clone_state_matrix = new int*[H];
    
    for (int i=0; i< H ; i++){
        clone_state_matrix[i] = new int[W];
        for (int j = 0 ; j < W ; j++){
            clone_state_matrix[i][j] = mat[i][j];
        }
    }
    

    State cloned_state = State(clone_state_matrix,H,W);
    return cloned_state;
}

State State::normalizeState_matrix(State s){
    
    
    int H = s.state_height;
    int W = s.state_width;
    int** state = s.state_matrix;
    
    int nextIdx = 3;
    for(int i = 0;i < H;i++) {
        for(int j = 0;j < W;j++) {
            if (state[i][j]==nextIdx) {
                nextIdx++;
            } else if (state[i][j] > nextIdx) {
                int idx1 = nextIdx;
                int idx2 = state[i][j];
                
                for(int i = 0;i < H;i++) {
                    for(int j = 0;j < W;j++) {
                        if (state[i][j]==idx1) {
                            state[i][j]=idx2;
                        } else if (state[i][j]==idx2) {
                            state[i][j]=idx1;
                        }
                    }
                }
                nextIdx++;
            }
        }
    }
    
    s.state_matrix = state;
    return s;
    //return state;
}

bool State::isSolved(State s)
{
    
    int** arr = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    int area = W*H;
    
    int sum = 0;
    int zeroSum = 0;

    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            if (arr[i][j] == -1 ){
                sum++;
            }
            else if (arr[i][j] == 0){
                zeroSum++;
            }
        }
    }
    //std::cout<<"sum is: "<<sum<<" zeroSum is: "<<zeroSum<<std::endl;
    if ((sum == 0) && (zeroSum != area)) return true;
    else return false;
}

bool State::isIdentical(State s1, State s2){
    
    int** arr1 = s1.state_matrix;
    int h1 = s1.state_height;
    int w1 = s1.state_width;
    
    int** arr2 = s2.state_matrix;
    int h2 = s2.state_height;
    int w2 = s2.state_width;
    
    int res = 0;
    
    if ((h1 == h2) && (w1 == w2)){
        for (int i=0; i<h1; i++){
            for (int j=0; j<w1 ; j++){
                if (arr1[i][j] != arr2[i][j]) {
                    res++;
                }
            
            }
        }
    }
    
    if (res == 0) return true;
    else return false;
    
}


void State::printState_matrix(State s)
{
    
    int** arr = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    std::cout << "\n";
    std::cout<< W << "," << H << ",";
    for (int i = 0; i < H; i++){
        std::cout << "\n";
        for (int j = 0; j < W; j++){
            
            std::cout<< arr[i][j] <<",";
        }
    }
    
}

State State::applyMoveCloning(State s, Move m){


    State cloned_state = s.cloneState_matrix(s);
    cloned_state.applyMove(cloned_state,m);
    
    return cloned_state;
    
}

void State::applyMove(State s, Move m){
    
    //int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    int dirr = m.direction;
    int px = m.piece_x;
    int py = m.piece_y;
    
    int dir = dirr;
    int i = px;
    int j = py;
    
    
    int p = s.state_matrix[i][j];
    int temp;
    
    if (dir == 1) { // right direction
        

        /*
        if (((s.state_matrix[i+1][j]) == p1 ) && (last_elem == p1 )){ // square cell
            
            temp = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = temp;
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j+2];
            s.state_matrix[i][j+2] = temp;
            
            temp = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = s.state_matrix[i+1][j+2];
            s.state_matrix[i+1][j+2] = temp;
            
        }
        */
        if ((s.state_matrix[i+1][j]) == p ){ //vertical cell
            
            temp = s.state_matrix[i][j];
            //int last_elem = s.state_matrix[i][j+1];
            s.state_matrix[i][j] = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = temp;
  
            
            temp = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = temp;
        }
        
        else if(s.state_matrix[i][j+1] == p ){ // horizontal cell
            
            temp = s.state_matrix[i][j+1];
            //int last_elem = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i][j+2];
            s.state_matrix[i][j+2] = temp;
            
   
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = temp;
        }
        else {
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = temp;
        }
        
        
        
    }
    else if (dir == 2){ //left direction
        
        
        /*
        if (((s.state_matrix[i+1][j]) == p1 ) && ((s.state_matrix[i][j+1]) == p1 )){ // square cell
            
            temp = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = s.state_matrix[i+1][j-1];
            s.state_matrix[i+1][j-1] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i][j];
            s.state_matrix[i][j] = temp;
            
            temp = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
            
        }
        */
        if ((s.state_matrix[i-1][j]) == p ){ //vertical cell
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i-1][j] = s.state_matrix[i-1][j-1];
            s.state_matrix[i-1][j-1] = temp;
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j-1];
            s.state_matrix[i][j-1] = temp;
        }
        else if((s.state_matrix[i][j+1]) == p ){ // horizontal cell
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j-1];
            s.state_matrix[i][j-1] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i][j];
            s.state_matrix[i][j] = temp;

            
            
        }
        else {
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i][j-1];
            s.state_matrix[i][j-1] = temp;
        }
        
        
        
        
    }
    else if (dir == 3) { // down direction
        
        
        /*
        if (((s.state_matrix[i-1][j]) == p1 ) && ((s.state_matrix[i][j+1]) == p1 )){ // square cell
            
            temp = s.state_matrix[i-1][j];
            s.state_matrix[i-1][j] = s.state_matrix[i][j];
            s.state_matrix[i][j] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = temp;
            
            temp = s.state_matrix[i-1][j+1];
            s.state_matrix[i-1][j+1] = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = temp;
            
        }
         */
        
        if (s.state_matrix[i+1][j] == p ){ //vertical cell
            
            temp = s.state_matrix[i+2][j];
            s.state_matrix[i+2][j] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
            
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
            
            
        }
        
        else if((s.state_matrix[i][j+1]) == p ){ // horizontal cell
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = temp;
            
        }
        
        else {
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
        }
        
        
    }
    else if (dir == 4){ // up direction



        /*
        if (((s.state_matrix[i+1][j]) == p1 ) && ((s.state_matrix[i][j+1]) == p1 )){ // square cell
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i-1][j+1];
            s.state_matrix[i-1][j+1] = temp;
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i+1][j+1];
            s.state_matrix[i+1][j+1] = temp;
            
        }
        */
        if ((s.state_matrix[i+1][j]) == p ){ //vertical cell
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i-1][j];
            s.state_matrix[i-1][j] = temp;
            
            temp = s.state_matrix[i+1][j];
            s.state_matrix[i+1][j] = s.state_matrix[i][j];
            s.state_matrix[i][j] = temp;
        }
        else if((s.state_matrix[i][j+1]) == p ){ // horizontal cell
            
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i-1][j];
            s.state_matrix[i-1][j] = temp;
            
            temp = s.state_matrix[i][j+1];
            s.state_matrix[i][j+1] = s.state_matrix[i-1][j+1];
            s.state_matrix[i-1][j+1] = temp;
        }
        else {
            temp = s.state_matrix[i][j];
            s.state_matrix[i][j] = s.state_matrix[i-1][j];
            s.state_matrix[i-1][j] = temp;
        }
        
    }
    
    
    
    //std::cout << "executing the move: ";
    
    for (int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            if (s.state_matrix[i][j] == -1){
                s.state_matrix[i][j] =0;
            }
        }
    }
    
    //printState_matrix(s);
    //std::cout << "\n";

    //return state;
    
}

MoveMove State::genAllMoves(State s)
{
    int** arr = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    
    std::vector<Move> all_moves_ij;
    std::vector<Move> all_moves_p;
    
    for (int k=2; k<20; k++){
        std::vector<Move> m1 = s.genMoves(s,k).moves_ij;
        std::vector<Move> m2 = s.genMoves(s,k).moves_p;
        for (int i =0; i< m1.size(); i++){
            all_moves_ij.push_back(m1[i]);
            all_moves_p.push_back(m2[i]);
        }
        
    }
    
    MoveMove res = MoveMove(all_moves_ij,all_moves_p);
    
    return res;
}

MoveMove State::genMoves(State s, int p)
{
    int** arr = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    std::vector<Move> possible_moves;
    std::vector<Move> possible_moves2;
    
    int down =0;
    int right =0;
    
    int res_move=0; // 1 for right, 2 for left, 3 for down, 4 for up
    
    for (int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            
            int r=0;
            int d=0;
            
            
            if (arr[i][j] == p){
                
                while (arr[i][j+r+1] == p) r++;
                while (arr[i+d+1][j] == p) d++;
                
                right = r;
                down = d;
                
                
                if (right == 0 && down == 0){ // 1 cell
                    if ( ( p == 2 && arr[i][j+1] == -1) || (arr[i][j+1] == 0) ){
                        res_move = 1;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                        
                    }
                    if (( p == 2 && arr[i][j-1] == -1) || (arr[i][j-1] == 0)){
                        res_move = 2;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if (( p == 2 && arr[i+1][j] == -1) || (arr[i+1][j] == 0)){
                        res_move = 3;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if (( p == 2 && arr[i-1][j] == -1) || (arr[i-1][j] == 0)){
                        res_move = 4;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    
                }
                else if (right > 0 && down == 0){ //2 horizontal cells
                    if (( p == 2 && arr[i][j-1] == -1) || (arr[i][j-1] == 0)){
                        res_move = 2;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if (( p == 2 && arr[i][j+2] == -1) || (arr[i][j+2] == 0)){
                        res_move = 1;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ( (( p == 2 && arr[i-1][j] == -1) && ( p == 2 && arr[i-1][j+1] == -1)) ||
                        ((arr[i-1][j] == 0) && (arr[i-1][j+1] == 0)) ){
                        res_move = 4;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i+1][j] == -1) && ( p == 2 && arr[i+1][j+1] == -1)) ||
                        ((arr[i+1][j] == 0) && (arr[i+1][j+1] == 0))){
                        res_move = 3;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    
                }
                
                else if (right == 0 && down > 0){ //2 vertical cells
                    if (( p == 2 && arr[i-1][j] == -1) || (arr[i-1][j] == 0)){
                        res_move = 4;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if (( p == 2 && arr[i+2][j] == -1) || (arr[i+2][j] == 0)){
                        res_move = 3;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i][j-1] == -1) && ( p == 2 && arr[i-1][j-1] == -1)) ||
                        ((arr[i][j-1] == 0) && (arr[i-1][j-1] == 0))){
                        res_move = 2;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i+1][j+1] == -1) && ( p == 2 && arr[i][j+1] == -1)) ||
                        (((arr[i+1][j+1]) == 0) && (arr[i][j+1] == 0))){
                        res_move = 1;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                }
                else if (right > 0 && down > 0){ //4 square cells
                    if ((( p == 2 && arr[i-1][j] == -1) && ( p == 2 && arr[i-1][j+1] == -1)) ||
                        ((arr[i-1][j] == 0) && (arr[i-1][j+1] == 0))){
                        res_move = 4;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i+2][j] == -1) && ( p == 2 && arr[i+2][j+1] == -1)) ||
                        ((arr[i+2][j] == 0) && (arr[i+2][j+1] == 0))){
                        res_move = 3;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i][j+2] == -1) && ( p == 2 && arr[i+1][j+2] == -1)) ||
                        ((arr[i][j+2] == 0) && (arr[i+1][j+2] == 0))){
                        res_move = 1;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    if ((( p == 2 && arr[i][j-1] == -1) && ( p == 2 && arr[i+1][j-1] == -1)) ||
                        ((arr[i][j-1] == 0) && (arr[i+1][j-1] == 0))){
                        res_move = 2;
                        possible_moves.push_back(Move(i,j,res_move));
                        possible_moves2.push_back(Move(p,res_move));
                    }
                    
                }
                
                
                
                break;
                
            }
            
            
            
        }
        
    }
    
    
    MoveMove res = MoveMove(possible_moves,possible_moves2);
    
    return res;
    
}

class MoveParent {
    
public:
    State state;
    std::vector<Move> moves;
    
    MoveParent(State s){ state = s ; }
    MoveParent(State s,std::vector<Move> m){ state = s; moves = m; }
    
};




int** randomWalks(State s, int N){
    
    int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;

    State ss = s;
    
    int nn = N;
    while (nn > 0){
        

        //1. generate all possible moves
        
        std::vector <Move> moves = ss.genAllMoves(ss).moves_ij;
       
        //2. select one move randomly
        int r = rand() % moves.size();
        int p = ss.state_matrix[moves[r].piece_x][moves[r].piece_y];
    
        
        std::string dirs;
        if (moves[r].direction == 1){
            dirs = "right";
        }
        else if (moves[r].direction == 2){
            dirs = "left";
        }
        else if (moves[r].direction == 3){
            dirs = "down";
        }
        else if (moves[r].direction == 4){
            dirs = "up";
        }

        std::cout<< "\n("<< p << "," << dirs<<")"<<std::endl;
    
        //3. execute the random move to the state matrix
    
        ss = ss.applyMoveCloning(ss,moves[r]);
        
   
        //4. normalize the state
        
        State normalized_state = ss.normalizeState_matrix(ss);

        
        normalized_state.printState_matrix(normalized_state);
        std::cout << "\n";
    
        
        //5. solved or continue to step 1
        if (normalized_state.isSolved(normalized_state)){
            std::cout<<"solved!"<<std::endl;
            break;
        }
        state = normalized_state.state_matrix;
  
        nn--;
    }
    return state;
    
}

int** BFS(State s){
    
    int** solved;
    int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    std::queue<State> myQ;

    std::vector<State> visited_states;
    std::vector<MoveParent> mp;
    
    State myState = s;
    
    myQ.push(myState);
    mp.push_back(MoveParent(myState));
    
    
    int length = 0;
 
    while (!myQ.empty()){
        

        myState = myQ.front();

        myQ.pop();
        
        MoveParent first_parent = mp[0];
        State first_ps = first_parent.state;
        std::vector<Move> first_pm = first_parent.moves;
        
        visited_states.push_back(myState);
        
        
        std::vector<Move> ms2 = myState.genAllMoves(myState).moves_ij;
        std::vector<Move> ms = myState.genAllMoves(myState).moves_p;
        
        int res= 0 ;
        int added = 0;
        
        for (int k= 0; k< ms.size(); k++){
            
            res = 0;
            added = 0;
            
            State ss = myState.applyMoveCloning(myState,ms2[k]);
            State ns = ss.normalizeState_matrix(ss);
            
            std::vector<Move> temp = first_pm;
            temp.push_back(ms[k]);
            MoveParent mpp = MoveParent(first_ps,temp);
            
            for (int j =0; j<visited_states.size(); j++){
 
                State visited = visited_states[j].normalizeState_matrix(visited_states[j]);
                if(ns.isIdentical(visited,ns)){
                    res++;
                    continue;
                }
            }
            
            if (res == 0){
                
                if (ss.isSolved(ss)){
                    std::cout<< "solved!"<<std::endl;
                    
                    int counter = 0;
                    
                    std::vector<Move> mmss = mpp.moves;

                    
                    for (int l = 0 ; l<mmss.size(); l++){
                        
                        int diir = mmss[l].direction;
                        std::string dirString;
                        if (diir == 1){
                            dirString = "right";
                        }
                        else if (diir == 2){
                            dirString = "left";
                        }
                        else if (diir == 3){
                            dirString = "down";
                        }
                        else if (diir == 4){
                            dirString = "up";
                        }
                        
                        std::cout<<"("<<mmss[l].piece<<","<<dirString<<")"<<std::endl;
                        counter ++;
                    }
                    
                    solved = ss.state_matrix;
                    ss.printState_matrix(ss);
                    std::cout<<"\n";
                    std::cout<<"\n";
                    std::cout<<"#nodes length time";
                    std::cout<<"\n";
                    std::cout<<visited_states.size()<<" ";
                    std::cout<<counter<<" ";
                    
                    
                    
                    return solved;
                }
                
                myQ.push(ss);
                mp.push_back(mpp);
                
                added++;
            }
            

            
        }
 
        if (added >= 1 ){
            length++;
        }
        
        mp.erase(mp.begin());

    }

    return solved;
    
}

int** DFS(State s){
    int** solved;
    int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    std::stack<State> myQ;
    
    std::vector<State> visited_states;
    
    std::vector<Move> parent_tracks;
    
    State myState = s;
    
    myQ.push(myState);
    
    
    int length = 0;
    
    while (!myQ.empty()){
        
        
        myState = myQ.top();
        
        myQ.pop();
        
        visited_states.push_back(myState);
        
        std::vector<Move> steps;
        
        std::vector<Move> ms2 = myState.genAllMoves(myState).moves_ij;
        std::vector<Move> ms = myState.genAllMoves(myState).moves_p;
        
        int res= 0 ;
        int added = 0;
        
        for (int k= 0; k< ms.size(); k++){
            
            res = 0;
            added = 0;
            
            State ss = myState.applyMoveCloning(myState,ms2[k]);
            State ns = ss.normalizeState_matrix(ss);

            
            for (int j =0; j<visited_states.size(); j++){
                
                State visited = visited_states[j].normalizeState_matrix(visited_states[j]);
                if(ns.isIdentical(visited,ns)){
                    res++;
                    continue;
                }
            }
            
            if (res == 0){
 
                if (ss.isSolved(ss)){
                    std::cout<< "solved!"<<std::endl;
                    
                    int counter = 0;

                    for (int jj=0 ; jj < parent_tracks.size(); jj++){
                        int diir = parent_tracks[jj].direction;
                        int piece = parent_tracks[jj].piece;
                        
                        std::string dirString;
                        if (diir == 1){
                            dirString = "right";
                        }
                        else if (diir == 2){
                            dirString = "left";
                        }
                        else if (diir == 3){
                            dirString = "down";
                        }
                        else if (diir == 4){
                            dirString = "up";
                        }
                        
                        std::cout<<"("<<piece<<","<<dirString<<")"<<std::endl;
                        counter ++;
                    }
                    
                    
                    solved = ss.state_matrix;
                    ss.printState_matrix(ss);
                    std::cout<<"\n";
                    std::cout<<"\n";
                    std::cout<<"#nodes length time";
                    std::cout<<"\n";
                    std::cout<<visited_states.size()<<" ";
                    std::cout<<length<<" ";
                    
                    
                    
                    
                    return solved;
                }
                
                myQ.push(ss);
                steps.push_back(ms[k]);
                
                added++;
            }
 
            
        }


        if (added >= 1 ){
            length++;
        }
        
        int stepSize = steps.size();
        while (stepSize > 0){

            parent_tracks.push_back(steps.back());
            stepSize--;
            
        }
        
        parent_tracks.pop_back();


        
    }
    
    return solved;
    
}

int** depthLimitedDFS(State s, int L){
    
    
    int** solved;
    int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    std::stack<State> myQ;
    std::vector<State> visited_states;
    
    
    State myState = s;
    myQ.push(myState);
    int length = 0;
    int depth = 0;
    std::vector<Move> parent_tracks2;
    
    while (!myQ.empty()){
        
        
        State myState = myQ.top();
        myQ.pop();
        

        if (myState.isSolved(myState)){
            
            //std::cout<< "solved at depth "<<depth<<std::endl;
            solved = myState.state_matrix;
            

            
        }
     
        if ( depth <= L ){
        
            std::vector<Move> parent_tracks;
            visited_states.push_back(myState);
            
            std::vector<Move> steps;
            
            std::vector<Move> ms2 = myState.genAllMoves(myState).moves_ij;
            std::vector<Move> ms = myState.genAllMoves(myState).moves_p;
            
            std::vector<State> children;
            
            for (int k= 0; k< ms.size(); k++){
                
                State ss = myState.applyMoveCloning(myState,ms2[k]);
                State ns = ss.normalizeState_matrix(ss);
                
                children.push_back(ns);
            }
            
            reverse(children.begin(),children.end());
            
            int res= 0 ;
            int added = 0;
            for (int i = 0 ; i< children.size(); i++){
                res = 0;
                added = 0;
                
                for (int j =0; j<visited_states.size(); j++){
                    
                    
                    State visited = visited_states[j].normalizeState_matrix(visited_states[j]);
                    if(children[i].isIdentical(visited,children[i])){
                        res++;
                        continue;
                    }
                    
                }
                
                if (res == 0){
                    
                    myQ.push(children[i]);
                    steps.push_back(ms[i]);
                    
                    
                    added++;
                }
                
            }
            
            if (added > 0 ){
                length++;
                depth++;
            }
            
            int stepSize = steps.size();
            while (stepSize > 0){
                
                parent_tracks.push_back(steps.back());
                stepSize--;
                
            }
            
            parent_tracks.pop_back();
            
            
        }
        
    }

    
    std::cout<<"\n";
    std::cout<<"\n";
    std::cout<<"#nodes length time";
    std::cout<<"\n";
    std::cout<<visited_states.size()<<" ";
    std::cout<<depth<<" ";

    
    iterDeep += visited_states.size();
    return solved;
}

int** iterartiveDeepening(State s, int L){
    
    int** solved;
    int** state = s.state_matrix;
    int H = s.state_height;
    int W = s.state_width;
    
    for (int i =0; i< L ; i++){
        std::cout<<"\n";
        std::cout<<"for L = "<<i<<std::endl;
        float start_s_itDeep = clock();
        solved = depthLimitedDFS(s,i);
        float stop_s_itDeep = clock();
        std::cout <<((float)(stop_s_itDeep-start_s_itDeep))/CLOCKS_PER_SEC<<std::endl;
    
    }
    
    return solved;
}


int main(int argc, char **argv)
{

    std::ifstream fin;
    fin.open("SBP-level0.txt");
    
    int width ,height;
    char comma;
    fin >> width >> comma >> height>> comma;
    
    
    std::cout<<"\nPart 1: Random Walks\n"<<std::endl;
    
    
    std::cout << width << "," << height<< ",";
    
    // generating a new state and state matrix
    State state;
    int** state_matrix = state.genState_matrix(height,width);
    state.state_matrix = state_matrix;

    
    int k = -1;
    while (fin)
    {
        std::string s;
        if (!getline( fin, s )) break;
        
        std::istringstream ss( s );
        std::vector <std::string> record;
        int val;
        while (ss)
        {
            std::string s;
            if (!getline( ss, s, ',' )) break;
            record.push_back( s );
        }
        
        
        for (int i =0 ; i< record.size() ; i++){
            
            state_matrix[k][i] = strtof((record[i]).c_str(),0);
            std::cout <<state_matrix[k][i]<< ",";
       
        }
        std::cout<<"\n";
        k++;
   
    }
    
    if (!fin.eof())
    {
        std::cerr << "Fooey!\n";
    }
    
    fin.close();
    
    //normalizing our state
    State normalized_state = state.normalizeState_matrix(state);

 
    //random walks for N = 3
    int** newState_random = randomWalks(normalized_state,3);
    
    
    std::cout<<"\nPart 2: Search Algorithms\n"<<std::endl;
    
    fin.open("SBP-level1.txt");
    
    fin >> width >> comma >> height>> comma;

    std::cout << width << "," << height<< ",";
    
    // generating a new state and state matrix

    state_matrix = state.genState_matrix(height,width);
    state.state_matrix = state_matrix;
    
    
    int kk = -1;
    while (fin)
    {
        std::string s;
        if (!getline( fin, s )) break;
        
        std::istringstream ss( s );
        std::vector <std::string> record;
        int val;
        while (ss)
        {
            std::string s;
            if (!getline( ss, s, ',' )) break;
            record.push_back( s );
        }
        
        
        for (int i =0 ; i< record.size() ; i++){
            
            state_matrix[kk][i] = strtof((record[i]).c_str(),0);
            std::cout <<state_matrix[kk][i]<< ",";
            
            
        }
        std::cout<<"\n";
        kk++;
        
        
        
    }
    
    if (!fin.eof())
    {
        std::cerr << "Fooey!\n";
    }
    
    fin.close();
    
    
    //breadth-first search
    std::cout<< "\nBreath First Search:"<<std::endl;
    std::cout<<"\n";
    
    float start_s_bfs=clock();
    int** newState_bfs = BFS(state);
    float stop_s_bfs=clock();
    float time = ((float)(stop_s_bfs-start_s_bfs))/CLOCKS_PER_SEC;
    std::cout <<time<< std::endl;
    
    //depth-first search
    std::cout<< "\nDepth First Search:"<<std::endl;
    std::cout<<"\n";

    float start_s_dfs=clock();
    int** newState_dfs = DFS(state);
    float stop_s_dfs=clock();
    std::cout <<((float)(stop_s_dfs-start_s_dfs))/CLOCKS_PER_SEC<<std::endl;
    
    
    //iterative deepening search
    std::cout<< "\nIterative Deepening Search:"<<std::endl;
    float t1 = clock();
    int** newState_itDeep = iterartiveDeepening(state,200);

    State myS = State (newState_itDeep,height,width);
    if(myS.isSolved(myS)){
        myS.printState_matrix(myS);
        std::cout<<"\n";
    }
    float t2 = clock();
    std::cout<<"\n";
    std::cout<<"Totally for all the depths:"<<std::endl;
    std::cout<<"#nodes time"<<std::endl;
    std::cout<<iterDeep<<" "<<((float)(t2-t1))/CLOCKS_PER_SEC<<std::endl;
    
    
    
}

