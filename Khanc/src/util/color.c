#include <stdio.h>
#include "util/color.h"

void printColored(const char *color, const char *style, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Stil ve renk ile başla
    printf("%s%s", style, color);

    // Formatlı mesajı yazdır
    vprintf(format, args);

    // Renk sıfırlama kodunu yazdır
    printf("%s", RESET);

    va_end(args);
}
