#include <stdio.h>

#define uint8 unsigned char
#define uint64 unsigned long long

uint8 readUint8() {
    uint8 buffer = 0;
    fread(&buffer, sizeof(uint8), 1, stdin);
    return buffer;
}

uint8 readUint64() {
    uint64 buffer = 0;
    fread(&buffer, sizeof(uint64), 1, stdin);
    return buffer;
}

void writeUint8(const uint8 value) {
    fwrite(&value, sizeof(uint8), 1, stdout);
}

void writeUint64(const uint64 value) {
    fwrite(&value, sizeof(uint64), 1, stdout);
}

uint8 eofStdin() {
    return feof(stdin);
}

enum Command {
    GET_AT = 0,
    SET_AT = 1,
    GET_SIZE = 2,
    STOP = 3
};

int main() {

    const uint8 RESPONSE_INVALID_COMMAND = 1;
    const uint8 RESPONSE_EOF_AFTER_COMMAND = 2;
    const uint8 RESPONSE_STOPPED = 3;

    const uint8 GET_AT_RESPONSE_TYPE_OK = 4;
    const uint8 GET_AT_RESPONSE_TYPE_ERROR = 5;
    const uint8 GET_AT_RESPONSE_ERROR_INVALID_POSITION = 1;
    const uint8 GET_AT_RESPONSE_ERROR_EOF_AFTER_POSITION = 2;

    const uint8 SET_AT_RESPONSE_TYPE_OK = 6;
    const uint8 SET_AT_RESPONSE_TYPE_ERROR = 7;
    const uint8 SET_AT_RESPONSE_ERROR_EOF_AFTER_POSITION = 1;
    const uint8 SET_AT_RESPONSE_ERROR_INVALID_POSITION = 2;
    const uint8 SET_AT_RESPONSE_ERROR_EOF_AFTER_VALUE = 3;

    const uint8 END_EOF = 0;
    const uint8 END_EOF_AFTER_COMMAND = 1;
    const uint8 END_STOP = 2;
    const uint8 END_EOF_AFTER_GET_AT_POSITION = 3;
    const uint8 END_EOF_AFTER_SET_AT_POSITION = 4;
    const uint8 END_EOF_AFTER_SET_AT_VALUE = 5;

    freopen(NULL, "rb", stdin);
    freopen(NULL, "wb", stdout);

#define STORAGE_SIZE 5000000000
    uint8 array[STORAGE_SIZE] = {};

    uint8 command = 0;
    uint64 position = 0;
    uint8 value = 0;

    while (1) {
//        printf("while\n");
        command = readUint8();

        if (eofStdin()) {
//            printf("eof after command\n");
            writeUint8(RESPONSE_EOF_AFTER_COMMAND);
            return END_EOF_AFTER_COMMAND;
        }

        switch (command) {
            case STOP:
//                printf("stop\n");
                writeUint8(RESPONSE_STOPPED);
                return END_STOP;
            case GET_SIZE:
//                printf("get size\n");
//                printf("actual size: %llu\n", STORAGE_SIZE);
                writeUint64(STORAGE_SIZE);
                continue;
            case GET_AT:
//                printf("get at\n");
                position = readUint64();
//                printf("position: %llu\n", position);
                if (eofStdin()) {
//                    printf("eof after position\n");
                    writeUint8(GET_AT_RESPONSE_TYPE_ERROR);
                    writeUint8(GET_AT_RESPONSE_ERROR_EOF_AFTER_POSITION);
                    return END_EOF_AFTER_GET_AT_POSITION;
                }
                if (position >= STORAGE_SIZE) {
                    writeUint8(GET_AT_RESPONSE_TYPE_ERROR);
                    writeUint8(GET_AT_RESPONSE_ERROR_INVALID_POSITION);
                    continue;
                }
//                printf("value to get: %hhu\n", array[position]);
                writeUint8(GET_AT_RESPONSE_TYPE_OK);
                writeUint8(array[position]);
                continue;
            case SET_AT:
//                printf("set at\n");
                position = readUint64();
//                printf("position: %llu\n", position);
                if (eofStdin()) {
//                    printf("eof after position\n");
                    writeUint8(SET_AT_RESPONSE_TYPE_ERROR);
                    writeUint8(SET_AT_RESPONSE_ERROR_EOF_AFTER_POSITION);
                    return END_EOF_AFTER_SET_AT_POSITION;
                }
                if (position >= STORAGE_SIZE) {
                    writeUint8(SET_AT_RESPONSE_TYPE_ERROR);
                    writeUint8(SET_AT_RESPONSE_ERROR_INVALID_POSITION);
                    continue;
                }
                value = readUint8();
                if (eofStdin()) {
//                    printf("eof after value\n");
                    writeUint8(SET_AT_RESPONSE_TYPE_ERROR);
                    writeUint8(SET_AT_RESPONSE_ERROR_EOF_AFTER_VALUE);
                    return END_EOF_AFTER_SET_AT_VALUE;
                }
//                printf("value: %hhu\n", value);
                array[position] = value;
                writeUint8(SET_AT_RESPONSE_TYPE_OK);
                continue;
            default:
//                printf("invalid command\n");
                writeUint8(RESPONSE_INVALID_COMMAND);
                continue;
        }
    }

//    printf("eof\n");

    return END_EOF;
}