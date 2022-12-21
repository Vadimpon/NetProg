#include <iostream>
#include <string>
#include <cstdlib> 
#include <cstring> 
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode ) // Исключения (возможные ошибки завершение программы)
{
    cout << "Error:"<<exitCode <<endl<< why << endl;
    exit(exitCode);
}
int main()
{
    // структура с адресом программы клиента
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; // интернет протокол IPv4
    selfAddr->sin_port = 0;         // любой порт 
    selfAddr->sin_addr.s_addr = 0;  
    // структура с адресом сервера
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET; // интернет протокол IPv4
    remoteAddr->sin_port = htons(7);  // port 7
    remoteAddr->sin_addr.s_addr = inet_addr("95.152.62.42"); //  адрес 
    // буфер для передачи и приема данных
    char *bufer = new char[4096];
    strcpy(bufer,"Hello, Penza!");   //копируем строку
    int msgLen = strlen(bufer);      //вычисляем длину строки
    // создаём сокет
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); //tcp протокол
    if (mySocket == -1) {
        close(mySocket);
        Exception("Error open socket",1);
    }
    //связываем сокет с адрессом
    int rc = bind(mySocket,(const sockaddr *) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Error bind socket with local address",2);
        }
    //установливаем соединение
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Error connect socket with remote server.", 3);
    }
    // передаём сообщение из буффера
    rc = send(mySocket, bufer, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Error send message", 4);
    }
    cout << "We send: " << bufer << endl; 
    // принимаем ответ в буффер
    rc = recv(mySocket, bufer, 1024, 0);
    if (rc == -1) {
        close(mySocket);
       Exception("Error receive answer.", 5);
    }
    bufer[rc] = '\0'; // конец принятой строки
    cout << "We receive: " << bufer << endl; // вывод полученного сообщения от сервера
    // закрыем сокет
    close(mySocket);
    delete selfAddr;
    delete remoteAddr;
    delete[] bufer;
    return 0;
}
