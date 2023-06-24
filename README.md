# Como executar
Instale a biblioteca gráfica.  
Ubuntu:  
sudo apt install graphviz  
Fedora:  
sudo yum install graphviz  
Windows(EXE installer):  
https://gitlab.com/api/v4/projects/4207231/packages/generic/graphviz-releases/8.0.5/windows_10_cmake_Release_graphviz-install-8.0.5-win64.exe

Compile o código passando o arquivo alvo como argumento.

g++ -std=c++11 main.cpp -o ./builds/main && ./builds/main ./tests/debug.html

(BONUS)Este segundo arquivo a seguir faz o encode de um arquivo todo, diferente do primeiro que gera a tabela a partir do html e recebe pequenas entradas.

g++ -std=c++11 main2EncodeFile.cpp -o ./builds/main2 && ./builds/main2 ./tests/debug.html