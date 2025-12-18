# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -std=c++17

# Biblioteca de rede do Windows
LIBS = -lws2_32

# Arquivos
SERVIDOR = servidor.cpp
CLIENTE = cliente.cpp

# Executáveis
SERVIDOR_EXE = servidor.exe
CLIENTE_EXE = cliente.exe

# Regra padrão
all: servidor cliente

# Compila o servidor
server:
	$(CXX) $(CXXFLAGS) $(SERVIDOR) -o $(SERVIDOR_EXE) $(LIBS)

# Compila o cliente
client:
	$(CXX) $(CXXFLAGS) $(CLIENTE) -o $(CLIENTE_EXE) $(LIBS)

# Limpa os executáveis
clean:
	del *.exe
