#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

const double FLAT_DIST = 30;
const double HEIGHT_DIST = 11;

class point{

	public:

		double x;
		double y;
		point(double x_,double y_) : x(x_), y(y_) {};

		bool operator==(const point &other) const {
			if(x==other.x && y==other.y){
				return 1;
			}
			return 0;
		}

		bool operator!=(const point &other) const {
			if(*this==other){
				return 0;
			}
			return 1;
		}

		string str(){
			return to_string(x) + ", " + to_string(y);
		}
};

class point3{

	public:

		double x;
		double y;
		double z;
		point3(double x_,double y_, double z_) : x(x_), y(y_), z(z_) {};

		bool operator==(const point3 &other) const {
			if(x==other.x && y==other.y && z==other.z){
				return 1;
			}
			return 0;
		}

		bool operator!=(const point3 &other) const {
			if(*this==other){
				return 0;
			}
			return 1;
		}

		string str(){
			return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
		}
};

bool is_error(point p){
	if(p.x == -1 && p.y == -1){
		return true;
	}
	return false;
}

bool is_error(point3 p){
	if(p.x == -1 && p.y == -1){
		return true;
	}
	return false;
}

vector<vector<double> > read_file(char* filename, double width=512, double height=512){
	std::ifstream input( filename, std::ios::binary );

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

double dist(point3 a, point3 b){
	return pow(pow(a.x-b.x,2) + pow(a.y-b.y,2) + pow(a.z-b.z,2) ,0.5);
}

double find_slope(point a, point b){

	if(a.y == b.y){
		return 0;
	}

	return (double(a.y)-double(b.y)) / (double(a.x)-double(b.x));
}

double find_intersect(point a, point b, double slope){
	return double(a.y) - (slope * double(a.x));
}

point is_possible(point p, point a, point b, point c, point d){

	bool ab_pass = 0;

	if((p.x <= a.x && p.x >= b.x) || (p.x >= a.x && p.x <= b.x)){
		if((p.y <= a.y && p.y >= b.y) || (p.y >= a.y && p.y <= b.y)){
			ab_pass = 1;
		}
	}

	bool cd_pass = 0;

	if((p.x <= c.x && p.x >= d.x) || (p.x >= c.x && p.x <= d.x)){
		if((p.y <= c.y && p.y >= d.y) || (p.y >= c.y && p.y <= d.y)){
			cd_pass = 1;
		}
	}

	if(ab_pass && cd_pass){
		return p;
	}else{
		return point(-1,-1);
	}
}

point intersection(point a, point b, point c, point d)
{

	//Only two equalvalent points
	if(a==c && b!=d){
		return a;
	}

	if(a==d && b!=a){
		return a;
	}

	if(b==c && a!=d){
		return b;
	}

	if(b==d && a!=c){
		return b;
	}

	//Equal points
	if((a.x==b.x && a.y==b.y)||(c.x==d.x && c.y==d.y)){
		return point(-1,-1);
	}

	//Vertical line
	if(a.x==b.x && c.x==d.x){
		return point(-1,-1);
	}

	if(a.x==b.x){
		double slope2 = find_slope(c,d);
		double intersect2 = find_intersect(c,d,slope2);

		point output(0,0);
		output.x = a.x;
		output.y = slope2 * a.x + intersect2;

		return is_possible(output,a,b,c,d);
	}


	if(c.x==d.x){
		double slope1 = find_slope(a,b);
		double intersect1 = find_intersect(a,b,slope1);

		point output(0,0);
		output.x = c.x;
		output.y = slope1 * c.x + intersect1;

		return is_possible(output,a,b,c,d);
	}

	double slope1 = find_slope(a,b);
	double intersect1 = find_intersect(a,b,slope1);

	double slope2 = find_slope(c,d);
	double intersect2 = find_intersect(c,d,slope2);

	//Parallel
	if(slope1 == slope2){
		return point(-1,-1);
	}

	double x = (intersect2 - intersect1) / (slope1 - slope2);
	double y = slope1 * x + intersect1;

	return is_possible(point(x,y),a,b,c,d);
}

bool is_int(double x){
	return x == double(int(x));
}

double lerp(double a, double b, double f)
{
    return a + f * (b - a);
}

vector<point3> find_heights(vector<vector<double> > surface, vector<point> intersects){

	vector<point3> output;

	for(point &v : intersects){

		if(is_error(v))continue;

		//On Grid
		if( is_int(v.x) && is_int(v.y) ){

			output.push_back(point3(v.x,v.y,surface[int(v.y)][int(v.x)]));
		}

		//On Horizontal
		if( is_int(v.x) && !is_int(v.y) ){

			double height = lerp( surface[int(v.y)][int(v.x)], surface[int(v.y)+1][int(v.x)], v.y - double(int(v.y)) );

			output.push_back(point3(v.x,v.y,height));
		}

		//On Vertical
		if( !is_int(v.x) && is_int(v.y) ){



			double height = lerp( surface[int(v.y)][int(v.x)], surface[int(v.y)][int(v.x)+1], v.x - double(int(v.x)) );

			output.push_back(point3(v.x,v.y,height));
		}

		//On Diagonal
		if( !is_int(v.x) && !is_int(v.y) ){

			double height = lerp( 	surface[int(v.y)+1][int(v.x)],
								 	surface[int(v.y)][int(v.x)+1],
								 	pt(v.x - double(int(v.x)),v.y - double(int(v.y)))/pow(2,0.5)
								);

			output.push_back(point3(v.x,v.y,height));
		}
	}

	return output;
}

double surface_dist(vector<vector<double> > surface, point start, point end){

	if(surface.size()==0 || surface[0].size()==0){
		cout << "Bad surface size" << endl;
		return 0.0;
	} 

	vector<point> intersects;

	//Horizontal intersections
	for(int i=0;i<surface.size();i++){
		point output = intersection(start,end,point(0,i),point(surface[i].size()-1,i));
		intersects.push_back(output);
	}

	//Vertical intersections
	for(int i=0;i<surface[0].size();i++){
		point output = intersection(start,end,point(i,0),point(i,surface.size()-1));
		intersects.push_back(output);
	}

	//Horizontal Diagonal intersections
	for(int i=1;i<surface.size();i++){
		point output = intersection(start,end,point(0,i),point(i,0));
		intersects.push_back(output);
	}

	//Vertical Diagonal intersections
	for(int i=1;i<surface[0].size();i++){
		point output = intersection(start,end,point(surface.size()-1,i),point(i,surface[0].size()-1));
		intersects.push_back(output);
	}

	//Find heights
	vector<point3> intersects3 = find_heights(surface, intersects);

	//Sort by distance from start
	std::sort(intersects3.begin(), intersects3.end(), 
	    [&](const point3 & a, const point3 & b) -> bool
	{ 	
		point a2(a.x,a.y);
		point b2(b.x,b.y);
	    return dist(a2,start) < dist(b2,start); 
	});

	//Convert to scale
	vector<point3> scaled_intersects3;
	for(auto&v : intersects3){
		if(!is_error(v)){
			scaled_intersects3.push_back(point3(v.x*FLAT_DIST, v.y*FLAT_DIST, v.z*HEIGHT_DIST));
		}
	}
	intersects3 = scaled_intersects3;

	//Calculate dists
	if(intersects3.size()<2){
		return 0;
	}

	double output = 0;
	point3 last_point = intersects3[0];
	for(point3 &v : intersects3){
		//cout << last_point.str() << " -> " << v.str() << " " << dist(last_point,v) << endl;
		output += dist(last_point,v);
		last_point = v;
	}

	return output;
}

void assert(bool x, string message){
	if(!x){
		cout << message << endl;
	}
}

void run_intersection_asserts(){
	assert(intersection(point(0,0),point(1,1),point(0,1),point(1,0))==point(0.5,0.5),"test 1 failed");

	assert(intersection(point(0,0),point(0,1),point(0,1),point(1,0))==point(0,1),"test 2 failed");

	assert(intersection(point(0,5),point(10,5),point(5,1),point(5,100))==point(5,5),"test 3 failed");

	assert(intersection(point(6,5),point(6,50),point(5,1),point(5,100))==point(-1,-1),"test 4 failed");

	assert(intersection(point(0,3),point(6,3),point(5,1),point(5,2))==point(-1,-1),"test 5 failed");

	assert(intersection(point(0,0),point(1,1),point(2,2),point(4,2))==point(-1,-1),"test 6 failed");

	assert(intersection(point(0,0),point(1,1),point(2,2),point(3,3))==point(-1,-1),"test 7 failed");

	assert(intersection(point(0,0),point(1,1),point(0,0),point(1,2))==point(0,0),"test 8 failed");
}

void run_surf_asserts(){

	vector<vector<double> > surf_A = {{0,0,0},{1,1,1},{2,2,2}};

	assert(surface_dist(surf_A,point(0,0),point(0,0))==0,"test 1 failed");
	assert(surface_dist(surf_A,point(0,0),point(1,0))==30,"test 2 failed");
	assert(surface_dist(surf_A,point(0,0),point(2,0))==60,"test 3 failed");
	assert(int(surface_dist(surf_A,point(1,0),point(1,1)))==31,"test 4 failed");
	assert(int(surface_dist(surf_A,point(1,1),point(1,0)))==31,"test 5 failed");
	assert(int(surface_dist(surf_A,point(0,0),point(1,1)))==43,"test 6 failed");
	assert(int(surface_dist(surf_A,point(0,0),point(2,1)))==68,"test 7 failed");

	vector<vector<double> > surf_B = {{1,2,3},{4,5,6},{7,8,9}};

	assert(surface_dist(surf_B,point(0,0),point(0,0))==0,"test 8 failed");
	assert(int(surface_dist(surf_B,point(0,0),point(1,0)))==31,"test 9 failed");
	assert(int(surface_dist(surf_B,point(1,0),point(0,0)))==31,"test 10 failed");
	assert(int(surface_dist(surf_B,point(0,0),point(1,1)))==61,"test 11 failed");

	vector<vector<double> > surf_C = {{0,0,0},{1,1,1},{0,0,0}};

	assert(int(surface_dist(surf_C,point(0,0),point(0,2)))==63,"test 12 failed");

	vector<vector<double> > surf_D = {{10,15},{11,14},{12,13}};

	assert(int(surface_dist(surf_C,point(0,0),point(1,2)))==63,"test 12 failed");
}

int main(int argc, char** argv)
{

 	//run_intersection_asserts();
    //run_surf_asserts();

    if(argc != 6){
   		cout << "correct format './main filename.data x1 y1 x2 y2'" << endl;
    }else{
    	cout << surface_dist(	read_file(argv[1]),
    							point(stod(argv[2]),stod(argv[3])),
    							point(stod(argv[4]),stod(argv[5]))
    						) 
    	<< endl;
    }

}