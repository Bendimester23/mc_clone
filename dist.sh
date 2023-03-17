#build for linux
echo "Building for Linux 🐧"
cd ./cmake-build-release/
make
cd ../
rm -r ./dist/linux
mkdir ./dist/linux
cp ./cmake-build-release/mc_clone_2 ./dist/linux/
cp -r ./assets ./dist/linux/

# build for w*ndows
echo "Building for W*ndows 🪟"
cd ./cmake-build-windows/
make
cd ..
rm -r ./dist/windows
mkdir ./dist/windows
cp ./cmake-build-windows/mc_clone_2.exe ./dist/windows/
cp -r ./assets ./dist/linux/

echo "Compressing..."
zip -r not_a_windows_virus.zip dist

echo "Done ✅"
