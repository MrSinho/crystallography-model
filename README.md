# crystallography-model

@note _This is a pre-release version, first release is expected to be available by the end of **August 2024**._

A 3d simulator of crystallography interference and diffraction images using the Laue's equation. It's written in C and it uses the [`Vulkan API`](https://www.vulkan.org), [`shengine`](https://mrsinho.github.io/shengine-docs) and [`shvulkan`](https://github.com/mrsinho/shvulkan).

![](https://img.shields.io/badge/Sinho_softworks-006400?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-000000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/mrsinho/crystallography-model)

[TOC]

## Overview

![](.shci/linux/linux-exit-code.svg)
![](.shci/windows/windows-exit-code.svg)

![](https://img.shields.io/badge/Written_in_C-006400?style=for-the-badge&logo=c&logoColor=white&labelColor=003200#.svg)
![](https://img.shields.io/badge/Compatible_with_C%2b%2b-006400?style=for-the-badge&logo=c%2b%2b&logoColor=white&labelColor=003200#.svg)


## (Skip) CMakeLists.txt generation

@warning _Skip this step if you are not the developer._

```bash
cd externals/shengine
python export-application.py name=crystal-model path=../../crystal-model
```

If you exectued the command above by mistake just type `git stash` on the terminal.

## Generate project files and build

Building the program is fairly simple:

<div class="tabbed">

- <b class="tab-title">Windows</b>
    ```bash
    mkdir crystal-model/build
    mkdir crystal-model/build/windows
    cd    crystal-model/build/windows
    cmake ../../ -DCRYSTAL_MODEL_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="repo/absolute/path/externals/shengine"
    cmake --build .
    ```

- <b class="tab-title">Linux</b>
    ```bash
    mkdir crystal-model/build
    mkdir crystal-model/build/linux
    cd    crystal-model/build/linux
    cmake ../../ -DCRYSTAL_MODEL_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="repo/absolute/path/externals/shengine"
    cmake --build .
    ```

</div>


## Run crystal model executable 

<div class="tabbed">

- <b class="tab-title">Windows</b>
    ```bash
    ./crystal-model/bin/windows/crystal-model
    ```

- <b class="tab-title">Linux</b>
    ```bash
    ./crystal-model/bin/linux/crystal-model
    ```

</div>




## Usage of configuration files

At `crystal-model/smd/saved.smd` you can edit the main parameters used in crystallography, suchh as the incident ray wavelength and orientation in space, the lattice unit structure and the photosensitive matrix or film distance from the crystal.

```bash
FLOAT32    ^^ 1    !! lambda_ag       --> 5 ***
FLOAT32    ^^ 4    !! incident_ray    --> 1, 0, 0 ***

UINT32     ^^ 1    !! atom_count         --> 4 ***
FLOAT32    ^^ 16   !! atoms_positions_ag --> 0 0 0 1 0 10 0 1 0 10 0 1 10 0 0 1 ***

FLOAT32    ^^ 1    !! film_distance_cm --> 1 ***
```

To load the written or saved data from `crystal-model/smd/saved.smd` press `LEFT_CTRL` + `L`. To recovery the default information press `LEFT_CTRL` + `R`. To export the current information data to `crystal-model/smd/saved.smd`, press `LEFT_CTRL` + `E`.


## Commands

<div class="full_width_table">

|Parameter        |Increase keys    |Decrease keys    |
|-----------------|-----------------|-----------------|
| `lambda_ag`     |       _W_       |       _Q_       |
| `incident_ray`  |     _S F H_     |     _A D G_     |
| `film_distance` |       _X_       |       _Z_       |

</div>

![](docs/media/render3.png)

## To do (pre-release):

* Generate real 3d cones with shaders (need to rewrite shaders)
* Add projections of the cones on the film
    * currently the film is only a canvas of angles
    * missing orientation and position of the lattice and of the film
* 3d view
* abstract 3d library from the application
* shci tests
* From a source image find the lattice
* Compute interference coordinates and angles before fragment shader. Then check pixel coordinates with a tolerance threshold.
* From soure image to 3d model
* Make separate views of lattice and of interference rendering
* shci tests

![](docs/media/d=2lambda.png)

## Contributions and support

Should you encounter any issues with the tools or wish to introduce new features, I welcome your contributions to the repository. You can contribute by either opening an issue or submitting a pull request.

[![](https://img.shields.io/badge/Buy_Me_A_Coffee-006400?style=for-the-badge&logo=buy-me-a-coffee&logoColor=white)](https://www.buymeacoffee.com/mrsinho)
![](https://img.shields.io/badge/Sinho_softworks-006400?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
