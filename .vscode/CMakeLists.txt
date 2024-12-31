cmake_minimum_required(VERSION 3.10)
project(SDLGame)

# Enable Emscripten-specific settings
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# SDL2 configuration
set(SDL2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/emscripten/cache/sysroot/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")

# Add optional SDL extensions as needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_IMAGE=2")  # For SDL_image
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_TTF=2")    # For SDL_ttf
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_MIXER=2")  # For SDL_mixer

# WebAssembly specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_FUNCTIONS=['_main']")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file assets")

# Add your source files
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/game.cpp
    # Add your other source files here
)