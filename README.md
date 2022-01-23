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

Make sure you have [CMake](https://cmake.org/) installed and you have C++20 compatible compiler.

### Release
```
git clone https://github.com/Marcin648/m68k-emu
cd m68k-emu
git submodule update --init --recursive
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Debug
```
git clone https://github.com/Marcin648/m68k-emu
cd m68k-emu
git submodule update --init --recursive
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```
<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

1. Clone this repository inside libs directory in yours project
2. Add subdirectory and include directory to yours CMakeList.txt
   ```
   add_subdirectory(libs/m68k-emu)
   include_directories(libs/m68k-emu/include)
   ```
3. Link m68k-emu library
   ```
   target_link_libraries(myproject PRIVATE m68k-emu)
   ```

<p align="right">(<a href="#top">back to top</a>)</p>


## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>
