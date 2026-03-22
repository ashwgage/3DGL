<!--
*** This readme was adapted from Best-README-Template.
  https://github.com/othneildrew/Best-README-Template
-->

<div align="left">

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]

</div>

<a href="https://github.com/ashwgage/3DGL">
  <img alt="C++ Logo" src="assets/img/cpp.svg" align="right" width="150">
</a>

<div align="left">
  <h1><em><a href="https://github.com/ashwgage/3DGL">~3DGL</a></em></h1>
</div>

<!-- ABOUT THE PROJECT -->

A three-dimensional header-only graphics library written in C++13 and accelerated with CUDA/Apple Metal.

### Built With

[![C++][C++-shield]][C++-url]
[![CUDA][CUDA-shield]][CUDA-url]
[![Apple][Apple-shield]][Apple-url]
[![WebAssembly][WebAssembly-shield]][webassembly-url]
[![CMake][CMake-shield]][CMake-url]
[![Codecov][Codecov-shield]][Codecov-url]
[![GitHub Actions][github-actions-shield]][github-actions-url]

## Preview

<p align="center">
  <img src="assets/img/demo.mp4"
  width = "80%"
  alt = "Video demonstration"
  />
</p>

## Getting Started

### Prerequisites

Before attempting to build this project, make sure you have [Nix](https://nixos.org/download.html) with [Flake](https://nixos.wiki/wiki/Flakes) support installed on your machine.

Additionally, if you wish to utilize the GPU acceleration features, you will need to have [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) or [Apple Metal](https://developer.apple.com/metal/cpp/) installed on your machine.

### Installation

1. Clone the project repository

   ```sh
   git clone https://github.com/ashwgage/3DGL.git
   cd 3DGL
   ```

2. Install the project dependencies

   ```sh
   nix-shell --max-jobs $(nproc) # Linux / Windows (WSL)
   nix-shell --max-jobs $(sysctl -n hw.ncpu) # macOS
   ```

3. Build the project

   ```sh
   just build
   ```

4. Run the project

   ```sh
   just run <package_name>
   ```

## Usage

3DGL is designed to be easy to use. You can include the library in your C++ project by adding the following line to your source files:

```cpp
#include <3dgl/3dgl.h>
```

### Benchmarks

3DGL is capable of rendering 3D scenes with thousands of triangles at interactive frame rates.

| Benchmark | Description | Result |
| --------- | ----------- | ------ |
| `cube` | Render a cube with 12 triangles | 60 FPS |
| `sphere` | Render a sphere with 960 triangles | 60 FPS |
| `bunny` | Render a Stanford Bunny with 69451 triangles | 60 FPS |
| `dragon` | Render a Stanford Dragon with 871306 triangles | 60 FPS |

### Conventions

3DGL uses the following conventions:

- left-handed coordinate system  
- counter-clockwise winding order for triangle vertices  

### Keyboard Controls

3DGL uses the following keyboard shortcuts:

| Command Keybind | Command Description |
| --------------- | ------------------- |
| <kbd>CTRL</kbd> + <kbd>q</kbd> | Quit the application |

## Project Structure

```sh
3DGL/
├── .github/
├── include/
│   └── 3DGL/
├── src/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## License

The source code for [ashwgage/3DGL](https://github.com/ashwgage/3DGL) is distributed under the terms of the GNU General Public License v3.0. See [`LICENSE`](./LICENSE) for details.

<!-- MARKDOWN LINKS -->

[contributors-shield]: https://img.shields.io/github/contributors/ashwgage/3DGL.svg?style=for-the-badge
[contributors-url]: https://github.com/ashwgage/3DGL/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/ashwgage/3DGL.svg?style=for-the-badge
[forks-url]: https://github.com/ashwgage/3DGL/network/members
[stars-shield]: https://img.shields.io/github/stars/ashwgage/3DGL.svg?style=for-the-badge
[stars-url]: https://github.com/ashwgage/3DGL/stargazers

<!-- BADGES -->

[C++-shield]: https://img.shields.io/badge/C++-%23008080.svg?style=for-the-badge&logo=c%2B%2B&logoColor=004482&labelColor=222222&color=004482
[C++-url]: https://isocpp.org/
[CUDA-shield]: https://img.shields.io/badge/cuda-%23008080.svg?style=for-the-badge&logo=nVIDIA&logoColor=76B900&labelColor=222222&color=76B900
[CUDA-url]: https://developer.nvidia.com/cuda-zone
[Apple-shield]: https://img.shields.io/badge/metal-%23008080.svg?style=for-the-badge&logo=apple&logoColor=white&labelColor=222222&color=white
[Apple-url]: https://developer.apple.com/metal/
[WebAssembly-shield]: https://img.shields.io/badge/WebAssembly-20232A?style=for-the-badge&logo=WebAssembly&logoColor=654FF0&labelColor=222222&color=654FF0
[webassembly-url]: https://webassembly.org/
[CMake-shield]: https://img.shields.io/badge/CMake-%23008080.svg?style=for-the-badge&logo=cmake&logoColor=008FBA&labelColor=222222&color=008FBA
[CMake-url]: https://cmake.org/
[Codecov-shield]: https://img.shields.io/badge/codecov-%23008080.svg?style=for-the-badge&logo=codecov&logoColor=FF0077&labelColor=222222&color=FF0077
[Codecov-url]: https://codecov.io/
[github-actions-shield]: https://img.shields.io/badge/github%20actions-%232671E5.svg?style=for-the-badge&logo=githubactions&logoColor=2671E5&labelColor=222222&color=2671E5
[github-actions-url]: https://github.com/features/actions
