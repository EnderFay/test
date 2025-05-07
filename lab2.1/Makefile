CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = rzhd

# Список исходных файлов
SRCS = $(wildcard $(SRCDIR)/*.cpp) main.cpp

# Генерируем список объектных файлов в build/
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter $(SRCDIR)/%,$(SRCS))) \
       $(patsubst %.cpp,$(BUILDDIR)/%.o,$(filter-out $(SRCDIR)/%,$(SRCS)))

all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Правило для компиляции src/*.cpp → build/*.o
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для компиляции main.cpp → build/main.o
$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean