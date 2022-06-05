cd external/reactphysics3d/ || exit
cmake -G "Visual Studio 15 2017" -A x64 -T v141
cd ../../
python install.py