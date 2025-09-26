#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <iostream>

// new-serialized-event callback
static gboolean on_new_serialized_event(GstAppSink *appsink, gpointer user_data)
{
    (void)user_data;

    // Event'i çek
    GstMiniObject *obj = gst_app_sink_pull_object(appsink);
    if (!obj)
    {
        std::cout << "[EVENT] Obj alınamadı." << std::endl;
        return FALSE;
    }

    if (GST_IS_EVENT(obj))
    {
        GstEvent *event = GST_EVENT(obj);
        std::cout << "[EVENT] Yeni serialized event geldi: "
                  << GST_EVENT_TYPE_NAME(event) << std::endl;
    }
    else
    {
        std::cout << "[EVENT] Sample geldi (event değil)." << std::endl;
    }

    gst_mini_object_unref(obj);
    return TRUE; // Event'i biz işledik
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Basit pipeline (eventler akarken appsink yakalayacak)
    GstElement *pipeline = gst_parse_launch(
        "videotestsrc num-buffers=3 ! videoconvert ! appsink name=sink emit-signals=true",
        nullptr);

    // Appsink elementini al
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    GstAppSink *appsink = GST_APP_SINK(sink);

    // new-serialized-event sinyalini bağla
    g_signal_connect(appsink, "new-serialized-event", G_CALLBACK(on_new_serialized_event), nullptr);

    // Çalıştır
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Main loop
    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);

    // Temizlik
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
