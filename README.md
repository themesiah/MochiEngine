# SDLSpaceShooter

cmake -B build
cmake --build build --config Debug --target SpaceShooter
cmake --build build --config Release --target SpaceShooter

cd build
ctest -C Debug