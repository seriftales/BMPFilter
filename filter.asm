section .text
    global filter

filter:
    ; RDI = pixels dizisinin adresi
    ; RSI = size (toplam bayt)

    test rsi, rsi           ; Boyut 0 mı kontrol et
    jz end                ; 0 ise hiçbir şey yapma

loop:
    mov al, [rdi]           ; Bellekteki piksel değerini AL'ye al
    
    ; Negatif filtre mantığı: 255 - deger
    mov bl, 255
    sub bl, al              ; 255'ten piksel değerini çıkar
    mov [rdi], bl           ; Sonucu tekrar belleğe (RDI'ya) yaz
    
    inc rdi                 ; Bir sonraki bayta geç
    dec rsi                 ; Sayacı azalt
    jnz loop               ; RSI sıfır değilse devam et

end:
    ret