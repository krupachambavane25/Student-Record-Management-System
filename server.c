#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <windows.h>

#include "student.h"

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

    int contentLength = (int)strlen(content);

    sprintf(
        header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
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
        "Access-Control-Allow-Origin: *\r\n"
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
   SEND 405
   ===================================================== */

void send405(SOCKET client)
{
    const char *message =
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n"
        "\r\n"
        "405 - Method Not Allowed";

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

    /*
       IMPORTANT:
       Your frontend files are in the SAME directory
       as server.exe.

       Project structure:

       DSA SLA/
       ├── index.html
       ├── style.css
       ├── script.js
       ├── server.exe
       ├── server.c
       ├── student.c
       └── student.h
    */

    file = fopen(filename, "rb");

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
        size_t bytesRead;

        bytesRead =
            fread(
                content,
                1,
                (size_t)fileSize,
                file
            );

        if (bytesRead != (size_t)fileSize)
        {
            free(content);
            fclose(file);
            return NULL;
        }
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
        printf(
            "ERROR: Could not open file: %s\n",
            filename
        );

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
   SEND STUDENTS JSON
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

        written =
            sprintf(
                item,

                "%s"
                "{\"rollNo\":%d,"
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

            char *newJson =
                (char *)realloc(
                    json,
                    capacity
                );

            if (newJson == NULL)
            {
                free(json);

                sendResponse(
                    client,
                    "application/json",
                    "[]"
                );

                return;
            }

            json = newJson;
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
   HANDLE HTTP REQUEST
   ===================================================== */

void handleRequest(SOCKET client)
{
    char buffer[BUFFER_SIZE];

    int received;


    received =
        recv(
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


    /* =================================================
       OPTIONS
       ================================================= */

    if (
        strncmp(
            buffer,
            "OPTIONS",
            7
        ) == 0
    )
    {
        const char *response =
            "HTTP/1.1 204 No Content\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n"
            "Connection: close\r\n"
            "\r\n";

        send(
            client,
            response,
            (int)strlen(response),
            0
        );

        return;
    }


    /* =================================================
       MAIN WEBSITE
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET / HTTP/",
            11
        ) == 0
    )
    {
        printf(
            "Serving index.html...\n"
        );

        sendFile(
            client,
            "index.html",
            "text/html"
        );

        return;
    }


    /* =================================================
       CSS
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /style.css",
            14
        ) == 0
    )
    {
        printf(
            "Serving style.css...\n"
        );

        sendFile(
            client,
            "style.css",
            "text/css"
        );

        return;
    }


    /* =================================================
       JAVASCRIPT
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /script.js",
            14
        ) == 0
    )
    {
        printf(
            "Serving script.js...\n"
        );

        sendFile(
            client,
            "script.js",
            "application/javascript"
        );

        return;
    }


    /* =================================================
       FAVICON
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /favicon.ico",
            17
        ) == 0
    )
    {
        send404(client);

        return;
    }


    /* =================================================
       GET ALL STUDENTS
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /api/students",
            17
        ) == 0
    )
    {
        printf(
            "API: Getting all students...\n"
        );

        sendStudentsJSON(client);

        return;
    }


    /* =================================================
       GET STUDENT COUNT
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /api/count",
            14
        ) == 0
    )
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


    /* =================================================
       GET AVERAGE CGPA
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /api/average",
            16
        ) == 0
    )
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


    /* =================================================
       EXPORT CSV
       ================================================= */

    if (
        strncmp(
            buffer,
            "GET /api/export",
            15
        ) == 0
    )
    {
        printf(
            "API: Exporting student records...\n"
        );

        exportCSV();

        sendResponse(
            client,
            "application/json",
            "{\"success\":true}"
        );

        return;
    }


    /* =================================================
       UNKNOWN ROUTE
       ================================================= */

    printf(
        "404: Route not found\n"
    );

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

    printf(
        "================================================\n"
    );

    printf(
        "     STUDENT RECORD MANAGEMENT SYSTEM\n"
    );

    printf(
        "                C BACKEND SERVER\n"
    );

    printf(
        "================================================\n"
    );


    /* =================================================
       START WINSOCK
       ================================================= */

    result =
        WSAStartup(
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


    /* =================================================
       CREATE SOCKET
       ================================================= */

    serverSocket =
        socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );


    if (
        serverSocket ==
        INVALID_SOCKET
    )
    {
        printf(
            "Socket creation failed. Error: %d\n",
            WSAGetLastError()
        );

        WSACleanup();

        return 1;
    }


    /* =================================================
       SERVER ADDRESS
       ================================================= */

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


    /* =================================================
       BIND
       ================================================= */

    result =
        bind(
            serverSocket,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)
        );


    if (
        result ==
        SOCKET_ERROR
    )
    {
        printf(
            "Bind failed. Error: %d\n",
            WSAGetLastError()
        );

        closesocket(serverSocket);

        WSACleanup();

        return 1;
    }


    /* =================================================
       LISTEN
       ================================================= */

    result =
        listen(
            serverSocket,
            SOMAXCONN
        );


    if (
        result ==
        SOCKET_ERROR
    )
    {
        printf(
            "Listen failed. Error: %d\n",
            WSAGetLastError()
        );

        closesocket(serverSocket);

        WSACleanup();

        return 1;
    }


    /* =================================================
       LOAD STUDENTS
       ================================================= */

    loadStudents();


    printf("\n");

    printf(
        "================================================\n"
    );

    printf(
        "       SERVER STARTED SUCCESSFULLY!\n"
    );

    printf(
        "================================================\n"
    );


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


    /* =================================================
       ACCEPT CLIENTS
       ================================================= */

    while (1)
    {
        clientLength =
            sizeof(clientAddress);


        clientSocket =
            accept(
                serverSocket,
                (struct sockaddr *)&clientAddress,
                &clientLength
            );


        if (
            clientSocket ==
            INVALID_SOCKET
        )
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


    /* Normally unreachable */

    closesocket(
        serverSocket
    );


    WSACleanup();


    return 0;
}