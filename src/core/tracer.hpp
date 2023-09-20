//
// Created by syq on 23-8-23.
//

#pragma once
#include <tbb/parallel_for.h>

class Tracer {
private:
    int depth_limit{};
    int width, height, channel{};

public:
    unsigned char* image;
    Tracer();
    ~Tracer() = default;
    glm::vec3 Tracing(const Ray& ray, int depth);
    void DrawPixel(int row, int col, glm::vec4 color) const;
    void Render();
};

Tracer::Tracer() {
    this->width = SCREEN_WIDTH;
    this->height = SCREEN_HEIGHT;
    this->channel = 4;
    this->depth_limit = 50;
    this->image = new unsigned char[width * height * channel];
}

void Tracer::Render() {
    auto time_start = std::chrono::high_resolution_clock::now();
    std::atomic<size_t> counter = 0;

    cout << "Ray-tracing is running" << endl;
    tbb::parallel_for(tbb::blocked_range<int>(0, height * width, 692), [&](tbb::blocked_range<int>& r) {
        for(int index = r.begin(); index != r.end(); ++index) {
            int i = index / width;
            int j = index % width;
            auto color = glm::vec4(0.0f);
            for(int sps = 0; sps < NUM_SAMPLE_RAYS; ++sps) {
                float u = ((float)j + MagicRandom::Float(0, 1)) / (float)width;
                float v = ((float)i + MagicRandom::Float(0, 1)) / (float)height;
                Ray ray = Scene::camera->GetRay(u, v);
                color += glm::vec4(Tracing(ray, 0), 1.0f);
            }
            color /= (float)NUM_SAMPLE_RAYS;
            color.w = 1.0f;
            if (color.r != color.r)  color.r = 0.0f;
            if (color.g != color.g)  color.g = 0.0f;
            if (color.b != color.b)  color.b = 0.0f;
            color = glm::vec4(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);  //gamma校正
            DrawPixel(i, j, color);
            utils::ShowProgressRate((float)(++counter) / (float)(height * width));
        }
    });

    auto save_dir = fs::current_path().parent_path() / "ray_tracing.png";
    stbi_flip_vertically_on_write(true);
    stbi_write_png(save_dir.c_str(), width, height, channel, image, width * channel);

    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start);
    cout << endl << "Time cost: " << duration.count() << "s" << endl;
}

glm::vec3 Tracer::Tracing(const Ray &ray, int depth) {
    HitRecord hit;
    if (Scene::objects->Hit(ray, T_MIN, T_MAX, hit)) {
        auto emitted = hit.material->Emitted(ray, hit); //光源自发光
        ScatterRecord scatter;
        if (depth < depth_limit && hit.material->Scatter(ray, hit, scatter)) {
            if (scatter.is_sample) {  //蒙特卡罗采样
                auto pdf = Scene::lights->Empty() ? scatter.pdf :  //没有光源则仅从材质采样
                        std::make_shared<PDFMixture>(scatter.pdf, std::make_shared<PDFHittable>(Scene::lights, hit.position));  //有光源则混合采样

                auto direction = pdf->Sample();
                auto prob = pdf->Value(direction);
                Ray scattered = Ray(hit.position, direction);
                return emitted + scatter.attenuation * hit.material->ScatterPDF(ray, hit, scattered) * Tracing(scattered, depth + 1) / prob;
            }
            else {  //不采样
                return emitted + scatter.attenuation * Tracing(scatter.ray, depth + 1);
            }
        }
        else return emitted;
    }
    else return Scene::background;
}

void Tracer::DrawPixel(int row, int col, glm::vec4 color) const {
    if (row < 0 || row >= height || col < 0 || col >= width) return;
    int index = (row * width + col) * channel;
    image[index + 0] = (unsigned char )(255.0f * color.x);
    image[index + 1] = (unsigned char )(255.0f * color.y);
    image[index + 2] = (unsigned char )(255.0f * color.z);
    image[index + 3] = (unsigned char )(255.0f * color.w);
}


