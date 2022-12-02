echo "building eigen..."
cd external/eigen
git checkout 3147391d946bb4b6c68edd901f2add6ac1f31f8c
mkdir build
cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DEIGEN_BUILD_TESTING=0 -DEIGEN_BUILD_DOC=0 -DCMAKE_BUILD_TYPE="Release"


echo "building pangolin..."
cd ../../pangolin
mkdir build
cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DMSVC_USE_STATIC_CRT=0 -DGLEW_INCLUDE_DIR="../../GLEW/include" -DGLEW_LIBRARY="../../GLEW/lib/glew32.lib" -DCMAKE_BUILD_TYPE="Release" -DBUILD_EXAMPLES=0 -DBUILD_TOOLS=0 -DBUILD_TESTS=0
cmake --build . -j8 --config Release


echo "building glog..."
cd ../../glog
git checkout 8f9ccfe770add9e4c64e9b25c102658e3c763b73
mkdir build
cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE="Release"
cmake --build . -j8 --config Release


echo "building ceres..."
cd ../../ceres-solver
git checkout f0851667bea45564fe1c2a5b8a4f27bcde112e85
mkdir build
cd build
export CL="$CL /DNOMINMAX=1 /D_USE_MATH_DEFINES=1"
cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE="Release" -DBUILD_EXAMPLES=0
cmake --build . -j8 --config Release
