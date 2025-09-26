#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <iostream>

// Callback: new-sample sinyali geldiğinde çağrılacak
static GstFlowReturn on_new_sample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);
    if (!sample)
    {
        std::cout << "Sample alınamadı (EOS ya da hata)." << std::endl;
        return GST_FLOW_ERROR;
    }

    // sample'dan bufferı çıkarıyoruz, çünkü ham verimiz bufferda. sample'ın içinde ; GstBuffer (ham veri: pikseller, ses sample’ları)
    // GstCaps (verinin formatı)
    // GstSegment (akış bilgisi) vardır.
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstClockTime pts = GST_BUFFER_PTS(buffer); // Buffer’ın zaman damgasını (PTS – Presentation Timestamp) alıyoruz
    std::cout << "Yeni sample geldi! Timestamp: " << GST_TIME_AS_MSECONDS(pts) << " ms" << std::endl;

    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Pipeline oluştur: emit-signals=true; yapmazsan signalleri alamazsın, fonksiyonları kullanmak için yapman gerekli.
    GstElement *pipeline = gst_parse_launch(
        "videotestsrc ! videoconvert ! appsink name=sink emit-signals=true", nullptr);

    if (!pipeline)
    {
        std::cerr << "Pipeline oluşturulamadı!" << std::endl;
        return -1;
    }

    // Cast işlemi
    // Pipeline’ın içinden adı sink olan elementi bul ve bana getir. pipeline içinden sink elementini çekiyoruz, ardından appsinke dönüştürüyoruz.
    // bunu yapmamızın nedeni appsink fonksiyonunu kullanmak için appsinke dönüştürebilmek istememiz, pipeline bir grup oldugu için o veriyi almadan yapamayız.
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    // Appsink elementine dönüştürüyoruz, çünkü appsink fonksiyonu kullanmak istiyoruz.
    GstAppSink *appsink = GST_APP_SINK(sink);

    // appsink üzerinde şu sinyal (olay) tetiklenirse örneğin şuan (new-sample), benim verdiğim fonksiyonu şuan (G_CALLBACK(on_new_sample))çalıştır.
    // nullptr; olan kısım ekstra userdata eklemek istersek kullanılır
    // new-sample sinyaline callback bağlama
    g_signal_connect(appsink, "new-sample", G_CALLBACK(on_new_sample), nullptr);

    // Pipeline’ı oynatmaya başla
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Main loop çalıştır
    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);

    // Temizlik
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
