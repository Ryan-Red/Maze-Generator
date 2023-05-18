#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <time.h>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>





struct Node{
    int x;
    int y;
    std::vector<std::vector<int>> adj;
    int visited;
};


template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) 
{
    os << "[";
    for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
    {
        os << " " << *ii;
    }
    os << " ]";
    return os;
}

// A utility function to add an edge in an
// undirected graph.
void addEdge(std::vector<std::vector<Node>> &nodesList, std::vector<int> first, std::vector<int> second)
{
    // std::cout << "Added" << std::endl;
    // std::cout << first << second << std::endl;
    nodesList[first[1]][first[0]].adj.emplace_back(second);
    nodesList[second[1]][second[0]].adj.emplace_back(first);

    // std::cout << nodesList[first[0]][first[1]].adj << std::endl;

}
 
// A utility function to print the adjacency list
// representation of graph
void printGraph(std::vector<std::vector<Node>> nodesList, int n, int m)
{
    int i, j = 0;

    for(i = 0; i < n; i ++){
        for(j = 0; j < m; j++){
            std::vector<std::vector<int>> v = nodesList[i][j].adj;
            std::cout << "\n Adjacency list of vertex (" << i << ", " << j << ")\n head ";

            for (auto x : v)
                std::cout << " -> (" << x[0] << ", " << x[1] << ")";
            std::cout << std::endl;


        }
    }

}
 
void drawCircle(cv::Mat &whiteMatrix, std::vector<int> centerCoordinates, int radius = 50){
   
   cv::Point center(centerCoordinates[1], centerCoordinates[0]);//Declaring the center point
   cv::Scalar line_Color(0, 0, 0);//Color of the circle
   int thickness = 5;//thickens of the line
   cv::circle(whiteMatrix, center, radius, line_Color, thickness);//Using circle()function to draw the line//
  
}
void drawLine(cv::Mat &whiteMatrix, std::vector<int> p1, std::vector<int> p2){
    cv::Point point1(p1[1],p1[0]);
    cv::Point point2(p2[1],p2[0]);

    cv::Scalar line_Color(0, 0, 255);//Color of line
    int thickness = 5;//thickens of the line
    cv::line(whiteMatrix, point1, point2, line_Color,thickness, cv::LINE_AA);

}


// Driver code
int main(int argc, char* argv[])
{


    srand( (unsigned)time(NULL) );
    int n = 4;
    int m = 3;

    if (argc >= 3)
    {
        std::istringstream iss( argv[1] );
        std::istringstream iss2( argv[2] );
        int val;

        if (iss >> n && iss2 >> m)
        {
            std::cout << "n is " << n << " m is " << m << std::endl;
            // Conversion successful
        }else{
            std::cout << "Invalid input" << std::endl;
            return -1;
        }
    }


    std::vector<std::vector<Node>> nodesList(n,std::vector<Node>(m));

    int i, j = 0;

    for(i = 0; i < n; i ++){
        for(j = 0; j < m; j++){
            nodesList[i][j].x = j;
            nodesList[i][j].y = i;
            nodesList[i][j].visited = 0;
        }
    }

    std::vector<int> curCell = {0,0};
    std::vector<std::vector<int>> stack;
    std::vector<std::vector<int>> options;

    nodesList[curCell[1]][curCell[0]].visited = 1;
    stack.emplace_back(curCell);
    while(stack.size() != 0){

        options.clear();
        curCell = stack.back();
        stack.pop_back();

        auto potCell = curCell;

        int cur_x = curCell[0];
        int cur_y = curCell[1];

        if(cur_x > 0){
            potCell = {cur_x - 1, cur_y};
            if(nodesList[potCell[1]][potCell[0]].visited == 0){
                options.emplace_back(potCell);
            }
        }
        if(cur_x < (m-1)){
            potCell = {cur_x + 1, cur_y};
            if(nodesList[potCell[1]][potCell[0]].visited == 0){
                options.emplace_back(potCell);
            }
        }
        if(cur_y > 0){
            potCell = {cur_x, cur_y - 1};
            if(nodesList[potCell[1]][potCell[0]].visited == 0){
                options.emplace_back(potCell);
            }
        }
        if(cur_y < (n-1)){
            potCell = {cur_x, cur_y + 1};
            if(nodesList[potCell[1]][potCell[0]].visited == 0){
                options.emplace_back(potCell);
            }
        }


      
        if(options.size() > 0){
            stack.emplace_back(curCell);
            int randomPick = rand() % (options.size());
            

            std::cout << stack <<  randomPick << std::endl;
            auto pickedCell = options[randomPick];
            std::cout << "Picked!" << std::endl;

            addEdge(nodesList, curCell, pickedCell);
            std::cout << "Added the edge" << std::endl;
            nodesList[pickedCell[1]][pickedCell[0]].visited = 1;
            stack.emplace_back(pickedCell);


        }


    }
    printGraph(nodesList, n, m);
    int vert_mult = static_cast<int>(1200.0f/(m+1));
    int horiz_mult = static_cast<int>(1200.0f/(n+1));

    cv::Mat whiteMatrix(1300, 1300, CV_8UC3, cv::Scalar(255, 255, 255));//Declaring a white matrix
    cv::namedWindow("whiteMatrix");//Declaring a window to show the circle
  
    for(i = 0; i < n; i ++){
        for(j = 0; j < m; j++){
            int cur_x = nodesList[i][j].x;
            int cur_y = nodesList[i][j].y;
            drawCircle(whiteMatrix, {cur_x * vert_mult + 150, cur_y * horiz_mult + 150}, 20);

            std::vector<std::vector<int>> v = nodesList[i][j].adj;

            for (auto x : v)
                drawLine(whiteMatrix, {cur_x * vert_mult + 150, cur_y* horiz_mult + 150 }, {x[0] * vert_mult + 150, x[1] * horiz_mult + 150});

                // std::cout << " -> (" << x[0] << ", " << x[1] << ")";

            // cv::line(whiteMatrix, )
            
        }
    }
   
   cv::imshow("WhiteMatrix", whiteMatrix);//Showing the circle//
   cv::waitKey(0);//Waiting for Keystroke//



    return 0;
}