# Simple draughts.

# Our project uses cmake 3.0+ for generating makefiles.

# Libraries
We use glfw3 and glm as git submodules
We also use GLAD Loader-Generator generated specifically for our project.

# Downloading
Clone our repository using command:
	 `git clone --recurse-submodules https://github.com/adrset/warcaby`

# Compilation
Firstly configure the project using:
	`cmake .`

If the proccess didn't encouter any errors build the project:
	`make`

The binary is located in build directory.
	
# Graphics drivers
In order to run this program you need graphics drivers that support GLSL 1.3+ (130 and higher) and OpenGL context 3.3.
