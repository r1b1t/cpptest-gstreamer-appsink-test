Appsink Nedir? (Türkçe)

appsink, GStreamer pipeline’ındaki veriyi (ör. video frame, ses sample’ı vb.) uygulamanın kendi koduna alabilmeni sağlar. Normalde filesink veya autovideosink gibi elementler veriyi dosyaya ya da ekrana gönderir. Ama appsink ile veriyi uygulama kodunda alıp işleyebilirsin.

Kullanım Senaryoları

Video karelerini alıp OpenCV ile işlemek

Ses sample’larını alıp analiz etmek

Veriyi network üzerinden başka yere göndermek

Önemli Sinyaller
1. Sample Signal (new-sample)

Appsink’e yeni bir veri (sample) geldiğinde tetiklenir.

Callback içinde gst_app_sink_pull_sample() ile sample alınır.

Amaç: Uygulamaya “yeni veri hazır” demek (ör. video frame, ses bloğu).

Örnek: Her yeni frame geldiğinde timestamp yazmak veya işlemek.

Alternatif:
try-pull-sample

Appsink kuyruğundan bir sample çekmeye çalışır.

Timeout süresince (örn. 2 sn) bekler.

Sonuç:

Sample gelirse → GstSample* döner

Timeout olursa → NULL döner

EOS (End Of Stream) olursa → NULL döner (kontrol için gst_app_sink_is_eos())

2. EOS Signal (eos)

Pipeline’daki kaynak bittiğinde tetiklenir (örn. videotestsrc num-buffers=100).

Anlamı: Veri bitti, artık sample gelmeyecek.

Kullanım: Kaynağı kapatmak, belleği temizlemek, “oynatma bitti” mesajı göstermek.

3. New Preroll Signal (new-preroll)

Pipeline PAUSED durumuna geçtiğinde ilk sample hazır olduğunda tetiklenir.

Bu sample’a preroll sample denir.

Kullanım:

Seek sonrası ilk frame’i almak

Pause modundayken ekranda son görüntüyü göstermek

4. New Serialized Event (new-serialized-event)

Downstream serialized event (ör. TAG, SEGMENT, CUSTOM) appsink’e ulaştığında tetiklenir.

EOS bu sinyalle gelmez, ayrı eos sinyaliyle yakalanır.

Sadece emit-signals=true ise çalışır.

Kullanım Alanları:

Metadata (örn. TAG → şarkı adı, sanatçı bilgisi)

Segment değişiklikleri

Uygulamanın tanımladığı özel eventler

5. Propose Allocation (propose-allocation)

Downstream element, appsink’e “belleği nasıl ayıralım?” diye sorduğunda tetiklenir.

Amaç: Bellek ayırma stratejisini (allocation strategy) belirlemek.

Kullanım: Performans optimizasyonu, özel memory pool, DMA buffer, GPU memory, zero-copy işlemler.

Not: Basit uygulamalarda gerek yok, GStreamer otomatik uygun seçim yapar.

What is Appsink? (English)

appsink allows you to retrieve data (e.g., video frames, audio samples) from the GStreamer pipeline into your own application code. Normally, elements like filesink or autovideosink send data to a file or the screen, but with appsink, you can pull the data directly into your code for processing.

Use Cases

Capture video frames and process them with OpenCV

Capture audio samples for analysis

Forward data over the network

Key Signals
1. Sample Signal (new-sample)

Triggered when new data (sample) arrives at appsink.

Use gst_app_sink_pull_sample() in the callback to retrieve the sample.

Purpose: Notify the application that “new data is ready” (video frame, audio block).

Example: Print a timestamp or process each new frame.

Alternative:
try-pull-sample

Attempts to pull a sample from the appsink queue.

Waits up to a given timeout (e.g., 2 seconds).

Results:

Sample available → returns GstSample*

Timeout → returns NULL

EOS (End Of Stream) → returns NULL (check with gst_app_sink_is_eos())

2. EOS Signal (eos)

Triggered when the source in the pipeline finishes (e.g., videotestsrc num-buffers=100).

Meaning: No more samples will arrive.

Usage: Close source, release memory, or display “playback finished” message.

3. New Preroll Signal (new-preroll)

Triggered when the pipeline enters PAUSED state and the first sample becomes ready.

This sample is called the preroll sample.

Usage:

Get the first frame after seeking

Show the last frame while in pause mode

4. New Serialized Event (new-serialized-event)

Triggered when a downstream serialized event (e.g., TAG, SEGMENT, CUSTOM) reaches appsink.

EOS does not arrive via this signal—it has its own eos signal.

Works only if emit-signals=true.

Use Cases:

Metadata (e.g., TAG → song title, artist info)

Segment changes

Application-defined custom events

5. Propose Allocation (propose-allocation)

Triggered when a downstream element asks appsink “how should we allocate memory?”.

Purpose: Define the buffer allocation strategy.

Use Cases: Performance optimization, custom memory pools, DMA buffer, GPU memory, zero-copy processing.

Note: For most simple applications, you don’t need this—GStreamer chooses suitable defaults.
