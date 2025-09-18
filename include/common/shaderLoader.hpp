#pragma once

#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

ShaderProgramSource parseShader(const std::string& filepath);
unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

