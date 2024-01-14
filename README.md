# crystallography-model

Template repository used for many C-based projects. Sets up some boilerplate code for C headers, `CMake` and `Doxygen`.

![](https://img.shields.io/badge/Sinho_softworks-FFBF00?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-000000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/mrsinho/crystallography-model)

[TOC]

## Todo

* Film as canvas
* Mesh input vertices (atoms positions)
    * smd loader
    * mesh orientation (optional)
* Film shader inputs
    * lambda
    * distance of film from origin
    * film orientation (optional)

* 3d view (iper optional)
* film rotation around the globe animation (iper optional)

## Overview

This template has been used on many libraries (see [`shengine`](https://mrsinho.github.io/shengine-docs), [`shgui`](https://mrsinho.github.io/shgui-docs))

![](.shci/linux/linux-exit-code.svg)
![](.shci/windows/windows-exit-code.svg)

![](https://img.shields.io/badge/Written_in_C-FFBF00?style=for-the-badge&logo=c&logoColor=white&labelColor=FFA000#.svg)
![](https://img.shields.io/badge/Compatible_with_C%2b%2b-FFBF00?style=for-the-badge&logo=c%2b%2b&logoColor=white&labelColor=FFA000#.svg)


## Generate CMakeLists.txt (skip)

```shell
cd externals/shengine
python export-application.py name=crystal-model path=../../crystal-model
```

## Build

Building the template code is fairly simple:

```shell
cd    crystal-model
mkdir build
cd    build
mkdir windows
cd    windows
cmake ../../ -DCRYSTAL_MODEL_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="D:/home/desktop/github/crystallography-model/externals/shengine"
```


## Contributions and support

Should you encounter any issues with the tools or wish to introduce new features, I welcome your contributions to the repository. You can contribute by either opening an issue or submitting a pull request.


[![](https://img.shields.io/badge/Buy_Me_A_Coffee-FFDD00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://www.buymeacoffee.com/mrsinho)
![](https://img.shields.io/badge/Sinho_softworks-FFBF00?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
