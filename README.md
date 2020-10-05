# Versus

### Libraries

- [SDL](https://www.libsdl.org/) for Windows, Mac, and Linux platform abstraction

- [stb_image](https://github.com/nothings/stb) (single header) for image loading

- [yaml-cpp](https://github.com/jbeder/yaml-cpp) for [YAML](https://yaml.org/) handling

### Enviroment Variables

__TODO: Use CMake or something similar__
 
The Visual Studio solution expects the following enviroment variables

- `SDL2_INCLUDE` - SDL2 include folder path

- `SDL2_LIB32` - SDL2 (32 bit) library folder path (`SDL2main.lib`, `SDL2.lib`, `SDL2.dll`)

- `SDL2_LIB64` - Same as above but for the 64 bit libraries

-`YAMLCPP_INCLUDE` - yaml-cpp include folder path

- `YAMLCPP_LIB32` - yaml-cpp library folder path (`yaml-cpp.lib`, `yaml-cppd.lib`) 

- `YAMLCPP_LIB64` - Same as above but for the 64 bit libraries
