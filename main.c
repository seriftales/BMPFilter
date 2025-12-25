#define _POSIX_C_SOURCE 199309L 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

// Assembly fonksiyon bildirimi
extern void filter(unsigned char* data, int size);

// C tabanlı filtre fonksiyonu (Karsılastırma için)
void filter_c(unsigned char* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i] = 255 - data[i];
    }
}

int main() {
    char fileName[256];
    char inputPath[512] = "/home/kaan/BMPFilter/bmps/"; 
    char outputPath[512];
    struct timespec start, end;
    double time_c, time_asm;

    printf("=========================================\n");
    printf("        BMP FILTRELEME & BENCHMARK       \n");
    printf("=========================================\n");
    
    printf("Dosya adini giriniz: ");
    if (scanf("%255s", fileName) != 1) return 1;

    // Yol birleştirme
    strcat(inputPath, fileName);
    sprintf(outputPath, "/home/kaan/BMPFilter/output/output_%s", fileName);

    // 1. DOSYA ACMA
    FILE *input = fopen(inputPath, "rb");
    if (!input) {
        perror("\nHata: Kaynak dosya acilamadi");
        return 1;
    }

    // 2. HEADER OKUMA
    unsigned char header[54];
    fread(header, 1, 54, input);

    // 3. BOYUT HESAPLAMA
    int width = abs(*(int*)&header[18]);
    int height = abs(*(int*)&header[22]);
    int size = width * height * 3;

    // 4. BELLEK AYIRMA
    unsigned char *pixels = (unsigned char*)malloc(size);
    if (!pixels) {
        printf("Hata: Bellek yetersiz!\n");
        fclose(input);
        return 1;
    }

    // 5. PIKSEL VERISINI OKUMA
    fread(pixels, 1, size, input);
    fclose(input);

    printf("\n[BILGI] Resim: %dx%d | Veri: %d bayt\n", width, height, size);
    printf("-----------------------------------------\n");

    // --- HIZ TESTI: C VERSIYONU ---
    printf("[1/2] C Filtresi calistiriliyor...");
    fflush(stdout);
    clock_gettime(CLOCK_MONOTONIC, &start);
    filter_c(pixels, size);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_c = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf(" TAMAMLANDI\n");

    // Veriyi eski haline getir (Assembly temiz veriyle baslasın)
    filter_c(pixels, size);

    // --- HIZ TESTI: ASSEMBLY VERSIYONU ---
    printf("[2/2] Assembly Filtresi calistiriliyor...");
    fflush(stdout);
    clock_gettime(CLOCK_MONOTONIC, &start);
    filter(pixels, size);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_asm = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf(" TAMAMLANDI\n");

    // 6. SONUCLARI RAPORLA
    printf("-----------------------------------------\n");
    printf("C Süresi        : %.9f sn\n", time_c);
    printf("Assembly Süresi : %.9f sn\n", time_asm);
    
    if (time_asm < time_c) {
        double fark = ((time_c - time_asm) / time_c) * 100;
        printf(">> SONUC: Assembly, C'den %.2f%% daha hizli!\n", fark);
    } else {
        printf(">> SONUC: C derleyicisi cok iyi optimize etmis!\n");
    }
    printf("-----------------------------------------\n");

    // 7. DOSYAYI KAYDET
    FILE *output = fopen(outputPath, "wb");
    if (output) {
        fwrite(header, 1, 54, output);
        fwrite(pixels, 1, size, output);
        fclose(output);
        printf("[OK] Yeni dosya olusturuldu: %s\n", outputPath);
    }

    free(pixels);
    return 0;
}