#include "maze.h"
#include "path.h"
#include<queue>
#include<stack>
#include<vector>
#include<list>
#include<unordered_set>
#include<tuple>
#include<utility>
#include<iostream>
#include<climits>
#include<sstream>
#include<unordered_map>
#include<queue>
#include<algorithm>
#include<memory>
using namespace std;

path solve_dfs(Maze& m, int rows, int cols);
path solve_bfs(Maze& m, int rows, int cols);
//path solve_dijkstra(Maze& m, int rows, int cols);
path solve_dijkstra(Maze& m, point & start, point & end, int & mycost);
path solve_tour(Maze& m, int rows, int cols);


int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cerr << "usage:\n"
             << "./maze option rows cols\n"
             << " options:\n"
             << "  -dfs: depth first search (backtracking)\n"
             << "  -bfs: breadth first search\n"
             << "  -dij: dijkstra's algorithm\n"
             << "  -tour: all corners tour" << endl;
        return 0;
    }
    string opt(argv[1]);

    int rows, cols;
    stringstream s;
    s << argv[2] << " " << argv[3];
    s >> rows >> cols;

    // construct a new random maze;
    Maze m(rows, cols);

    // bao added some variables for changed dijkstra's function
    point start(0, 0);
    point end(rows - 1, cols - 1);
    int mycost = 0;

    // print the initial maze out
    cout << "Initial maze" << endl;
    m.print_maze(cout, opt == "-dij" || opt == "-tour");

    if(opt == "-dfs")
    {
        // solve the maze
        cout << "\nSolved dfs" << endl;
        path p = solve_dfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);
    }


    if(opt == "-bfs")
    {
        cout << "\nSolved bfs" << endl;
        path p = solve_bfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);
    }

    if(opt == "-dij")
    {
        cout << "\nSolved dijkstra" << endl;
	path p =solve_dijkstra(m, start, end, mycost);

        //path p = solve_dijkstra(m, rows, cols);
        m.print_maze_with_path(cout, p, true, false);
    }

    if(opt == "-tour")
    {
        cout << "\nSolved all courners tour" << endl;
        path p = solve_tour(m, rows, cols);
        m.print_maze_with_path(cout, p, true, true);
    }

    if(opt == "-basic")
    {
	    cout << "\nSolved dfs" << endl;
	    path p = solve_dfs(m, rows, cols);
	    m.print_maze_with_path(cout, p, false, false);

            cout << "\nSolved bfs" << endl;
	    p = solve_bfs(m, rows, cols);
	    m.print_maze_with_path(cout, p, false, false);

	    cout << "\nSolved dijkstra" << endl;
	    p =solve_dijkstra(m, start, end, mycost);
	    //p = solve_dijkstra(m, rows, cols);
	    m.print_maze_with_path(cout, p, true, false);
	
    }

    if(opt == "-advanced")
    {
	    cout << "\nSolved dfs" << endl;
	    path p = solve_dfs(m, rows, cols);
	    m.print_maze_with_path(cout, p, false, false);

            cout << "\nSolved bfs" << endl;
	    p = solve_bfs(m, rows, cols);
	    m.print_maze_with_path(cout, p, false, false);

	    cout << "\nSolved dijkstra" << endl;
	    p =solve_dijkstra(m, start, end, mycost);
	    //p = solve_dijkstra(m, rows, cols);
	    m.print_maze_with_path(cout, p, true, false);

    	    cout << "\nSolved all courners tour" << endl;
	    p = solve_tour(m, rows, cols);
	    m.print_maze_with_path(cout, p, true, true);
	    
    }

}

struct pair_hash
{
	size_t operator() (pair<int, int> const &pair)const
	{
		size_t h1 = hash<int>()(pair.first);
		size_t h2 = hash<int>()(pair.second);
		return h1^h2*31;
	}
};

//update for dfs
void update(point p, int dir, stack<point> & Stack, unordered_set<point, pair_hash> & myset, vector<vector<point>> & track)
{
	point p1 = p + moveIn(dir);

	if(myset.count(p1))
		return;

	Stack.push(p1);
	myset.insert(p1);
	track[p1.first][p1.second] = p;
}
//update for bfs
void update_bfs(point p, int dir, queue<point> & Q, unordered_set<point, pair_hash> & myset, vector<vector<point>> & track)
{
	point p1 = p + moveIn(dir);

	if(myset.count(p1))
		return;

	Q.push(p1);
	myset.insert(p1);
	track[p1.first][p1.second] = p;
}

