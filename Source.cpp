#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <fstream>

struct IPHeader {
    BYTE  bVersion;   
    BYTE  bHeaderLength;  
    BYTE  bTypeOfService; 
    USHORT usTotalLength; 
    USHORT usIdentification;
    USHORT usFlagsAndOffset; 
    BYTE  bTTL;           
    BYTE  bProtocol;      
    USHORT usChecksum;   
    ULONG ulSourceIP;    
    ULONG ulDestIP;       
};

void ProcessPacket(char* pBuffer, int nDataSize)
{

    IPHeader* pIPHeader = (IPHeader*)pBuffer;

    std::ofstream outfile("output.txt", std::ios::app);

    std::cout << "Source IP: " << inet_ntoa(*(in_addr*)&pIPHeader->ulSourceIP) << std::endl;
    std::cout << "Destination IP: " << inet_ntoa(*(in_addr*)&pIPHeader->ulDestIP) << std::endl;
    std::cout << "Protocol: " << (int)pIPHeader->bProtocol << std::endl;
    std::cout << "Total Length: " << ntohs(pIPHeader->usTotalLength) << std::endl;
    std::cout << "Header Length: " << (int)pIPHeader->bHeaderLength * 4 << " bytes" << std::endl;
    std::cout << "Version: " << (int)pIPHeader->bVersion << std::endl;
    std::cout << "Type of Service: " << (int)pIPHeader->bTypeOfService << std::endl;
    std::cout << "Identification: " << ntohs(pIPHeader->usIdentification) << std::endl;
    std::cout << "Flags and Fragment Offset: " << ntohs(pIPHeader->usFlagsAndOffset) << std::endl;
    std::cout << "Time to Live: " << (int)pIPHeader->bTTL << std::endl;
    std::cout << "Checksum: " << ntohs(pIPHeader->usChecksum) << std::endl;
    std::cout << std::endl;

    outfile << "Source IP: " << inet_ntoa(*(in_addr*)&pIPHeader->ulSourceIP) << std::endl;
    outfile << "Destination IP: " << inet_ntoa(*(in_addr*)&pIPHeader->ulDestIP) << std::endl;
    outfile << "Protocol: " << (int)pIPHeader->bProtocol << std::endl;
    outfile << "Total Length: " << ntohs(pIPHeader->usTotalLength) << std::endl;
    outfile << "Header Length: " << (int)pIPHeader->bHeaderLength * 4 << " bytes" << std::endl;
    outfile << "Version: " << (int)pIPHeader->bVersion << std::endl;
    outfile << "Type of Service: " << (int)pIPHeader->bTypeOfService << std::endl;
    outfile << "Identification: " << ntohs(pIPHeader->usIdentification) << std::endl;
    outfile << "Flags and Fragment Offset: " << ntohs(pIPHeader->usFlagsAndOffset) << std::endl;
    outfile << "Time to Live: " << (int)pIPHeader->bTTL << std::endl;
    outfile << "Checksum: " << ntohs(pIPHeader->usChecksum) << std::endl;
    outfile << std::endl;
    outfile.close();
}

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port = htons(2000);
    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError
        () << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    SOCKET clientSocket;
    sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    char receiveBuffer[65536];

    while (true) {
        int nBytesReceived = recvfrom(listenSocket, receiveBuffer, sizeof(receiveBuffer), 0, (sockaddr*)&clientAddress, &clientAddressLength);
        if (nBytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        ProcessPacket(receiveBuffer, nBytesReceived);
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}