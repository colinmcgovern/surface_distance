#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

const double FLAT_DIST = 30;
const double HEIGHT_DIST = 11;

typedef struct point{
	int x;
	int y;
	point(int x_,int y_) : x(x_), y(y_) {};
} point;

vector<vector<double> > read_file(char* filename, double width=512, double height=512){
	std::ifstream input( "post.data", std::ios::binary );

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	vector<vector<double> > output;
    vector<double> new_row;

    for(auto &v : buffer){
    	new_row.push_back(double(v));
    	if(new_row.size()==512){
    		output.push_back(new_row);
    		new_row = {};
    	}
    }

    return output;
}

double pt(double x, double y){
	return pow(pow(x,2) + pow(y,2),0.5);
}

double dist(point a, point b){
	return pow(pow(a.x-b.x,2) + pow(a.y-b.y,2) ,0.5);
}

double find_slope(point a, point b){
	return (double(a.y)-double(b.y)) / (double(a.x)-double(b.x));
}

double find_intersect(point a, point b, double slope){
	return double(a.y) - (slope * double(a.x));
}

point intersection(point a, point b, point c, point d)
{
	double slope1 = find_slope(a,b);
	double intersect1 = find_intersect(a,b,slope1);

	double slope2 = find_slope(c,d);
	double intersect2 = find_intersect(c,d,slope2);

	cout << "slope1 " << slope1 << endl;
	cout << "intersect1 " << intersect1 << endl;
	cout << "slope2 " << slope2 << endl;
	cout << "intersect2 " << intersect2 << endl;

	double x = (intersect2 - intersect1) / (slope1 - slope2);
	double y = slope1 * x + intersect1;

	return point(x,y);
}

double surface_dist(vector<vector<double> > surface, point start, point end){

	cout << endl;

	point a1 = intersection(start,end,point(0,1),point(2,1));
	cout << a1.x << " " << a1.y << endl;

	// cout << surface[start.y][start.x] << endl;
	// cout << surface[end.y][end.x] << endl;

	return pt( dist(start,end)*FLAT_DIST, abs(surface[start.y][start.x]-surface[end.y][end.x])*HEIGHT_DIST );
}

void assert(bool x, string message){
	if(!x){
		cout << message << endl;
	}
}

void run_asserts(){

	vector<vector<double> > surf_A = {{0,0,0},{1,1,1},{2,2,2}};

	assert(surface_dist(surf_A,point(0,0),point(0,0))==0,"test 1 failed");
	assert(surface_dist(surf_A,point(0,0),point(1,0))==30,"test 2 failed");
	assert(surface_dist(surf_A,point(0,0),point(2,0))==60,"test 3 failed");
	assert(int(surface_dist(surf_A,point(1,0),point(1,1)))==31,"test 4 failed");
	assert(int(surface_dist(surf_A,point(1,1),point(1,0)))==31,"test 5 failed");
	assert(int(surface_dist(surf_A,point(0,0),point(1,1)))==43,"test 6 failed");
	assert(int(surface_dist(surf_A,point(0,0),point(2,1)))==67,"test 7 failed");

	vector<vector<double> > surf_B = {{1,2,3},{4,5,6},{7,8,9}};

	assert(surface_dist(surf_B,point(0,0),point(0,0))==0,"test 8 failed");
	assert(int(surface_dist(surf_B,point(0,0),point(1,0)))==31,"test 9 failed");
	assert(int(surface_dist(surf_B,point(1,0),point(0,0)))==31,"test 10 failed");
	assert(int(surface_dist(surf_B,point(0,0),point(1,1)))==61,"test 11 failed");

	vector<vector<double> > surf_C = {{0,0,0},{1,1,1},{0,0,0}};

	assert(surface_dist(surf_C,point(0,0),point(0,2))==63,"test 12 failed");
}

int main()
{
    //run_asserts();

    vector<vector<double> > surf_C = {{0,0,0},{1,1,1},{0,0,0}};
    cout << surface_dist(surf_C,point(0,0),point(2,2)) << endl;
}