#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <tiny_obj_loader.h>

#include "mesh.hpp"

ogld::obj_model::obj_model(std::string const& path) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string error_string;
    if (!tinyobj::LoadObj(shapes, materials, error_string, (static_cast<std::stringstream&>(std::stringstream() << std::ifstream(path).rdbuf()).str()).c_str())) { 
        throw std::runtime_error(std::string("failed to load '") + path + std::string("'.\n\terror_string: ") + error_string); 
    }

    for (auto const& shape : shapes) {
        for (auto const& index : shape.mesh.indices) {
            vertex_data.push_back(shape.mesh.positions.at(index * 3));
            vertex_data.push_back(shape.mesh.positions.at(index * 3 + 1));
            vertex_data.push_back(shape.mesh.positions.at(index * 3 + 2));

            vertex_data.push_back(shape.mesh.normals.at(index * 3));
            vertex_data.push_back(shape.mesh.normals.at(index * 3 + 1));
            vertex_data.push_back(shape.mesh.normals.at(index * 3 + 2));
        }
    }
}