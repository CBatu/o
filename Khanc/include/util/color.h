#ifndef COLOR_H
#define COLOR_H

#include <stdarg.h> 

// Renk ve stil kodları
#define RESET   "\033[0m"        // Reset
#define BOLD    "\033[1m"        // Kalın
#define UNDERLINE "\033[4m"      // Altı çizili

// Renk kodları
#define RED     "\033[31m"       // Kırmızı
#define GREEN   "\033[32m"       // Yeşil
#define YELLOW  "\033[33m"       // Sarı
#define BLUE    "\033[34m"       // Mavi
#define MAGENTA "\033[35m"       // Magenta
#define CYAN    "\033[36m"       // Cyan
#define WHITE   "\033[37m"       // Beyaz

// Fonksiyonlar
void printColored(const char *color, const char *style, const char *format, ...);

#endif // COLOR_H
