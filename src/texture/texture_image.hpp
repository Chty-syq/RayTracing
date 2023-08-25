//
// Created by syq on 23-8-25.
//

#pragma once
#include "texture/texture.hpp"

class TextureImage: public Texture {
private:
    int width{}, height{}, channel{};
    unsigned char* bytes{};
public:
    TextureImage() = default;
    explicit TextureImage(const std::string &path);
    ~TextureImage() override = default;

    glm::vec3 Sample(float u, float v, glm::vec3 p) const override;
};

TextureImage::TextureImage(const std::string &path) {
    bytes = stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (bytes == nullptr) {
        throw std::runtime_error("Unexpected behavior loading from " + path);
    }
}

glm::vec3 TextureImage::Sample(float u, float v, glm::vec3 p) const {
    int row = (int)(u * (float)width);
    int col = (int)(v * (float)height);
    row = std::min(std::max(row, 0), width - 1);
    col = std::min(std::max(col, 0), height - 1);
    int index = (col * width + row) * channel;
    float r = (float)(bytes[index + 0]) / 255.0f;
    float g = (float)(bytes[index + 1]) / 255.0f;
    float b = (float)(bytes[index + 2]) / 255.0f;
    return { r, g, b };
}
