#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

int main(int argc, char **argv){
    setlocale(LC_ALL, "");
    FILE* src = fopen(argv[1], "r");
    FILE* dst = fopen(argv[2], "w+"); // use w+ to truncate the file to 0
    fwide(src, 1);
    fwide(dst, 1);
    int buffersize = atoi(argv[3]);
    if(buffersize == -1){
        // linebuffered
        setvbuf(src, NULL, _IOLBF, 0);
        setvbuf(dst, NULL, _IOLBF, 0);
    }
    else if(buffersize == 0){
        // unbuffered
        setvbuf(src, NULL, _IONBF, 0);
        setvbuf(dst, NULL, _IONBF, 0);
    }
    else{
        // fullbuffered
        setvbuf(src, NULL, _IOFBF, buffersize * 1024);
        setvbuf(dst, NULL, _IOFBF, buffersize * 1024);
    }
    wint_t inputint;
    wchar_t inputchar;
    wchar_t buffer[82] = {0};
    int buflen = 0;
    int linepos = 0;
    // start copy
    while(1){
        inputint = fgetwc(src);
        if(inputint == WEOF) break;
        inputchar = (wchar_t)inputint;
        // wprintf(L"%lc", inputchar);
        buffer[buflen++] = inputchar;
        if(inputchar == '\n'){
            // if string too long, then get newline
            // Then, output the data in buffer
            if(linepos + buflen > 80) fwprintf(dst, L"\n");
            fwprintf(dst, L"%ls", buffer);
            linepos = buflen;
            buflen = 0;
            // reset the buffer
            memset(buffer, '\0', sizeof(buffer));
        }
        else if(inputchar == ' '){
            // Because ' ' probably not the end
            // So when the string too long, just get newline
            // then it don't need to output directly
            // If not too long, just append it after the original output
            if(linepos + buflen > 80){
                fwprintf(dst, L"\n");
                linepos = 0;
            }
            else{
                fwprintf(dst, L"%ls", buffer);
                linepos += buflen;
                buflen = 0;
                memset(buffer, '\0', sizeof(buffer));
            }
        }
    }
    // Deal with the data still in the buffer
    if(buflen > 0){
        // wprintf(L"-%ls-\n", buffer);
        if(linepos + buflen > 80) fwprintf(dst, L"\n%ls", buffer);
        else fwprintf(dst, L"%ls", buffer);
    }
    // close the stream
    fclose(src);
    fclose(dst);
}