#include <iostream>
#include <stdexcept>
#include <algorithm>

class DynamicArray {
private:
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

    ~DynamicArray() {
        delete[] elements;
    }

    void display() const {
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

    void setElement(size_t position, int value) {
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
};

class ExtendedDynamicArray : public DynamicArray {
public:
    ExtendedDynamicArray(size_t initialSize = 0) : DynamicArray(initialSize) {}

    ExtendedDynamicArray(const DynamicArray& source) : DynamicArray(source) {}

    ExtendedDynamicArray(const ExtendedDynamicArray& source) : DynamicArray(source) {}

    double average() const {
        if (length() == 0) {
            throw std::runtime_error("Cannot calculate average of empty array");
        }

        double sum = 0;
        for (size_t i = 0; i < length(); ++i) {
            sum += getElement(i);
        }
        return sum / length();
    }
    double median() const {
        size_t size = length();
        if (size == 0) {
            throw std::runtime_error("Cannot calculate median of empty array");
        }

        int* temp = new int[size];
        for (size_t i = 0; i < size; ++i) {
            temp[i] = getElement(i);
        }

        std::sort(temp, temp + size);

        double result;
        if (size % 2 == 0) {
            result = (temp[size/2 - 1] + temp[size/2]) / 2.0;
        } else {
            result = temp[size/2];
        }

        delete[] temp;
        return result;
    }

    int minElement() const {
        if (length() == 0) {
            throw std::runtime_error("Cannot find minimum in empty array");
        }

        int minVal = getElement(0);
        for (size_t i = 1; i < length(); ++i) {
            int current = getElement(i);
            if (current < minVal) {
                minVal = current;
            }
        }
        return minVal;
    }

    int maxElement() const {
        if (length() == 0) {
            throw std::runtime_error("Cannot find maximum in empty array");
        }

        int maxVal = getElement(0);
        for (size_t i = 1; i < length(); ++i) {
            int current = getElement(i);
            if (current > maxVal) {
                maxVal = current;
            }
        }
        return maxVal;
    }

    void displayStats() const {
        if (length() == 0) {
            std::cout << "Array is empty, no statistics available." << std::endl;
            return;
        }

        display();
        std::cout << "Statistics:" << std::endl;
        std::cout << "  Minimum: " << minElement() << std::endl;
        std::cout << "  Maximum: " << maxElement() << std::endl;
        std::cout << "  Average: " << average() << std::endl;
        std::cout << "  Median: " << median() << std::endl;
    }
};

int main() {
    try {
        std::cout << "=== Testing ExtendedDynamicArray ===" << std::endl;

        ExtendedDynamicArray arr(5);
        arr.setElement(0, 10);
        arr.setElement(1, -5);
        arr.setElement(2, 25);
        arr.setElement(3, 15);
        arr.setElement(4, 0);

        std::cout << "Original array: ";
        arr.display();
        std::cout << "\nStatistical Analysis:" << std::endl;
        std::cout << "Minimum element: " << arr.minElement() << std::endl;
        std::cout << "Maximum element: " << arr.maxElement() << std::endl;
        std::cout << "Average value: " << arr.average() << std::endl;
        std::cout << "Median value: " << arr.median() << std::endl;

        std::cout << "\n=== Testing with even number of elements ===" << std::endl;
        ExtendedDynamicArray arr2(4);
        arr2.setElement(0, 1);
        arr2.setElement(1, 3);
        arr2.setElement(2, 2);
        arr2.setElement(3, 4);

        arr2.displayStats();

        std::cout << "\n=== Testing with single element ===" << std::endl;
        ExtendedDynamicArray arr3(1);
        arr3.setElement(0, 42);
        arr3.displayStats();

        std::cout << "\n=== Testing empty array exception handling ===" << std::endl;
        ExtendedDynamicArray emptyArr;
        try {
            emptyArr.minElement();
        } catch (const std::exception& e) {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
        }

        std::cout << "\n=== Testing inheritance functionality ===" << std::endl;
        emptyArr.append(10);
        emptyArr.append(20);
        emptyArr.append(30);
        emptyArr.displayStats();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}