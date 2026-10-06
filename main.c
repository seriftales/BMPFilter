#define _POSIX_C_SOURCE 199309L 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

// Assembly fonksiyon bildirimi
extern void filter(unsigned char* data, int size);

// C tabanlı filtre fonksiyonu (Karşılaştırma için)
void filter_c(unsigned char* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i] = 255 - data[i];
    }
}

// Ana program
int main(int argc, char *argv[]) {
    
    char inputPath[512] ;
    char outputPath[512];
    struct timespec start, end;
    double time_c, time_asm;

    printf("==========================================\n");
    printf("        BMP FILTRELEME & BENCHMARKING       \n");
    printf("==========================================\n");
    
    if (argc != 2) {
        printf("HATA: Eksik parametre girdiniz!\n");
        printf("Doğru Kullanım: %s <dosya_adi.bmp>\n", argv[0]);
        printf("Not: İşlenecek dosya 'bmps/' klasöründe olmalıdır.\n");
        return 1;
    }

    // Yol birleştirme
    snprintf(inputPath, sizeof(inputPath), "bmps/%s", argv[1]);
    snprintf(outputPath, sizeof(outputPath), "output/output_%s", argv[1]);

    // 1. DOSYA ACMA
    FILE *input = fopen(inputPath, "rb");
    if (!input) {
        perror("\nHata: Kaynak dosya açılamadı");
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
    printf("[1/2] C Filtresi Çalıstırılıyor...");
    fflush(stdout);
    clock_gettime(CLOCK_MONOTONIC, &start);
    filter_c(pixels, size);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_c = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf(" TAMAMLANDI\n");

    // Veriyi eski haline getir (Assembly temiz veriyle baslasın)
    filter_c(pixels, size);

    // --- HIZ TESTI: ASSEMBLY VERSIYONU ---
    printf("[2/2] Assembly Filtresi Çalıstırılıyor...");
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
        printf(">> SONUÇ: Assembly, C'den %.2f%% daha hızlı!\n", fark);
    } else {
        printf(">> SONUÇ: C derleyicisi çok iyi optimize etmiş!\n");
    }
    printf("-----------------------------------------\n");

    // 7. DOSYAYI KAYDET
    FILE *output = fopen(outputPath, "wb");
    if (output) {
        fwrite(header, 1, 54, output);
        fwrite(pixels, 1, size, output);
        fclose(output);
        printf("[OK] Yeni dosya oluşturuldu: %s\n", outputPath);
    }

    free(pixels);
    return 0;
}