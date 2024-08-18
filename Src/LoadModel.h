
#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include <vector>

// 顶点数据结构
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Model
{
public:
    int LoadOBJ(std::string& file,const char* texture);
    void render() const;
    void cleanup();
private:
    void loadModel(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    unsigned int createShaderProgram();
};






#endif // !LOADMODEL_H