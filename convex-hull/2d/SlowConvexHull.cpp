#include <iostream>
#include <vector>

struct Point {
	float x;
	float y;

	Point operator-(const Point& other) {
		Point temp;
		temp.x = x - other.x;
		temp.y = y - other.y;
		return temp;
	}

};

inline bool operator==(const Point& lhs, const Point& rhs) {return (lhs.x == rhs.x) && (lhs.y == rhs.y);}
inline bool operator!=(const Point& lhs, const Point& rhs) {return !(lhs == rhs);}

std::ostream& operator<<(std::ostream& os, const Point& obj)
{
	os << "(" << obj.x << ", " << obj.y << ")";
	return os;
}

struct Line{
	Point p;
	Point q;

	// Line(const Line& other) : p(other.p), q(other.q) {}
};

std::ostream& operator<<(std::ostream& os, const Line& obj)
{
	os << "p:(" << obj.p.x << ", " << obj.p.y << "), q:(" << obj.q.x << ", " << obj.q.y << ")";
	return os;
}

bool pointRightOfLine(const Point& point, Line& line)
{
	auto& A{line.p};
	auto& B{line.q};
	auto& P{point};
	auto d = ((P.x - A.x)*(B.y - A.y) - (P.y - A.y)*(B.x - A.x));
	return d > 0 ? true : false;
}

void sortLineSegments(std::vector<Line>& lines)
{
	for (int i = 1; i < lines.size(); i++) { 
		auto& last_point{lines.at(i-1).q};
		// Loop through rest of vector while holding this one
		for (int j = i; j < lines.size(); j++) {
			auto& current_line{lines.at(j)};
			if (last_point != current_line.p) {
				// Loop through next points to find which comes next
				continue;
			}
			
			// Swap elements
			Line temp_swap{current_line};
			current_line = lines.at(i);
			lines.at(i) = temp_swap;
			break;
		}
	}
}


/**
* input: A set of points in the plane
* output: A list containing the vertices of CH(input) in clockwise order.
*
* Cannot handle degenerate cases, i.e 3 points on a line becomes invalid linesegemtns
*/
void slowConvexHull(const std::vector<Point>& input, std::vector<Point>& output)
{
	std::vector<Line> line_segments;
	for (auto p = input.begin(); p != input.end(); p++) {
		for (auto q = input.begin(); q != input.end(); q++) {
			if (q == p)
				continue;
			bool valid{true};
			Line line{*p, *q};

			for (const Point& r : input) {
				if ((r == *p) || (r == *q))
					continue;

				if (!pointRightOfLine(r, line))
					valid = false;
			}

			if (valid) {
				line_segments.push_back(line);
			}
		}
	}

	std::cout << "valid line segments: " << line_segments.size() << "\n";
	sortLineSegments(line_segments);
	for (const auto& line : line_segments) {
		output.push_back(line.p);
	}
}

int main(int argc, char* argv[])
{
	// Write some tests
	Point a{2,1};
	Point b{3,2};
	Line l{a,b};
	Point p_right{4,1};
	Point p_left{1,3};

	std::cout << "Point: " << p_right << " to the right of line: " << pointRightOfLine(p_right, l) << "\n";
	std::cout << "Point: " << p_left << " to the left of line: " << !pointRightOfLine(p_left, l) << "\n";

	std::vector<Point> input = {a,p_right, p_left, {4,2}, {5,4}};
	std::vector<Point> input2 = {{0,0},{2,3}, {5,4}, {3,2}, {1,1},{3,0}};
	std::vector<Point> output;

	slowConvexHull(input2, output);
	for (auto point : output) {
		std::cout << point << "\n";
	}


	return 0;
}
