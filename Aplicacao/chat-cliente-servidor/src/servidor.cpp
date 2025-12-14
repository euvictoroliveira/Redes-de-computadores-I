#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib"); // inclue a biblioteca winsock automaticamente

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// Cria vetor global de cilentes
vector <SOCKET> clientes;

#define MAX 1024 // macro para tamanho do buffer

// Implementa função para conversa de comunicação entre diferentes clientes
void handle_client(SOCKET client_socket){
    char buffer[MAX]; // cria buffer para recepção das mensagens
    string nome_usario; 

    while(true){
        memset(buffer, 0, sizeof(buffer)); // limpa o buffer
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0); // variável relacionada ao número de bytes recebidos

        if(bytes <= 0)
            break; // encerra função se o número de bytes for menor ou igual a 0(significa que não foram recebidos dados)

        string mensagem(buffer); // converte os bytes recebidos em uma string de mensagem
        
        if(mensagem.rfind("LOGIN", 0) == 0){
            nome_usario = mensagem.substr(0); // captura nome de usuário

            cout << "Novo usario conectado: " << nome_usario << endl; // mensagem de conexão bem-sucedida
        }else if(mensagem.rfind("MSG", 0) == 0){
            string texto = mensagem.substr(0); // recupera mensagem do usuário

            string formatado = nome_usario + ":" + texto; // formata mensagem 

            // Etapa d envio
            for(SOCKET c : clientes){
                send(c, formatado.c_str(), formatado.size(), 0);
            }
        }else if(mensagem.rfind("LOGOUT", 0 ) == 0){
            cout << "Usuario " << nome_usario << " saiu" << endl;
        }  
    }

    // Fecha a conexção tcp com o cliente
    closesocket(client_socket);
    clientes.erase(remove(clientes.begin(), clientes.end(), client_socket), clientes.end()); // remove cliente da lista de conectados.

}

// Função main
int main(){
    WSADATA wsa;

    WSAStartup(MAKEWORD(2,2), &wsa); // inicializa biblioteca
    SOCKET socket_servidor = socket(AF_INET, SOCK_STREAM, 0); // cria socket para o servidor.

    // Estrura do endereço
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // porta 8080
    server_addr.sin_addr.s_addr = INADDR_ANY; // deve aceitar qualquer IP

    bind(socket_servidor, (sockaddr*)&server_addr, sizeof(server_addr)); // faz associação do socket ao IP e porta adequados
    listen(socket_servidor, 5);

    cout << "Servidor rodando na porta 8080" << endl;

    while(true){
        SOCKET socket_cliente = accept(socket_servidor, nullptr, nullptr); // cria novo socket de cliente

        clientes.push_back(socket_cliente);

        std::thread(handle_client, socket_cliente).detach();
    }
    
    closesocket(socket_servidor); // fecha o socket do servidor
    WSACleanup(); 

    return 0;
}