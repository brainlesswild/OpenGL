#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/transform.hpp>
#include <unistd.h>
#include <math.h>

class Treangle
{
    public:
        
        glm::vec3 vertex_1;
        glm::vec3 vertex_2;
        glm::vec3 vertex_3;
        Treangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3); 
        
};


Treangle::Treangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) :
vertex_1(v1), vertex_2(v2), vertex_3(v3);
{ }


// Пармаетры экрана 
const int width = 800;
const int height = 600;
const int near_z = 1;
const int far_z = 7;
const int devider = width/height;  
float angle = 60.0f;

class Cube
{
    public:
        
        void draw(glm::vec position);
    
    private:
        
        glm::mat4 perspectMatrix = glm::mat4(1.0);
        glm::mat4 rotateMatrix   = glm::mat4(1.0);
        glm::mat4 posMatrix      = glm::mat4(1.0);
        
        glm::mat4 modelMatrix  = glm::mat4(1.0); // главная матрица !!!111
        
        const int number_treangles = 12;
        Treangle cube_tr[12] = {Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, -0.5, -0.5)),
                                Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5)),
                                // front kvadrat
                                
                                Treangle(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, -0.5, 0.5)),
                                Treangle(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5)),
                                // back kvadrat
                                
                                Treangle(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, -0.5, 0.5)),
                                Treangle(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, -0.5)),
                                //right kvadrat
                                
                                Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5, -0.5, 0.5)),
                                Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, -0.5)),
                                //left kvadrat
                                
                                Treangle(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, -0.5)),
                                Treangle(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5)),
                                //up kvadrat
                                
                                Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.5, -0.5, -0.5)),
                                Treangle(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, 0.5), glm::vec3(-0.5, -0.5, 0.5)) 
                                }; //VOVA TI DOWN KVADRAT
                                
        void CalculateModelMatrix();
        void calculatePerspective();
        void rotateX(float angle);
                            
};

void Cube::calculatePerspective()
{
    perspectMatrix = glm::perspective(angle, devider, near_z, far_z);
}
    

void Cube::rotateX(float angle)
{
    glm::mat4 changeMatrix = glm::mat4(1.0, 0.0,                  0.0, 0.0
                                       0.0,   cos(angle), -sin(angle), 0.0,
                                       0.0,   sin(angle),  cos(angle), 0.0,
                                       0.0,         0.0,   0.0,        1.0);  
    rotateMatrix = rotateMatrix * changeMatrix;
}


void Cube::CalculateModelMatrix()
{
    calculatePerspective();
    modelMatrix = rotateMatrix * posMatrix * perspectMatrix;
}


void Cube::draw(glm::vec3 position)
{
    CalculateModelMatrix();
    
    for(int i = 0; i < number_treangles; i++)
    {
        
        glm::vec3 changed_vertex_1 = modelMatrix * cube_tr[i].vertex_1 + position;
        glm::vec3 changed_vertex_2 = modelMatrix * cube_tr[i].vertex_2 + position;
        glm::vec3 changed_vertex_3 = modelMatrix * cube_tr[i].vertex_3 + position;
        
        printf("Vertex1: %f %f %f\n", changed_vertex_1.x, changed_vertex_1.y, changed_vertex_1.z);
        
        glBegin(GL_TRIANGLES);
        glColor3f(0.1 + 0.04*i, 0.2 + 0.02*i, 0.04*i);
        
        for(int i = 0; i < 12; i++)
        { 
            glVertex3f(glm::vec3( modelMatrix * glm::vec4(cube_tr[i].vertex_1, 1)), glm::vec3( modelMatrix * glm::vec4(cube_tr[i].vertex_2, 1)), glm::vec3(modelMatrix * glm::vec4(cube_tr[i].vertex_3, 1)));
        }
        
        glEnd();
    }
}


int main ()
{
    
    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    	window.setVerticalSyncEnabled(true);
    
    window.setActive(true);
    
    Cube my_cube;
    
    float    x = 0;
    float    y = 0;
    
    while(window.isOpen())
    {
        //sleep(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        my_cube.draw(glm::vec3(x, y, 0));
        my_cube.rotateX(0.01);
        window.display();
        
    }

    return 0;
}
    
    
    
