#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Assembly fonksiyon bildirimi
extern void filter(unsigned char* data, int size);

int main() {
    char fileName[256];
    char inputPath[512] = "/home/kaan/BMPFilter/bmps/"; // Varsayılan çalışma dizini
    char outputPath[512];

    printf("=== BMP Filter App ===\n");
    printf("File Name : ");
    
    // Kullanıcıdan dosya adını alma
    if (scanf("%255s", fileName) != 1) {
        printf("Gecersiz girdi!\n");
        return 1;
    }

    // Tam yolu olusturma
    strcat(inputPath, fileName);
    
    // Cikis dosya adini belirleme (cikti_ + orijinal_isim)
    sprintf(outputPath, "/home/kaan/BMPFilter/output/output_%s", fileName);

    // 1. Dosyayı Açma
    FILE *input = fopen(inputPath, "rb");
    if (input == NULL) {
        printf("\nHata: Dosya acilamadi!\n");
        printf("Sistem Mesaji: %s\n", strerror(errno));
        printf("Denenen Yol: %s\n", inputPath);
        return 1;
    }
    printf("\n[OK] Dosya bulundu ve acildi.\n");

    // 2. Header Okuma (54 Bayt)
    unsigned char header[54];
    if (fread(header, 1, 54, input) != 54) {
        printf("Hata: BMP Basligi (Header) okunamadi!\n");
        fclose(input);
        return 1;
    }

    // 3. Boyut Bilgilerini Çekme
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    width = abs(width);
    height = abs(height);

    int size = width * height * 3; 
    printf("[BILGI] Boyut: %d x %d | Toplam Veri: %d bayt\n", width, height, size);

    // 4. Bellek Ayırma
    unsigned char *pixels = (unsigned char*)malloc(size);
    if (pixels == NULL) {
        printf("Hata: RAM uzerinde yer ayrilamadi!\n");
        fclose(input);
        return 1;
    }

    // 5. Veri Okuma
    fread(pixels, 1, size, input);
    fclose(input);

    // 6. ASSEMBLY FILTRELEME
    printf("[ISLEM] Assembly filtresi uygulaniyor...\n");
    filter(pixels, size);

    // 7. Kaydetme
    FILE *output = fopen(outputPath, "wb");
    if (output == NULL) {
        printf("Hata: Cikis dosyasi olusturulamadi!\n");
        free(pixels);
        return 1;
    }

    fwrite(header, 1, 54, output);
    fwrite(pixels, 1, size, output);
    
    fclose(output);
    free(pixels);

    printf("[BAŞARILI] Islem bitti.\n");

    return 0;
}