#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL2
#

#CXX = g++  
CXX = clang++

EXE = inspector 
IMGUI_DIR = . 
SOURCES = $(wildcard *.cpp)
#SOURCES += $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_impl_sdl2.cpp $(IMGUI_DIR)/imgui_impl_sdlrenderer.cpp
#SOURCES += $(IMGUI_DIR)/implot.cpp $(IMGUI_DIR)/implot_items.cpp 
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS = -stdlib=libc++ -std=c++20 -I$(IMGUI_DIR)  
CXXFLAGS +=    -fexperimental-library -g -Wall -Wformat
LIBS = -lSDL2

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)
	./inspector

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
