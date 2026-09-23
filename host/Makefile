CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -MMD -MP $(shell pkg-config --cflags glfw3)
LDLIBS := $(shell pkg-config --libs glfw3) -lGL -lGLU -lpthread

TARGET := imu_monitor
SRCS := serial.cpp parser.cpp fusion.cpp vis.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
