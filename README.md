<div id="top"></div>

<br />
<div align="center">
<h3 align="center">m68k-emu</h3>
  <p align="center">
    Motorola 68000 CPU Emulator as C++ library 
  </p>
</div>

### Built With

* [CMake](https://cmake.org/)

<p align="right">(<a href="#top">back to top</a>)</p>


## How to build

### Prerequisites

Make sure you have [CMake](https://cmake.org/) 3.21+ installed and you have C++20 compatible compiler.

### Release
```
git clone https://github.com/Marcin648/m68k-emu
cd m68k-emu
cmake -B Release -DCMAKE_BUILD_TYPE=Release
cmake --build Release -j $(nproc)
```

### Debug
```
git clone https://github.com/Marcin648/m68k-emu
cd m68k-emu
cmake -B Debug -DCMAKE_BUILD_TYPE=Debug
cmake --build Debug -j $(nproc)
```

### Running tests
```
./test/m68k_tests
./test/m68k_tests "[instruction]"
./test/m68k_tests "[benchmark]"
```
<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

1. Add m68k-emu to your CMakeLists.txt with FetchContent
   ```
   include(FetchContent)
   FetchContent_Declare(
       m68k-emu
       GIT_REPOSITORY https://github.com/Marcin648/m68k-emu.git
       GIT_TAG        main
   )
   FetchContent_MakeAvailable(m68k-emu)
   ```
2. Link m68k-emu library
   ```
   target_link_libraries(myproject PRIVATE m68k-emu)
   ```

<p align="right">(<a href="#top">back to top</a>)</p>


## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>
