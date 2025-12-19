#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

class DynamicArray {
protected:
    int* elements;
    size_t count;

public:
    DynamicArray(size_t initialSize = 0) : count(initialSize) {
        if (count > 0) {
            elements = new int[count]();
        } else {
            elements = nullptr;
        }
    }

    DynamicArray(const DynamicArray& source) : count(source.count) {
        if (count > 0) {
            elements = new int[count];
            for (size_t i = 0; i < count; ++i) {
                elements[i] = source.elements[i];
            }
        } else {
            elements = nullptr;
        }
    }

    virtual ~DynamicArray() {
        delete[] elements;
    }

    virtual void display() const {
        std::cout << "Array [size: " << count << "]: ";
        if (count == 0) {
            std::cout << "empty";
        } else {
            for (size_t i = 0; i < count; ++i) {
                std::cout << elements[i];
                if (i < count - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    virtual void setElement(size_t position, int value) {
        if (position >= count) {
            throw std::out_of_range("Invalid position");
        }
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Value out of range");
        }
        elements[position] = value;
    }

    int getElement(size_t position) const {
        if (position >= count) {
            throw std::out_of_range("Invalid position");
        }
        return elements[position];
    }

    size_t length() const {
        return count;
    }

    void append(int value) {
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Value out of range");
        }

        int* newElements = new int[count + 1];

        for (size_t i = 0; i < count; ++i) {
            newElements[i] = elements[i];
        }

        newElements[count] = value;

        delete[] elements;
        elements = newElements;
        count++;
    }

    DynamicArray combine(const DynamicArray& other) const {
        size_t maxLength = (count > other.count) ? count : other.count;
        DynamicArray result(maxLength);

        for (size_t i = 0; i < maxLength; ++i) {
            int first = (i < count) ? elements[i] : 0;
            int second = (i < other.count) ? other.elements[i] : 0;
            int total = first + second;

            if (total < -100) total = -100;
            if (total > 100) total = 100;

            result.elements[i] = total;
        }

        return result;
    }

    DynamicArray difference(const DynamicArray& other) const {
        size_t maxLength = (count > other.count) ? count : other.count;
        DynamicArray result(maxLength);

        for (size_t i = 0; i < maxLength; ++i) {
            int first = (i < count) ? elements[i] : 0;
            int second = (i < other.count) ? other.elements[i] : 0;
            int diff = first - second;

            if (diff < -100) diff = -100;
            if (diff > 100) diff = 100;

            result.elements[i] = diff;
        }

        return result;
    }

    DynamicArray& operator=(const DynamicArray& source) {
        if (this != &source) {
            delete[] elements;
            count = source.count;
            if (count > 0) {
                elements = new int[count];
                for (size_t i = 0; i < count; ++i) {
                    elements[i] = source.elements[i];
                }
            } else {
                elements = nullptr;
            }
        }
        return *this;
    }

    virtual void saveToFile() const {
        throw std::runtime_error("Base class saveToFile not implemented");
    }

protected:
    std::string getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_struct;
        
        #ifdef _WIN32
            localtime_s(&tm_struct, &now_time);
        #else
            localtime_r(&now_time, &tm_struct);
        #endif
        
        std::ostringstream oss;
        oss << std::put_time(&tm_struct, "%Y%m%d_%H%M%S");
        return oss.str();
    }
};

class ArrTxt : public DynamicArray {
public:
    ArrTxt(size_t initialSize = 0) : DynamicArray(initialSize) {}
    
    ArrTxt(const DynamicArray& source) : DynamicArray(source) {}
    
    void saveToFile() const override {
        std::string filename = getCurrentDateTime() + ".txt";
        std::ofstream outFile(filename);
        
        if (!outFile.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        outFile << "Dynamic Array TXT Export\n";
        outFile << "Size: " << count << "\n";
        outFile << "Elements: ";
        
        if (count == 0) {
            outFile << "empty";
        } else {
            for (size_t i = 0; i < count; ++i) {
                outFile << elements[i];
                if (i < count - 1) outFile << ", ";
            }
        }
        
        outFile << "\n\nIndex\tValue\n";
        outFile << "-----\t-----\n";
        
        for (size_t i = 0; i < count; ++i) {
            outFile << i << "\t" << elements[i] << "\n";
        }
        
        outFile.close();
        std::cout << "Array saved to TXT file: " << filename << std::endl;
    }
    
    void display() const override {
        std::cout << "[TXT Array] Size: " << count << " -> ";
        DynamicArray::display();
    }
};

class ArrCSV : public DynamicArray {
public:
    ArrCSV(size_t initialSize = 0) : DynamicArray(initialSize) {}
    
