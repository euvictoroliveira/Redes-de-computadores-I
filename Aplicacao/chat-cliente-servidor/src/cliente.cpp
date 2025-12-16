#include <iostream>
#include <thread>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::cin;
using std::endl;
using std::string;

void recebe_mensagens(SOCKET sock){
    char buffer[1024];

    while(true){
        memset(buffer, 0, sizeof(buffer)); // limpa o buffer

        int bytes = recv(sock, buffer, sizeof(buffer), 0); // recebe os dados do servidor 

        if(bytes <= 0)
            break;

        cout << buffer;

    }

}

// Função main
int main(){
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    // Estrutura de servidor
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;          
    server_addr.sin_port = htons(8080);        
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); 

    // Conecta ao servidor
    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

    // Solicita o nome do usuário
    string nome_usuario;
    cout << "Usuario: ";
    getline(cin, nome_usuario);

    // Envia comando LOGIN conforme o protocolo da aplicação
    string login = "LOGIN " + nome_usuario + "\n";
    send(sock, login.c_str(), login.size(), 0);

    // Thread para receber mensagens enquanto o usuário digita
    std::thread(recebe_mensagens, sock).detach();

    // Loop para envio de mensagens
    string mensagem;
    while (true) {
        getline(cin, mensagem);
        
        // Comando local para sair
        if (mensagem == "exit")
            break;

        // Envia mensagem usando o protocolo da aplicação
        string envia_mensagem = "MSG " + mensagem + "\n";
        send(sock, envia_mensagem.c_str(), envia_mensagem.size(), 0);
    }

    
    string logout = "LOGOUT\n";
    send(sock, logout.c_str(), logout.size(), 0);

    // Fecha conexão TCP
    closesocket(sock);
    WSACleanup();


    return 0;
}
