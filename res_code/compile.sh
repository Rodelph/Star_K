echo "Compiling the file !"
g++ main.cpp -lglfw ./c_file/glad.o -ldl -lsfml-audio -o gameDev
echo "Executing the file !"
./gameDev