#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <windows.h>

#include "student.h"


#pragma comment(lib, "ws2_32.lib")


#define PORT 8080
#define BUFFER_SIZE 16384


/* =====================================================
   SEND HTTP RESPONSE
   ===================================================== */

void sendResponse(
    SOCKET client,
    const char *contentType,
    const char *content
)
{
    char header[1000];

    int contentLength;

    contentLength = (int)strlen(content);


    sprintf(
        header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Connection: close\r\n"
        "\r\n",
        contentType,
        contentLength
    );


    send(
        client,
        header,
        (int)strlen(header),
        0
    );


    send(
        client,
        content,
        contentLength,
        0
    );
}


/* =====================================================
   SEND 404
   ===================================================== */

void send404(SOCKET client)
{
    const char *message =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "\r\n"
        "404 - Page Not Found";


    send(
        client,
        message,
        (int)strlen(message),
        0
    );
}


/* =====================================================
   READ FILE
   ===================================================== */

char *readFile(const char *filename)
{
    FILE *file;

    long fileSize;

    char *content;


    /* Try the requested path first. If the EXE is started from the project folder, use frontend/... as a fallback. */
    file = fopen(
        filename,
        "rb"
    );

    if (file == NULL)
    {
        if (strcmp(filename, "../frontend/index.html") == 0)
            file = fopen("frontend/index.html", "rb");
        else if (strcmp(filename, "../frontend/style.css") == 0)
            file = fopen("frontend/style.css", "rb");
        else if (strcmp(filename, "../frontend/script.js") == 0)
            file = fopen("frontend/script.js", "rb");
    }

    if (file == NULL)
    {
        return NULL;
    }


    fseek(
        file,
        0,
        SEEK_END
    );


    fileSize = ftell(file);


    rewind(file);


    if (fileSize < 0)
    {
        fclose(file);
        return NULL;
    }


    content =
        (char *)malloc(
            (size_t)fileSize + 1
        );


    if (content == NULL)
    {
        fclose(file);
        return NULL;
    }


    if (fileSize > 0)
    {
        fread(
            content,
            1,
            (size_t)fileSize,
            file
        );
    }


    content[fileSize] = '\0';


    fclose(file);


    return content;
}


/* =====================================================
   SEND FILE
   ===================================================== */

void sendFile(
    SOCKET client,
    const char *filename,
    const char *contentType
)
{
    char *content;


    content = readFile(filename);


    if (content == NULL)
    {
        send404(client);
        return;
    }


    sendResponse(
        client,
        contentType,
        content
    );


    free(content);
}


/* =====================================================
   CREATE STUDENT JSON
   ===================================================== */

void sendStudentsJSON(SOCKET client)
{
    Student *temp;

    char *json;

    size_t capacity = 65536;

    size_t used = 0;

    int written;


    json =
        (char *)malloc(capacity);


    if (json == NULL)
    {
        sendResponse(
            client,
            "application/json",
            "[]"
        );

        return;
    }


    json[0] = '[';

    used = 1;


    temp = head;


    while (temp != NULL)
    {
        char item[1000];


        written = sprintf(
            item,
            "%s{\"rollNo\":%d,"
            "\"name\":\"%s\","
            "\"course\":\"%s\","
            "\"email\":\"%s\","
            "\"cgpa\":%.2f}",
            (used > 1 ? "," : ""),
            temp->rollNo,
            temp->name,
            temp->course,
            temp->email,
            temp->cgpa
        );


        if (used + written + 2 >= capacity)
        {
            capacity *= 2;

            json =
                (char *)realloc(
                    json,
                    capacity
                );


            if (json == NULL)
            {
                sendResponse(
                    client,
                    "application/json",
                    "[]"
                );

                return;
            }
        }


        memcpy(
            json + used,
            item,
            written
        );


        used += written;


        temp = temp->next;
    }


    json[used] = ']';

    used++;


    json[used] = '\0';


    sendResponse(
        client,
        "application/json",
        json
    );


    free(json);
}


/* =====================================================
   HANDLE REQUEST
   ===================================================== */

