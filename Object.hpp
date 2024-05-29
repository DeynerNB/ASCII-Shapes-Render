#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <math.h>

class Object {
private:
    std::vector<std::vector<int>> Points;

    int max_Depth = 0;
    int min_Depth = 0;

public:
    Object();
    ~Object();

    void addPoint(std::vector<int> new_point);
    std::vector<std::vector<int>> getPoints();

    void createCube(int cube_Size);
    void createSphere(int radius);
    void createCone(int height);

    int getMaxDepth();
    int getMinDepth();
};

Object::Object() {}
Object::~Object() {}

void Object::addPoint(std::vector<int> new_point) {
    this->Points.push_back(new_point);
}

std::vector<std::vector<int>> Object::getPoints() {
    return this->Points;
}

void Object::createCube(int cube_Size) {
    int startPointX = -(cube_Size / 2);
    int startPointY = -(cube_Size / 2);
    int cubeWidth = cube_Size / 2;
    int temp_CubeWidth = cubeWidth;

    for (int i = 0; i < cube_Size; i += 2) {
        this->addPoint( {startPointX + i, startPointY,   cubeWidth} );
        this->addPoint( {startPointX, startPointY + i,   cubeWidth} );
        this->addPoint( {-startPointX - i, -startPointY, cubeWidth} );
        this->addPoint( {-startPointX, -startPointY - i, cubeWidth} );

        this->addPoint( {startPointX + i, startPointY,   -cubeWidth} );
        this->addPoint( {startPointX, startPointY + i,   -cubeWidth} );
        this->addPoint( {-startPointX - i, -startPointY, -cubeWidth} );
        this->addPoint( {-startPointX, -startPointY - i, -cubeWidth} );

        this->addPoint( { startPointX,  startPointY, temp_CubeWidth} );
        this->addPoint( {-startPointX,  startPointY, temp_CubeWidth} );
        this->addPoint( { startPointX, -startPointY, temp_CubeWidth} );
        this->addPoint( {-startPointX, -startPointY, temp_CubeWidth} );

        temp_CubeWidth -= 2;
    }

    // Define Max and Min depth for lighting
    max_Depth = (cube_Size * (cube_Size * 0.3));
    min_Depth = -(cube_Size * (cube_Size * 0.3));
}

void Object::createSphere(int radius) {
    float angle = 0.0;
    float fixed_Angle = 0.0;

    int a = 0;
    int b = 0;
    int c = 0;

    while (fixed_Angle < (2 * M_PI)) {

        angle = 0.0;

        while (angle < M_PI) {
            a = radius * sin(angle) * cos(fixed_Angle);
            b = radius * sin(angle) * sin(fixed_Angle);
            c = radius * cos(angle);

            angle += 0.2;
            this->addPoint( {a, b, c} );
        }
        fixed_Angle += 0.50;
    }

    fixed_Angle = 0.0;

    while (fixed_Angle < (2 * M_PI)) {
        a = radius * cos(fixed_Angle);
        b = radius * sin(fixed_Angle);

        fixed_Angle += 0.2;
        this->addPoint( {a, b, 0} );
    }

    // Define Max and Min depth for lighting
    max_Depth = radius;
    min_Depth = -radius;
}

void Object::createCone(int height) {

    float angle = 0.0;
    int current_H = 0;
    int increase_H_Factor = 3;

    // Add the point 0, 0, 0 to the shape
    int a = 0;
    int b = 0;
    int c = 0;

    this->addPoint( {a, b, c} );

    // Built up the rest of the shape
    while (current_H <= height) {
        
        current_H += increase_H_Factor;
        while(angle < (2 * M_PI)) {
            a = current_H * cos(angle);
            b = current_H * sin(angle);
            c = current_H;

            this->addPoint( {a, b, c} );
            angle += 0.2;
        }
        angle = 0.0;
    }

    // Define Max and Min depth for lighting
    max_Depth = (2 * height);
    min_Depth = -(2 * height);
}

int Object::getMaxDepth() {
    return this->max_Depth;
}
int Object::getMinDepth() {
    return this->min_Depth;
}


#endif