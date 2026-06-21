#include "pch.h"
#include "C_UDP_Client.h"

C_UDP_Client::C_UDP_Client()
{
    client_socket = INVALID_SOCKET;
    slen = 0;
}

C_UDP_Client::~C_UDP_Client()
{
    closesocket(client_socket);
    WSACleanup();
}

bool C_UDP_Client::isClientSocket()
{
    if (client_socket != 0)
		return true;
    return false;
}

bool C_UDP_Client::Begin_Winsock()
{
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        Error_Info.Format(_T("Failed. Error Code: %d"), WSAGetLastError());
        WifiStatus = Error_Info;
        return false;
    }
    // create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        Error_Info.Format(_T("socket() failed with error code: %d"), WSAGetLastError());
        WifiStatus = Error_Info;
        return false;
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    slen = sizeof(sockaddr_in);
    WifiStatus = _T("Build up socket");

    return true;
}

void C_UDP_Client::Send_Data(byte *message)
{
    int len = message[1];

    if (sendto(client_socket, (char*)message, len, 0, (sockaddr*)&server, slen) == SOCKET_ERROR)
    {
        Error_Info.Format(_T("sendto() failed with error code: %d"), WSAGetLastError());
    }
}

bool C_UDP_Client::Listen_to_Data()
{
    int Result;
    Result = recvfrom(client_socket, (char*)Get_Data, LENGTHANSWER, 0, (sockaddr*)&server, &slen);
     if (Result == SOCKET_ERROR)
     {
         int error = WSAGetLastError();
         Error_Info.Format(_T("recvfrom() failed with error code: %d"), error);
         return false;
     }
    return true;
}

byte C_UDP_Client::get_Kennung()
{
    return Get_Data[0];
}

void C_UDP_Client::get_Wifi_RSSI()
{
    byte send_Data[255] = { 0 };
    make_Message(0x0C, "", send_Data);
	Send_Data(send_Data);
}

void C_UDP_Client::connect_Server()
{
	byte send_Data[255] = { 0 };
	make_Message(0x0E, "Desktop", send_Data); 
	Send_Data(send_Data);
    
}

void C_UDP_Client::disconnect_Server()
{
    byte send_Data[255] = { 0 };
    make_Message(0x0F, "Desktop", send_Data);
    Send_Data(send_Data);
}

void C_UDP_Client::make_Message(byte Kennung, char* Text, byte* Data)
{
	size_t length = strlen(Text);
	Data[0] = Kennung; // Kennung
	Data[1] = static_cast<byte>(length + 2); // Length of the message including Kennung and length byte
    for (size_t i = 0; i < length; i++)
    {
		Data[i + 2] = static_cast<byte>(Text[i]); // Copy the text into the Data array
    }
}

void C_UDP_Client::make_Message(byte Kennung, byte Data1, byte* Data)
{
    Data[0] = Kennung; // Kennung
    Data[1] = static_cast<byte> (0x03); 
    Data[2] = static_cast<byte>(Data1);
}

void C_UDP_Client::make_Message(byte Kennung, byte Data1, byte Data2, byte* Data)
{
    Data[0] = Kennung; // Kennung
    Data[1] = static_cast<byte> (0x04);
    Data[2] = static_cast<byte>(Data1);
    Data[3] = static_cast<byte>(Data2);
}

void C_UDP_Client::make_Message_LZV_FE(byte Kennung, byte Data1, byte Data2, byte* Data)
{
    Data[0] = Kennung; // Kennung
    Data[1] = static_cast<byte> (0x07);
    Data[2] = static_cast<byte> (0xFF);
    Data[3] = static_cast<byte> (0xFE);
    Data[4] = static_cast<byte>(Data1);
    Data[5] = static_cast<byte>(Data2);
    Data[6] = static_cast<byte>(0x00);
}

CString C_UDP_Client::Get_Data_String() const
{
    CString strData;
    for (int i = 2; i < Get_Data[1]; ++i)
    {
        strData.AppendChar(static_cast<TCHAR>(Get_Data[i]));
    }
    return strData;
}


void C_UDP_Client::Trace_Data() const
{
    CString Trace;
    
    for (int i = 0; i < Get_Data[1]; ++i)
    {
        CString byteStr;
        byteStr.Format(_T("%02X "), Get_Data[i]);
        Trace.Append(byteStr);
    }
    TRACE1(" in Data: %s \n ",Trace ); // Replace TRACE0 with TRACE                                                                                                                                                                                                                                                                                                                                                                       
}

byte C_UDP_Client::Get_Data_Byte(int index) const
{
    return Get_Data[index];
}