    ArrCSV(const DynamicArray& source) : DynamicArray(source) {}
    
    void saveToFile() const override {
        std::string filename = getCurrentDateTime() + ".csv";
        std::ofstream outFile(filename);
        
        if (!outFile.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        outFile << "Index,Value\n";
        
        for (size_t i = 0; i < count; ++i) {
            outFile << i << "," << elements[i] << "\n";
        }
        
        outFile.close();
        std::cout << "Array saved to CSV file: " << filename << std::endl;
    }
    
    void display() const override {
        std::cout << "[CSV Array] Size: " << count << " -> ";
        if (count == 0) {
            std::cout << "empty";
        } else {
            std::cout << elements[0];
            for (size_t i = 1; i < count; ++i) {
                std::cout << ";" << elements[i];
            }
        }
        std::cout << std::endl;
    }
};

void saveArrayToFile(DynamicArray& array) {
    try {
        array.saveToFile();
    } catch (const std::exception& e) {
        std::cerr << "Error saving array: " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::cout << "=== Creating and Testing Dynamic Arrays ===\n" << std::endl;
        
        std::cout << "1. Creating base DynamicArray:" << std::endl;
        DynamicArray baseArray(3);
        baseArray.setElement(0, 10);
        baseArray.setElement(1, -50);
        baseArray.setElement(2, 100);
        baseArray.display();
        
        std::cout << "\n2. Creating ArrTxt array from base array:" << std::endl;
        ArrTxt txtArray(baseArray);
        txtArray.display();
        
        std::cout << "\n3. Creating and modifying ArrCSV array:" << std::endl;
        ArrCSV csvArray(4);
        csvArray.setElement(0, 5);
        csvArray.setElement(1, 25);
        csvArray.setElement(2, -75);
        csvArray.setElement(3, 90);
        csvArray.display();
        
        std::cout << "\n4. Appending elements to arrays:" << std::endl;
        txtArray.append(45);
        csvArray.append(-30);
        
        std::cout << "Updated TXT array: ";
        txtArray.display();
        std::cout << "Updated CSV array: ";
        csvArray.display();
        
        std::cout << "\n5. Testing polymorphism with base class pointer:" << std::endl;
        DynamicArray* arrays[3];
        
        arrays[0] = new DynamicArray(2);
        arrays[0]->setElement(0, 20);
        arrays[0]->setElement(1, 40);
        
        arrays[1] = new ArrTxt(3);
        arrays[1]->setElement(0, 15);
        arrays[1]->setElement(1, -25);
        arrays[1]->setElement(2, 35);
        
        arrays[2] = new ArrCSV(3);
        arrays[2]->setElement(0, 100);
        arrays[2]->setElement(1, -100);
        arrays[2]->setElement(2, 0);
        
        for (int i = 0; i < 3; i++) {
            arrays[i]->display();
        }
        
        std::cout << "\n6. Saving arrays to files (TXT and CSV):" << std::endl;
        for (int i = 0; i < 3; i++) {
            try {
                saveArrayToFile(*arrays[i]);
            } catch (const std::runtime_error& e) {
                std::cout << "Base array cannot be saved to file directly" << std::endl;
            }
        }
        
        std::cout << "\n7. Array mathematics with derived classes:" << std::endl;
        ArrTxt array1(3);
        array1.setElement(0, 10);
        array1.setElement(1, 20);
        array1.setElement(2, 30);
        
        ArrCSV array2(2);
        array2.setElement(0, 5);
        array2.setElement(1, 15);
        
        std::cout << "Array 1 (TXT): ";
        array1.display();
        std::cout << "Array 2 (CSV): ";
        array2.display();
        
        DynamicArray sum = array1.combine(array2);
        std::cout << "Sum (base array): ";
        sum.display();
        
        std::cout << "\n8. Cleaning up memory:" << std::endl;
        for (int i = 0; i < 3; i++) {
            delete arrays[i];
        }
        
        std::cout << "\n=== Testing file creation directly ===" << std::endl;
        ArrTxt testTxt(5);
        for (int i = 0; i < 5; i++) {
            testTxt.setElement(i, i * 10);
        }
        testTxt.saveToFile();
        
        ArrCSV testCsv(5);
        for (int i = 0; i < 5; i++) {
            testCsv.setElement(i, i * -5);
        }
        testCsv.saveToFile();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
