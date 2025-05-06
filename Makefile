TARGET = railway_simulation


SRCDIR = src


OBJDIR = obj


CXX = g++


CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g


SRCS = $(wildcard $(SRCDIR)/*.cpp)


OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))


$(TARGET): $(OBJECTS) | $(OBJDIR)
	@echo "Линковка: $@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)


$(OBJDIR):
	@echo "Создание каталога: $@"
	mkdir -p $(OBJDIR)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Компиляция: $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@echo "Очистка..."
	$(RM) -f $(OBJECTS) $(TARGET)
	$(RM) -rf $(OBJDIR)


.PHONY: all
all: $(TARGET)


.PHONY: $(TARGET)

.PHONY: all clean $(OBJDIR)
