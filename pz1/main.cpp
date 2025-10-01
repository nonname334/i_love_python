#include <iostream>
#include "structs.h"
#include "func.h"

using namespace std;

void demonstratePoint() {
    cout << "=== Демонстрация работы с точкой ===" << endl;
    Point p = readPoint();
    cout << "Точка: ";
    printPoint(p);
    cout << endl << endl;
}
void demonstrateCircle() {
    cout << "=== Демонстрация работы с кругом ===" << endl;
    Circle c = readCircle();
    printCircle(c);
    cout << endl;
    cout << "Длина окружности: " << circleCircumference(c) << endl;
    cout << "Площадь круга: " << circleArea(c) << endl << endl;
}
void demonstrateSquare() {
    cout << "=== Демонстрация работы с квадратом ===" << endl;
    Square s = readSquare();
    printSquare(s);
    cout << endl;
    cout << "Периметр квадрата: " << squarePerimeter(s) << endl;
    cout << "Площадь квадрата: " << squareArea(s) << endl << endl;
}
void demonstratePointInFigures() {
    cout << "=== Проверка принадлежности точки фигурам ===" << endl;
    Point p = readPoint();
    Circle c = readCircle();
    Square s = readSquare();   
    cout << "Точка ";
    printPoint(p);
    cout << " находится внутри круга: " << (isPointInCircle(p, c) ? "Да" : "Нет") << endl;
    cout << "Точка ";
    printPoint(p);
    cout << " находится внутри квадрата: " << (isPointInSquare(p, s) ? "Да" : "Нет") << endl;
    cout << "Точка ";
    printPoint(p);
    cout << " находится на круге: " << (isPointOnCircle(p, c) ? "Да" : "Нет") << endl;
    cout << "Точка ";
    printPoint(p);
    cout << " находится на квадрате: " << (isPointOnSquare(p, s) ? "Да" : "Нет") << endl << endl;
}
void demonstrateIntersections() {
    cout << "=== Проверка пересечений фигур ===" << endl;
    Circle c1 = readCircle();
    Circle c2 = readCircle();
    Square s1 = readSquare();
    Square s2 = readSquare();   
    cout << "Круги пересекаются: " << (circlesIntersect(c1, c2) ? "Да" : "Нет") << endl;
    cout << "Квадраты пересекаются: " << (squaresIntersect(s1, s2) ? "Да" : "Нет") << endl;
    cout << "Круг и квадрат пересекаются: " << (circleSquareIntersect(c1, s1) ? "Да" : "Нет") << endl << endl;
}
void demonstrateContainment() {
    cout << "=== Проверка принадлежности фигур ===" << endl;
    Circle c1 = readCircle();
    Circle c2 = readCircle();
    Square s1 = readSquare();
    Square s2 = readSquare();   
    cout << "Первый круг внутри второго: " << (isCircleInCircle(c1, c2) ? "Да" : "Нет") << endl;
    cout << "Первый квадрат внутри второго: " << (isSquareInSquare(s1, s2) ? "Да" : "Нет") << endl;
    cout << "Квадрат внутри круга: " << (isSquareInCircle(s1, c1) ? "Да" : "Нет") << endl;
    cout << "Круг внутри квадрата: " << (isCircleInSquare(c1, s1) ? "Да" : "Нет") << endl << endl;
}
int main() {
    demonstratePoint();
    demonstrateCircle();
    demonstrateSquare();
    demonstratePointInFigures();
    demonstrateIntersections();
    demonstrateContainment();
    
    return 0;
}