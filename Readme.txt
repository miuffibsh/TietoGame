1. Navigate into Build directory.
2. Run the cmake command followed by the path to the source directory (the directory containing the
   CMakeLists.txt file) and specify the generator (e.g. Visual Studio 2019). For example:
      cmake .. -G "Visual Studio 16 2019" -A x64
   where the -A x64 is an option specifing the target Linux platform (64-bit).
3. Once the CMake configuration is complete, you can build the project by running the following command:
      cmake --build .
4. After the build process, the generated executables will be located in the Build directory.
5. Paste the files mapa.txt, rozkazy.txt and status.txt from the root directory to the directory containing
   Mediator executable, PlayerOne executable and Tietogra executable.

All of the libraries used in the project (<iostream>, <fstream>, <string>, <vector>, <cstdlib>, <ctime>,
<chrono>, <thread>, <sstream>, <tuple>, <algorithm>, <queue>, <climits>, <limits>, <random>, <utility>)
are part of the standard C++ library.