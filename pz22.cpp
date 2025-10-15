#include <iostream>
#include <stdexcept>

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

int main() {
    try {
        std::cout << "=== Task 1: Basic Operations ===" << std::endl;
        DynamicArray first(3);
        first.setElement(0, 10);
        first.setElement(1, -50);
        first.setElement(2, 100);
        first.display();

        std::cout << "\nElement at position 1: " << first.getElement(1) << std::endl;

        std::cout << "\n=== Task 2: Copy Operations ===" << std::endl;
        DynamicArray second = first;
        second.display();

        std::cout << "\n=== Task 3: Append Elements ===" << std::endl;
        first.append(25);
        first.append(-75);
        first.display();

        std::cout << "\n=== Task 4: Array Mathematics ===" << std::endl;
        DynamicArray alpha(3);
        alpha.setElement(0, 10);
        alpha.setElement(1, 20);
        alpha.setElement(2, 30);

        DynamicArray beta(2);
        beta.setElement(0, 5);
        beta.setElement(1, 15);

        std::cout << "Array Alpha: ";
        alpha.display();
        std::cout << "Array Beta: ";
        beta.display();

        DynamicArray sum = alpha.combine(beta);
        std::cout << "Alpha + Beta: ";
        sum.display();

        DynamicArray diff = alpha.difference(beta);
        std::cout << "Alpha - Beta: ";
        diff.display();

        std::cout << "\n=== Size Mismatch Test ===" << std::endl;
        DynamicArray shortArray(1);
        shortArray.setElement(0, 50);

        DynamicArray longArray(4);
        longArray.setElement(0, 10);
        longArray.setElement(1, 20);
        longArray.setElement(2, 30);
        longArray.setElement(3, 40);

        std::cout << "Short Array: ";
        shortArray.display();
        std::cout << "Long Array: ";
        longArray.display();

        DynamicArray combined = shortArray.combine(longArray);
        std::cout << "Combined: ";
        combined.display();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}