//non-recursive
/*
path solve_dfs(Maze& m, int rows, int cols)
{
	if(rows < 0 || cols < 0) return {};

	list<point> List; 
	stack<point> Stack;
	unordered_set<point, pair_hash> Set;

	point start(0, 0);
	point end(rows - 1, cols - 1);
	vector<vector<point>> track(rows, vector<point>(cols, end));


	Stack.push(start);
	Set.insert(start);

	while(!Stack.empty())
	{
		point p = Stack.top();
		Stack.pop();

		//check for neighbors
		if(p.first == end.first && p.second == end.second){
			break;
		}
		if(m.can_go_up(p.first, p.second))
		{
			update(p, UP, Stack, Set, track);
		}
		if(m.can_go_down(p.first, p.second))
		{
			update(p, DOWN, Stack, Set, track);
		}
		if(m.can_go_left(p.first, p.second))
		{
			update(p, LEFT, Stack, Set, track);
		}
		if(m.can_go_right(p.first, p.second))
		{
			update(p, RIGHT, Stack, Set, track);
		}
	}

	//get the final path
	List.push_front(end);
	List.push_front(track[rows-1][cols-1]);

	while(!(List.front().first == 0 && List.front().second == 0))
	{
		point P = track[List.front().first][List.front().second];
		List.push_front(P);
	}

	return List;
}
*/
//recursive
bool dfs(Maze & m, point & p, point & end, list<point> & List, unordered_set<point, pair_hash> & Set )
{
	if(Set.count(p))
	{
		return false;
	}

	if(p.first == end.first && p.second == end.second)
	{
		List.push_front(p);
		return true;
	}
	Set.insert(p);
	
	if(m.can_go_up(p.first, p.second) && !Set.count(p + moveIn(UP)))
	{
		point p1 = p + moveIn(UP);
		if(dfs(m, p1,end,List, Set))
		{
			List.push_front(p);
			return true;
		}
	}
	if(m.can_go_down(p.first, p.second)&& !Set.count(p + moveIn(DOWN)))
	{
		point p2 = p + moveIn(DOWN);
		if(dfs(m, p2 ,end,List, Set))
		{
			List.push_front(p);
			return true;
		}
	}
	if(m.can_go_left(p.first, p.second)&& !Set.count(p + moveIn(LEFT)))
	{
		point p3 = p + moveIn(LEFT);
		if(dfs(m, p3,end,List, Set))
		{
			List.push_front(p);
			return true;
		}
	}
	if(m.can_go_right(p.first, p.second)&& !Set.count(p + moveIn(RIGHT)))
	{
		point p4 = p + moveIn(RIGHT);
		if(dfs(m, p4,end,List, Set))
		{
			List.push_front(p);
			return true;
		}
	}
	return false;
}

path solve_dfs(Maze& m, int rows, int cols)
{
	if(rows < 0 || cols < 0) return {};

	list<point> List; 
	unordered_set<point, pair_hash> Set;
	
	point start(0, 0);
	point end(rows - 1, cols - 1);
	dfs(m, start, end, List, Set);
	return List;
}


path solve_bfs(Maze& m, int rows, int cols)
{
	if(rows < 0 || cols < 0) return {};

	list<point> List; 
	queue<point> Q;
	unordered_set<point, pair_hash> Set;

	point start(0, 0);
	point end(rows - 1, cols - 1);
	vector<vector<point>> track(rows, vector<point>(cols, end));


	Q.push(start);
	Set.insert(start);

	while(!Q.empty())
	{
		point p = Q.front();
		Q.pop();

		//check for neighbors
		if(p.first == end.first && p.second == end.second){
			break;
		}
		if(m.can_go_up(p.first, p.second))
		{
			update_bfs(p, UP, Q, Set, track);
		}
		if(m.can_go_down(p.first, p.second))
		{
			update_bfs(p, DOWN, Q, Set, track);
		}
		if(m.can_go_left(p.first, p.second))
		{
			update_bfs(p, LEFT, Q, Set, track);
		}
		if(m.can_go_right(p.first, p.second))
		{
			update_bfs(p, RIGHT, Q, Set, track);
		}
	}

	//get the final path
	List.push_front(end);
	List.push_front(track[rows-1][cols-1]);

	while(!(List.front().first == 0 && List.front().second == 0))
	{
		point P = track[List.front().first][List.front().second];
		List.push_front(P);
	}

	return List;
}

//built a struct to hold some information: point, cost, and a previous pointer 
struct info
{
	point p;
	int cost;
	shared_ptr<info> previous;
	info(point P, int mycost, shared_ptr<info> parent)

	{
		p = P;
		cost = mycost;
		previous = parent;
	}
};

