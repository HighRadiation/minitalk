# Minitalk — Uzun Metinlerde Çökme / Bozulma Analizi

Aşağıda projedeki gözlemlerim, uzun metinlerde neden bozulma/çökme olabileceğinin teknik sebepleri ve uygulanabilecek düzeltme önerileri Türkçe olarak derlenmiştir.

## Özet

- Asıl sorun: SIGUSR1/SIGUSR2 gibi klasik sinyaller kuyruklanmaz; istemci çok hızlı sinyal gönderirse bazı sinyaller kaybolabilir veya birleştirilebilir. Bu, uzun mesajlarda daha sık görülür.
- Mevcut istemci 1 ms (usleep(1000)) bekleyerek bitleri yolluyor; bu her ortamda yeterli olmayabilir. Sunucu ise ACK (onay) göndermiyor, dolayısıyla güvenilir teslimat yok.
- `signal()` yerine `sigaction()` ve ACK tabanlı senkronizasyon önerilir.

## Kodda tespit ettiğim kritik noktalar

1. Sinyal kuyruğu ve hız
   - Unix'te klasik sinyaller (SIGUSR1/SIGUSR2) aynı türdeki ardışık sinyallerin hepsini saklamayabilir. Bu yüzden istemci çok kısa aralıklarla kill() çağırırsa sunucu bazı bitleri kaçırabilir.
   - Uzun metin => daha fazla bit => zamanlama sorununa maruz kalma olasılığı artar.

# NOT: BAŞKA SİNYAL TÜRÜ KULLANMALIYIM.


2. Client tarafı: gönderim hızı ve ACK eksikliği
   - `client.c` içinde `send_to_server` her bitten sonra `usleep(1000)` kullanıyor. Bu çoğu durumda çalışır ama kesin değil.
   - En güvenli yöntem: her bit gönderildikten sonra server'dan ACK beklemek (senkronizasyon). Şu an böyle bir mekanizma yok.

3. Server tarafı: sinyal kurulumu ve güvenli fonksiyonlar
   - `server.c` `signal()` kullanıyor. `signal()` platforma göre farklı davranış gösterebilir; `sigaction()` daha öngörülebilirdir.
   - Handler içinde sadece `write()` kullanılmış — bu iyi çünkü `write` async-signal-safe fonksiyondur. Ancak başka non-safe fonksiyonlar kullanılması sorun yaratır.

4. Küçük ama önemli hatalar
   ## (düzeltildi) - `write(1, "\nMessage received.", 19);` satırındaki 19 değeri yanlış (string uzunluğu yanlış verilmiş). Bu hatalı uzunluk kullanımı dışarıdan rastgele byte okumaya sebep olabilir; blokiçinde çok kritik çökme olmasa da düzeltilmeli.
   - `client.c`'de sonlandırma gönderme (null terminator) mantığı yanıltıcı. `procces_message` içinde dışta `while (shift >= 0)` gibi tekrar eden bir blok var; amaç `\0` göndermekse daha net bir implementasyon gerek.

## Neden uzun metinler daha çok sorun çıkar?

- Daha fazla sinyal gönderimi demek; zamanlama hatalarının gözlemlenme olasılığı artar.
- `usleep(1000)` yeterli değilse birbiri ardına gönderilen sinyaller sunucu tarafından eksik alınır.

## Önerilen düzeltmeler (tercih sırasına göre)

1. En sağlam çözüm — ACK tabanlı senkron iletişim (önerilen)
   - Server: `sigaction` ile handler tanımla ve `siginfo_t` kullanarak gönderen PID'i al. Her bite başarıyla ulaşıldığında `kill(si_pid, SIGUSR1)` gibi bir ACK yolla.
   - Client: her bit gönderildikten sonra ACK beklesin (örn. `sigsuspend()` ile). ACK geldikten sonra sonraki biti göndersin.
   - Bu yaklaşım güvenilirliği çok artırır; zamanlama/kuyruk problemleri pratikte ortadan kalkar.

2. Daha basit ama daha az güvenilir: beklemeyi artır
   - `usleep(1000)` → `usleep(3000)` veya `usleep(5000)` gibi daha uzun bekleme koymak kısa vadede yardımcı olabilir. Ancak sistem koşulları değişince yine sorun çıkabilir.

3. `signal()` → `sigaction()`
   - `sigaction` kullanmak handler davranışını öngörülebilir yapar ve `SA_SIGINFO` ile `siginfo_t` almayı sağlar.

4. Diğer küçük düzeltmeler
   - `write` çağrılarındaki uzunlukları düzelt (ör. `"\nMessage received."` için doğru uzunluğu ver).
   - Handler içindeki statik değişkenler için gerektiğinde `volatile sig_atomic_t` kullanımı değerlendirilebilir.
   - `client.c`'de `procces_message` fonksiyonundaki son null-terminator gönderimini netleştir.

## Küçük kontrol listesi / adımlar (implementasyon için)

- [ ] Server: `sigaction` + SA_SIGINFO ile sinyal handler oluştur
- [ ] Server: handler her bite karşılık `kill(si_pid, SIGUSR1)` ile ACK yollasın
- [ ] Client: `send_to_server` sonrası ACK beklesin (`sigsuspend`/`pause` ile)
- [ ] `write` uzunluklarını düzelt
- [ ] (İsteğe bağlı) `volatile sig_atomic_t` kullanımı gözden geçir

## Nasıl test edilir / tekrar üretme

1. Terminal A: `./server` çalıştırın, PID'yi not edin.
2. Terminal B: `./client <PID> "çok uzun metin..."` çalıştırın.
3. Eğer çıktı bozuk/eksikse, bit kaybı veya fazla hızdan kaynaklanan sinyal kaybı muhtemeldir.
4. Gerçek çökme (process crash) varsa `dmesg` veya core/loglara bakın; fakat çoğunlukla "çökme" dedikleri şey çıktı bozulmasıdır.

## Sonuç / Önerilen sonraki adım

- Size iki seçenek sunmuştum: A) En sağlam değişiklikleri (ACK tabanlı, `sigaction`) uygulayayım; B) Hızlı düzeltme (usleep arttırma + küçük yazım düzeltleri).
- Eğer onay verirseniz ben A seçeneğini uygulayıp derleyip kısa testler çalıştırırım ve sonuçları buraya raporlarım.

---
Dosya oluşturuldu: `ANALIZ.md` — isterseniz dosya adını değiştirebilirim veya içeriği daha kısa/uzun hâle getirebilirim. Bir sonraki adımda hangi seçeneği uygulamamı istersiniz?
