# BMPFilter

BMPFilter, 24-bit sıkıştırılmamış BMP görüntülerine "Negatif" filtresi uygulayan ve bu işlemi donanım seviyesinde analiz eden bir benchmarking projesidir. 

Sistem, piksel matrisini işlerken aynı algoritmanın modern bir C derleyicisi  ile yazılmış hali ile x86-64 Assembly komut setleri arasındaki işlemci yürütme sürelerini kıyaslar. 
Ölçümler `CLOCK_MONOTONIC` kullanılarak nanosaniye hassasiyetinde yapılır.

## 🚀 Özellikler

* **Hibrit Mimari:** C tarafı dosya okuma/yazma ve bellek tahsisini üstlenirken; ağır işlem döngüleri  x86-64 NASM mimarisinde çalıştırılır.
* **Düşük Seviye Bellek Yönetimi:** C üzerinden tahsis edilen piksel matrisine, Assembly tarafında doğrudan bellek adresleri üzerinden müdahale edilir.
* **Performans Kıyaslama:** Aynı veri seti her iki dilde de işlenerek donanımdaki yürütme süresi farkı hesaplanır ve yüzdelik oranla terminale raporlanır.

## 🛠️ Kurulum ve Derleme (Linux)

Not:Projenin derlenebilmesi için sisteminizde GCC ve NASM kurulu olmalıdır.

```bash
sudo apt update
sudo apt install gcc nasm -y
```

1. **Depoyu Klonlayın:**
   ```bash
   git clone [https://github.com/seriftales/BMPFilter](https://github.com/seriftales/BMPFilter.git)
   cd BMPFilter
   ```

2. **Assembly Kodunu Nesne Dosyasına  Derleyin::**
   ```bash
    nasm -f elf64 filter.asm -o filter.o
   ```


3. **C Kodunu Assembly Nesnesi ile Bağlayarak Çalıştırılabilir Dosya Üretin:**
   ```bash
    gcc main.c filter.o -o bmpfilter
   ```

## ⚙️ Çalıştırma 

Uygulamayı başlatmak için terminalde derlenmiş dosyayı çalıştırın:

``` bash 
./bmpfilter test.bmp
```
Uygulama sizden işlenecek dosyanın adını isteyecektir (Örn: test.bmp).bmp test verileri bmps/ dizininde tutulmaktadır.

Çözümlenen veri matrisi sırasıyla C ve Assembly fonksiyonlarına gönderilerek hız testi yapılır.

İşlem bittiğinde sonuçlar terminale basılır ve filtrelenmiş yeni dosya output/ dizinine kaydedilir.


## 📁 Dizin Yapısı
```text
BMPFilter/
├── bmps/                # İşlenecek orijinal 24-bit BMP dosyaları 
├── output/              # Filtrelenmiş çıktı dosyalarının kaydedildiği dizin
├── main.c               # I/O, bellek yönetimi ve Benchmarking 
├── filter.asm           # x86-64 NASM piksel inverting algoritması
├── .gitignore          
├── README.md
```
