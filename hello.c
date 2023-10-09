#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char c;

    printf("Content-Type: application/json\r\n\r\n");

    file = fopen("1.json", "r");

    if (file) {
        while ((c = getc(file)) != EOF) {
            putchar(c);
        }
        fclose(file);
    } else {
        printf("<html>\n");
        printf("<head>\n");
        printf("<title>Error</title>\n");
        printf("</head>\n");
        printf("<body>\n");
        printf("<h1>Error opening file</h1>\n");
        printf("</body>\n");
        printf("</html>\n");
    }

    return 0;
}