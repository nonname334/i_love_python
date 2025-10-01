#include "structs.h"
#include <cmath>
#include <iostream>

using namespace std;

const double EPSILON = 1e-5;

bool areEqual(double a, double b) {
    return fabs(a - b) < EPSILON;
}

double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
Point readPoint() {
    Point p;
    cout << "x y: ";
    cin >> p.x >> p.y;
    return p;
}

void printPoint(const Point& p) {
    cout << "(" << p.x << ", " << p.y << ")";
}

Circle readCircle() {
    Circle c;
    cout << "Центр круга: ";
    cin >> c.center.x >> c.center.y;
    cout << "Радиус круга: ";
    cin >> c.radius;
    return c;
}

void printCircle(const Circle& c) {
    cout << "Круг: центр ";
    printPoint(c.center);
    cout << ", радиус = " << c.radius;
}

double circleCircumference(const Circle& c) {
    return 2 * M_PI * c.radius;
}

double circleArea(const Circle& c) {
    return M_PI * c.radius * c.radius;
}
Square readSquare() {
    Square s;
    cout << "Левый верхний угол квадрата: ";
    cin >> s.topLeft.x >> s.topLeft.y;
    cout << "длина стороны квадрата: ";
    cin >> s.side;
    return s;
}
void printSquare(const Square& s) {
    cout << "Квадрат: левый верхний угол ";
    printPoint(s.topLeft);
    cout << ", сторона = " << s.side;
}
double squarePerimeter(const Square& s) {
    return 4 * s.side;
}
double squareArea(const Square& s) {
    return s.side * s.side;
}
bool isPointInCircle(const Point& p, const Circle& c) {
    return distance(p, c.center) < c.radius - EPSILON;
}
bool isPointInSquare(const Point& p, const Square& s) {
    return (p.x > s.topLeft.x + EPSILON) && 
           (p.x < s.topLeft.x + s.side - EPSILON) &&
           (p.y < s.topLeft.y - EPSILON) && 
           (p.y > s.topLeft.y - s.side + EPSILON);
}
bool isPointOnCircle(const Point& p, const Circle& c) {
    return fabs(distance(p, c.center) - c.radius) < EPSILON;
}
bool isPointOnSquare(const Point& p, const Square& s) {
    bool onLeft = areEqual(p.x, s.topLeft.x) && (p.y <= s.topLeft.y) && (p.y >= s.topLeft.y - s.side);
    bool onRight = areEqual(p.x, s.topLeft.x + s.side) && (p.y <= s.topLeft.y) && (p.y >= s.topLeft.y - s.side);
    bool onTop = areEqual(p.y, s.topLeft.y) && (p.x >= s.topLeft.x) && (p.x <= s.topLeft.x + s.side);
    bool onBottom = areEqual(p.y, s.topLeft.y - s.side) && (p.x >= s.topLeft.x) && (p.x <= s.topLeft.x + s.side);
    return onLeft || onRight || onTop || onBottom;
}
bool circlesIntersect(const Circle& c1, const Circle& c2) {
    double dist = distance(c1.center, c2.center);
    return dist < c1.radius + c2.radius + EPSILON && 
           dist > fabs(c1.radius - c2.radius) - EPSILON;
}
bool squaresIntersect(const Square& s1, const Square& s2) {
    bool noOverlap = (s1.topLeft.x + s1.side < s2.topLeft.x - EPSILON) ||
                    (s2.topLeft.x + s2.side < s1.topLeft.x - EPSILON) ||
                    (s1.topLeft.y - s1.side > s2.topLeft.y + EPSILON) ||
                    (s2.topLeft.y - s2.side > s1.topLeft.y + EPSILON);
    return !noOverlap;
}
bool circleSquareIntersect(const Circle& c, const Square& s) {
    Point corners[4] = {
        s.topLeft,
        {s.topLeft.x + s.side, s.topLeft.y},
        {s.topLeft.x + s.side, s.topLeft.y - s.side},
        {s.topLeft.x, s.topLeft.y - s.side}
    };
    if (isPointInSquare(c.center, s)) return true;
    for (int i = 0; i < 4; i++) {
        Point p1 = corners[i];
        Point p2 = corners[(i + 1) % 4];
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        double fx = p1.x - c.center.x;
        double fy = p1.y - c.center.y;
        double a = dx * dx + dy * dy;
        double b = 2 * (fx * dx + fy * dy);
        double c_val = fx * fx + fy * fy - c.radius * c.radius;
        double discriminant = b * b - 4 * a * c_val;
        if (discriminant >= -EPSILON) {
            discriminant = max(0.0, discriminant);
            double t1 = (-b - sqrt(discriminant)) / (2 * a);
            double t2 = (-b + sqrt(discriminant)) / (2 * a);
            
            if ((t1 >= -EPSILON && t1 <= 1.0 + EPSILON) || 
                (t2 >= -EPSILON && t2 <= 1.0 + EPSILON)) {
                return true;
            }
        }
    }
    return false;
}
bool isCircleInCircle(const Circle& c1, const Circle& c2) {
    double dist = distance(c1.center, c2.center);
    return dist + c1.radius < c2.radius + EPSILON;
}
bool isSquareInSquare(const Square& s1, const Square& s2) {
    return (s1.topLeft.x >= s2.topLeft.x - EPSILON) &&
           (s1.topLeft.x + s1.side <= s2.topLeft.x + s2.side + EPSILON) &&
           (s1.topLeft.y <= s2.topLeft.y + EPSILON) &&
           (s1.topLeft.y - s1.side >= s2.topLeft.y - s2.side - EPSILON);
}
bool isSquareInCircle(const Square& s, const Circle& c) {
    Point corners[4] = {
        s.topLeft,
        {s.topLeft.x + s.side, s.topLeft.y},
        {s.topLeft.x + s.side, s.topLeft.y - s.side},
        {s.topLeft.x, s.topLeft.y - s.side}
    };
    for (int i = 0; i < 4; i++) {
        if (distance(corners[i], c.center) > c.radius - EPSILON) {
            return false;
        }
    }
    return true;
}
bool isCircleInSquare(const Circle& c, const Square& s) {
    Point closest;
    closest.x = max(s.topLeft.x, min(c.center.x, s.topLeft.x + s.side));
    closest.y = max(s.topLeft.y - s.side, min(c.center.y, s.topLeft.y));
    return (c.center.x - c.radius >= s.topLeft.x - EPSILON) &&
           (c.center.x + c.radius <= s.topLeft.x + s.side + EPSILON) &&
           (c.center.y - c.radius >= s.topLeft.y - s.side - EPSILON) &&
           (c.center.y + c.radius <= s.topLeft.y + EPSILON);
}