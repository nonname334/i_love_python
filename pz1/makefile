TARGET = MyProject.exe
CXX = g++

$(TARGET): main.o func.o
	$(CXX) main.o func.o -o $(TARGET)

main.o: main.cpp 
	$(CXX) -c main.cpp -o main.o

func.o: func.cpp 
	$(CXX) -c func.cpp -o func.o

clean:
	del $(TARGET) *.o