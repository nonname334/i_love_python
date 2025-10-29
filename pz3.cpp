#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <filesystem>
#include <cmath>

namespace fs = std::filesystem;

class ImageNoiseRemover {
private:
    std::vector<std::vector<int>> pixels;
    int width, height, maxVal;

public:
    bool readPGM(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Cannot open: " << filename << std::endl;
            return false;
        }

        std::string magic;
        file >> magic;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] != '#') break;
        }

        std::istringstream iss(line);
        iss >> width >> height;

        file >> maxVal;

        std::cout << "Reading " << width << "x" << height << " max=" << maxVal << std::endl;

        pixels.resize(height, std::vector<int>(width));

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (!(file >> pixels[i][j])) {
                    std::cout << "Error reading pixels" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool savePGM(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cout << "Cannot create: " << filename << std::endl;
            return false;
        }

        file << "P2\n";
        file << width << " " << height << "\n";
        file << maxVal << "\n";

        // Save pixels with proper formatting
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                file << pixels[i][j];
                if (j < width - 1) file << " ";
            }
            file << "\n";
        }

        std::cout << "Saved: " << filename << std::endl;
        return true;
    }

    void addSaltPepperNoise(float noiseLevel) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        int salt = maxVal;
        int pepper = 0;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float rnd = dis(gen);
                if (rnd < noiseLevel / 2) {
                    pixels[i][j] = pepper;
                } else if (rnd < noiseLevel) {
                    pixels[i][j] = salt;
                }
            }
        }
    }

    void medianFilter(int kernelSize = 3) {
        std::vector<std::vector<int>> result = pixels;
        int offset = kernelSize / 2;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::vector<int> window;

                for (int ki = -offset; ki <= offset; ++ki) {
                    for (int kj = -offset; kj <= offset; ++kj) {
                        int ni = i + ki;
                        int nj = j + kj;
                        if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                            window.push_back(pixels[ni][nj]);
                        }
                    }
                }

                std::sort(window.begin(), window.end());
                result[i][j] = window[window.size() / 2];
            }
        }
        pixels = result;
    }

    void meanFilter(int kernelSize = 3) {
        std::vector<std::vector<int>> result = pixels;
        int offset = kernelSize / 2;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int sum = 0;
                int count = 0;

                for (int ki = -offset; ki <= offset; ++ki) {
                    for (int kj = -offset; kj <= offset; ++kj) {
                        int ni = i + ki;
                        int nj = j + kj;
                        if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                            sum += pixels[ni][nj];
                            count++;
                        }
                    }
                }

                result[i][j] = sum / count;
            }
        }
        pixels = result;
    }

    double compareWithOriginal(const ImageNoiseRemover& original) {
        double mse = 0;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int diff = pixels[i][j] - original.pixels[i][j];
                mse += diff * diff;
            }
        }
        mse /= (width * height);
        if (mse == 0) return 100;
        return 20 * log10(maxVal / sqrt(mse));
    }
};

void processAllImages(const std::string& inputDir, const std::string& outputDir) {
    std::ofstream csv("results.csv");
    csv << "Image,NoiseLevel,Filter,PSNR\n";

    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".pgm") {
            std::string filename = entry.path().stem().string();
            std::cout << "Processing: " << filename << std::endl;

            ImageNoiseRemover original;
            if (!original.readPGM(entry.path().string())) continue;

            std::vector<float> noiseLevels = {0.05, 0.1, 0.2};

            for (float noise : noiseLevels) {
                std::cout << "Noise: " << noise * 100 << "%" << std::endl;

                ImageNoiseRemover noisy = original;
                noisy.addSaltPepperNoise(noise);

                ImageNoiseRemover median = noisy;
                median.medianFilter(3);
                csv << filename << "," << noise << ",Median," << median.compareWithOriginal(original) << "\n";
                median.savePGM(outputDir + "/" + filename + "_noise" + std::to_string(int(noise*100)) + "_median.pgm");

                ImageNoiseRemover mean = noisy;
                mean.meanFilter(3);
                csv << filename << "," << noise << ",Mean," << mean.compareWithOriginal(original) << "\n";
                mean.savePGM(outputDir + "/" + filename + "_noise" + std::to_string(int(noise*100)) + "_mean.pgm");
            }
        }
    }
}

int main() {
    std::string inputDir = "images";
    std::string outputDir = "results";

    fs::create_directory(inputDir);
    fs::create_directory(outputDir);

    std::cout << "Starting processing" << std::endl;
    processAllImages(inputDir, outputDir);
    std::cout << "Done! Saved in the results.csv" << std::endl;

    return 0;
}