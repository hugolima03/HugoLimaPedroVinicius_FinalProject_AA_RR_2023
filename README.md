# Como executar
g++ -std=c++11 main.cpp -o ./builds/main && ./builds/main ./tests/debug.html

Este segundo arquivo a seguir faz o encode de um arquivo todo, diferente do primeiro que gera a tabela a partir do html e recebe pequenas entradas.
g++ -std=c++11 main2EncodeFile.cpp -o ./builds/main2 && ./builds/main2 ./tests/debug.html