struct compare
{
	bool operator()(shared_ptr<info> a1, shared_ptr<info> a2)
	{
		return a1->cost > a2->cost;
	}
};
/*
void update_dijkstra(Maze &m,info * currentInfo, int dir, priority_queue<info *, vector<info*>, compare> & pq, unordered_map<point, int, pair_hash> & best)
{
	point p1 = currentInfo->p + moveIn(dir);
	int newCost = currentInfo->cost + m.cost(currentInfo->p.first, currentInfo->p.second, dir);

	if(best.find(p1) == best.end() || newCost < best[p1])
	{
		best[p1] = newCost;
		//info * newInfo = new info(p1, newCost, currentInfo);
		shared_ptr<info> newInfo = make_shared<info>(pl, newCost, currentInfo);	
		pq.push(newInfo);
	}

}
*/
void update_dijkstra(Maze &m, shared_ptr<info> currentInfo, 
		int dir, priority_queue<shared_ptr<info>, 
		vector<shared_ptr<info>>, compare> & pq, unordered_map<point, int, pair_hash> & best)
{
	point p1 = currentInfo->p + moveIn(dir);
	int newCost = currentInfo->cost + m.cost(currentInfo->p.first, currentInfo->p.second, dir);

	if(best.find(p1) == best.end() || newCost < best[p1])
	{
		best[p1] = newCost;

		shared_ptr<info> newInfo = make_shared<info>(p1, newCost, currentInfo);	
		
		pq.push(newInfo);
	}

}


//path solve_dijkstra(Maze& m, int rows, int cols)
path solve_dijkstra(Maze& m, point & start, point & end, int & mycost)

{
	//priority queue and unordered map come together
	unordered_map<point, int, pair_hash> best;
	priority_queue<shared_ptr<info>,vector<shared_ptr<info>>, compare> pq;

	//list to hold the path
	list<point> List; 

	//wrap information together: point and cost so far and it's previous pointer
	//1. push the first info into the q, the start point to the map
	//the queue and map always updated together
	shared_ptr<info> startPoint (new info(start, 0, NULL));
	pq.push(startPoint);
	
	best[start] = 0;

	//valiable for ending information
	shared_ptr<info> finalInfo;
	int finalCost = 0;

	//2. while que is not empty
	while(!pq.empty())
	{
		//3. get top element and pop the pq
		point p = pq.top()->p;
		int currentCost = pq.top()->cost;
		shared_ptr<info> currentInfo = pq.top();
		pq.pop();

		//exit of the loop
		if(p.first == end.first && p.second == end.second)
		{
			finalCost = currentCost;
			finalInfo = currentInfo;
		}

		//check the neighbors
		if(m.can_go_up(p.first, p.second))
		{
			update_dijkstra(m, currentInfo, UP, pq, best);
		}
		if(m.can_go_down(p.first, p.second))
		{
			update_dijkstra(m, currentInfo, DOWN, pq, best);
		}
		if(m.can_go_left(p.first, p.second))
		{
			update_dijkstra(m, currentInfo, LEFT, pq, best);
		}
		if(m.can_go_right(p.first, p.second))
		{
			update_dijkstra(m, currentInfo, RIGHT, pq, best);
		}
	}
	//get the path	
	shared_ptr<info> temp = finalInfo;
	while(temp->previous)
	{
		List.push_front(temp->p);
		temp = temp->previous;
	}

	List.push_front(start);

	mycost = finalCost;

	return List;
}

void calculateMatrix(Maze & m, vector<vector<int>> & matrix, vector<point> & p, vector<vector<list<point>>> & mypath)
{
	for(int i = 0; i < p.size(); ++i)
	{
		for(int j = 0; j < p.size(); ++j)
		{
			if(i == j)
				matrix[i][j] = 0;
			
			else
			{
				mypath[i][j] = solve_dijkstra(m, p[i], p[j], matrix[i][j]);
			}
		}
	}
}

path solve_tour(Maze& m, int rows, int cols)
{
	list<point> List;
	list<point> tracker;
	int myarray[4] = {1, 2, 3, 4};
	int mycost = 0;
	int mincost = INT_MAX;

	vector<vector<int>> matrix(5,vector<int>(5,0));
	vector<vector<list<point>>> mypath(5, vector<list<point>>(5));

	point middle(rows/2, cols/2);
	point leftTop(0, 0);
	point rightTop(0, cols - 1);
	point leftBot(rows - 1, 0);
	point rightBot(rows - 1, cols - 1);

	vector<point> p = {middle, leftTop, rightTop, leftBot, rightBot};
	
	calculateMatrix(m, matrix, p, mypath);

	do
	{
		List = mypath[0][myarray[0]];
		mycost += matrix[0][myarray[0]];

		for(int i = 1; i < 4; ++i)
		{
			list<point> temp = mypath[myarray[i-1]][myarray[i]];
			mycost += matrix[myarray[i-1]][myarray[i]];
			temp.pop_front();
			List.splice(List.end(), temp);
		}
	
		list<point> temp1 = mypath[myarray[3]][0];
		temp1.pop_front();
		List.splice(List.end(), temp1);	
		mycost += matrix[myarray[3]][0];
		
		if(mincost > mycost)
		{
			mincost = mycost;
			tracker = List;
		}
		
		List.clear();
		mycost = 0;

	}while(next_permutation(myarray, myarray + 4));
	
	return tracker;
}