void handleRequest(SOCKET client)
{
    char buffer[BUFFER_SIZE];

    int received;


    received = recv(
        client,
        buffer,
        BUFFER_SIZE - 1,
        0
    );


    if (received <= 0)
    {
        return;
    }


    buffer[received] = '\0';


    printf(
        "\n--------------------------------------\n"
    );

    printf(
        "%s\n",
        buffer
    );

    printf(
        "--------------------------------------\n"
    );


    /* ==========================================
       OPTIONS
       ========================================== */

    if (strncmp(
        buffer,
        "OPTIONS",
        7
    ) == 0)
    {
        sendResponse(
            client,
            "text/plain",
            ""
        );

        return;
    }


    /* ==========================================
       MAIN WEBSITE
       ========================================== */

    if (strncmp(
        buffer,
        "GET / ",
        6
    ) == 0)
    {
        sendFile(
            client,
            "../frontend/index.html",
            "text/html"
        );

        return;
    }


    /* ==========================================
       CSS
       ========================================== */

    if (strncmp(
        buffer,
        "GET /style.css",
        14
    ) == 0)
    {
        sendFile(
            client,
            "../frontend/style.css",
            "text/css"
        );

        return;
    }


    /* ==========================================
       JAVASCRIPT
       ========================================== */

    if (strncmp(
        buffer,
        "GET /script.js",
        14
    ) == 0)
    {
        sendFile(
            client,
            "../frontend/script.js",
            "application/javascript"
        );

        return;
    }


    /* ==========================================
       GET ALL STUDENTS
       ========================================== */

    if (strncmp(
        buffer,
        "GET /api/students",
        17
    ) == 0)
    {
        sendStudentsJSON(client);

        return;
    }


    /* ==========================================
       GET STUDENT COUNT
       ========================================== */

    if (strncmp(
        buffer,
        "GET /api/count",
        14
    ) == 0)
    {
        char result[100];


        sprintf(
            result,
            "{\"count\":%d}",
            getStudentCount()
        );


        sendResponse(
            client,
            "application/json",
            result
        );


        return;
    }


    /* ==========================================
       GET AVERAGE CGPA
       ========================================== */

    if (strncmp(
        buffer,
        "GET /api/average",
        16
    ) == 0)
    {
        char result[100];


        sprintf(
            result,
            "{\"average\":%.2f}",
            getAverageCGPA()
        );


        sendResponse(
            client,
            "application/json",
            result
        );


        return;
    }


    /* ==========================================
       EXPORT CSV
       ========================================== */

    if (strncmp(
        buffer,
        "GET /api/export",
        15
    ) == 0)
    {
        exportCSV();


        sendResponse(
            client,
            "application/json",
            "{\"success\":true}"
        );


        return;
    }


    /* ==========================================
       NOT FOUND
       ========================================== */

    send404(client);
}


/* =====================================================
   MAIN
   ===================================================== */

int main(void)
{
    WSADATA wsaData;

    SOCKET serverSocket;

    SOCKET clientSocket;

    struct sockaddr_in serverAddress;

    struct sockaddr_in clientAddress;

    int clientLength;

    int result;


    printf("\n");
    printf("================================================\n");
    printf("     STUDENT RECORD MANAGEMENT SYSTEM\n");
    printf("                C BACKEND SERVER\n");
    printf("================================================\n");


    /* ==========================================
       START WINSOCK
       ========================================== */

    result = WSAStartup(
        MAKEWORD(2, 2),
        &wsaData
    );


    if (result != 0)
    {
        printf(
            "WSAStartup failed. Error: %d\n",
            result
        );

        return 1;
    }


    /* ==========================================
       CREATE SOCKET
       ========================================== */

    serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );


    if (serverSocket == INVALID_SOCKET)
    {
        printf(
            "Socket creation failed. Error: %d\n",
            WSAGetLastError()
        );

        WSACleanup();

        return 1;
    }


    /* ==========================================
       SERVER ADDRESS
       ========================================== */

    memset(
        &serverAddress,
        0,
        sizeof(serverAddress)
    );


    serverAddress.sin_family =
        AF_INET;


    serverAddress.sin_addr.s_addr =
        htonl(INADDR_LOOPBACK);


    serverAddress.sin_port =
        htons(PORT);


    /* ==========================================
       BIND
       ========================================== */

    result = bind(
        serverSocket,
        (struct sockaddr *)&serverAddress,
        sizeof(serverAddress)
    );


    if (result == SOCKET_ERROR)
    {
        printf(
            "Bind failed. Error: %d\n",
            WSAGetLastError()
        );

        closesocket(serverSocket);

        WSACleanup();

        return 1;
    }


    /* ==========================================
       LISTEN
       ========================================== */

    result = listen(
        serverSocket,
        SOMAXCONN
    );


    if (result == SOCKET_ERROR)
    {
        printf(
            "Listen failed. Error: %d\n",
            WSAGetLastError()
        );

        closesocket(serverSocket);

        WSACleanup();

        return 1;
    }


    /* ==========================================
       LOAD SAVED STUDENTS
       ========================================== */

    loadStudents();


    printf("\n");
    printf("================================================\n");
    printf("       SERVER STARTED SUCCESSFULLY!\n");
    printf("================================================\n");

    printf(
        "Students loaded: %d\n",
        getStudentCount()
    );

    printf(
        "Average CGPA   : %.2f\n",
        getAverageCGPA()
    );

    printf("\n");

    printf(
        "Website URL:\n"
    );

    printf(
        "http://localhost:8080\n"
    );

    printf("\n");

    printf(
        "Keep this terminal open.\n"
    );

    printf(
        "Press CTRL+C to stop the server.\n"
    );

    printf("\n");


    /* ==========================================
       ACCEPT CLIENTS
       ========================================== */

    while (1)
    {
        clientLength =
            sizeof(clientAddress);


        clientSocket = accept(
            serverSocket,
            (struct sockaddr *)&clientAddress,
            &clientLength
        );


        if (clientSocket == INVALID_SOCKET)
        {
            printf(
                "Accept failed. Error: %d\n",
                WSAGetLastError()
            );

            continue;
        }


        handleRequest(
            clientSocket
        );


        shutdown(
            clientSocket,
            SD_BOTH
        );


        closesocket(
            clientSocket
        );
    }


    /* This code normally isn't reached */

    closesocket(
        serverSocket
    );


    WSACleanup();


    return 0;
}