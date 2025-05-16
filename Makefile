CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -Iinclude -g

SRC_DIR = src
OBJDIR = build

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = server

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
