Cavendish Mesher
================

This is a Cavendish mesher for educational purposes. The Cavendish method is detailled [here](http://github.com/VictorLamoine/cavendishMesher/blob/master/documentation/cavendish.md).

This mesher does not work with holes and some shapes might "crash" it in an infinite loop; if you want a real mesher, you better look at Qhull's Delaunay triangulation.

![HeartMeshAnimation](https://raw.githubusercontent.com/VictorLamoine/cavendishMesher/master/documentation/heart.gif)

How to build
============

Additionnaly to a C++ compiler you need cmake and OpenCV; on Ubuntu you can install them with:
```bash
sudo apt-get install cmake-curses-gui g++ libcv-dev libhighgui-dev libopencv-dev
```

Build the project with the following commands:
```bash
mkdir cavendishMesher && cd cavendishMesher
git clone https://github.com/VictorLamoine/cavendishMesher.git src
mkdir build && cd build
cmake ../src && make -j2
```

Run the program:
```bash
./mesh ../src/GeoFiles/heart.geo /dev/null
```

Documentation
=============

Documentation is built by default, you need to install doxygen:
```bash
sudo apt-get install doxygen
```

The documentation is generated in the build folder (`cavendishMesher_documentation/html/index.hml`).

If you don't want the documentation, use `cmake ../src -DBUILD_DOCUMENTATION=OFF`.

To do list
==========
- Finish the README
- Colours + starting point
- Keyboard actions
- Explain RdM6
