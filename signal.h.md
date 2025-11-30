# Signal.h Kütüphanesi Fonksiyonları

## 1. `sigemptyset(&mask)`
- **Amaç**: Bir signal kümesini (set) tamamen boşaltır
- **Kullanım**: `sigemptyset(&mask);`
- **Ne yapar**:
  - Tüm sinyalleri kümeden çıkarır
  - Yeni bir signal maskesi oluşturmadan önce kullanılır
  - Temiz bir başlangıç noktası sağlar

## 2. `sigaddset(&mask, SIGUSR1)`
- **Amaç**: Belirli bir sinyali signal kümesine ekler
- **Kullanım**: `sigaddset(&mask, SIGUSR1);`
- **Ne yapar**:
  - İkinci parametredeki sinyali (SIGUSR1) maskeye ekler
  - Hangi sinyallerin bloklanacağını/dinleneceğini belirler
  - Birden fazla sinyal eklemek için birden fazla kez çağrılabilir

## 3. `sigprocmask(SIG_BLOCK, &mask, &old_mask)`
- **Amaç**: Process'in signal maskesini değiştirir
- **Kullanım**: `sigprocmask(SIG_BLOCK, &mask, &old_mask);`
- **Parametreler**:
  - **SIG_BLOCK**: Belirtilen sinyalleri bloklar (mevcut maskeye ekler)
  - **SIG_UNBLOCK**: Sinyalları bloktan çıkarır
  - **SIG_SETMASK**: Maskeyi tamamen değiştirir
  - **&mask**: Değiştirilecek sinyallerin kümesi
  - **&old_mask**: Eski maskeyi saklar (geri yüklemek için)

## 4. `sigsuspend(&old_mask)`
- **Amaç**: Signal maskesini geçici olarak değiştirir ve signal bekler
- **Kullanım**: `sigsuspend(&old_mask);`
- **Ne yapar**:
  - Atomik operasyon (race condition önler)
  - Maskeyi geçici olarak old_mask ile değiştirir
  - Signal gelene kadar process'i uyutur
  - Signal gelince ve handler çalışınca eski maskeye döner
  - SIGUSR1 bloklu olduğu için race condition önlenir

## 5. `sigaction(SIGUSR1, &sa, NULL)`
- **Amaç**: Bir signal için handler (işleyici) tanımlar
- **Kullanım**: `sigaction(SIGUSR1, &sa, NULL);`
- **Parametreler**:
  - **SIGUSR1**: İşlenecek signal türü
  - **&sa**: Signal action yapısı (handler, flags, mask içerir)
  - **NULL**: Eski action'ı saklamaya gerek yok
- **struct sigaction içeriği**:
  ```c
  sa.sa_handler = ack_handler;  // Handler fonksiyonu
  sa.sa_mask                    // Handler çalışırken bloklanacak sinyaller
  sa.sa_flags = 0;              // Ek ayarlar (SA_RESTART vb.)
  ```

## 6. `kill(server_pid, SIGUSR1)`
- **Amaç**: Belirtilen process'e signal gönderir
- **Kullanım**: `kill(server_pid, SIGUSR1);`
- **Parametreler**:
  - **server_pid**: Hedef process ID
  - **SIGUSR1/SIGUSR2**: Gönderilecek signal türü
- **Dönüş**: Başarılı ise 0, hata varsa -1

## 7. `sig_atomic_t`
- **Amaç**: Signal handler içinde güvenle kullanılabilen veri tipi
- **Kullanım**: `volatile sig_atomic_t g_received = 0;`
- **Özellikler**:
  - Atomik okuma/yazma garantisi
  - Signal handler içinde güvenle değiştirilebilir
  - `volatile` ile derleyici optimizasyonunu engeller

---

## Kodunuzdaki Kullanım Akışı

```
1. sigemptyset(&mask)           → Temiz maske oluştur
2. sigaddset(&mask, SIGUSR1)    → SIGUSR1'i maskeye ekle
3. sigprocmask(SIG_BLOCK, ...)  → SIGUSR1'i blokla
4. g_received = 0                → Bayrağı sıfırla
5. kill(server_pid, signal)     → Server'a bit gönder
6. sigsuspend(&old_mask)        → Onay bekle (blok geçici kalkar)
7. sigprocmask(SIG_UNBLOCK,...) → SIGUSR1 bloğunu tamamen kaldır
```

## Race Condition Önleme Mantığı

- **Problem**: Signal gönderip hemen beklemek arasında signal gelebilir
- **Çözüm**:
  1. Önce SIGUSR1'i blokla
  2. Bayrağı sıfırla
  3. Signal gönder
  4. `sigsuspend` ile atomik olarak aç ve bekle
  5. Signal gelir, handler çalışır, bayrak 1 olur
  6. Loop'tan çık, bloğu kaldır
