echo "building eigen..."
cd external/eigen
md build
cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DEIGEN_BUILD_TESTING=0 -DEIGEN_BUILD_DOC=0 -DCMAKE_BUILD_TYPE="Debug"
cmake --build . --target=install --config "Debug"


echo "building pangolin..."
cd ../../pangolin
md build
cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DMSVC_USE_STATIC_CRT=0 -DGLEW_INCLUDE_DIR="../../GLEW/include" -DGLEW_LIBRARY="../../GLEW/lib/glew32.lib" -DCMAKE_BUILD_TYPE="Debug" -DBUILD_EXAMPLES=0 -DBUILD_TOOLS=0 -DBUILD_TESTS=0
cmake --build . -j8 --config Debug
cmake --build . --target=install
