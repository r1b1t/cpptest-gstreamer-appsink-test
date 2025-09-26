#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <iostream>

// new-preroll callback
static GstFlowReturn on_new_preroll(GstAppSink *appsink, gpointer user_data)
{
    (void)user_data;

    GstSample *sample = gst_app_sink_pull_preroll(appsink);
    if (!sample)
    {
        std::cout << "Preroll sample alınamadı." << std::endl;
        return GST_FLOW_ERROR;
    }

    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstClockTime pts = GST_BUFFER_PTS(buffer);
    std::cout << "Yeni PREROLL sample geldi! Timestamp: "
              << GST_TIME_AS_MSECONDS(pts) << " ms" << std::endl;

    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Pipeline: 1 frame üretecek
    GstElement *pipeline = gst_parse_launch(
        "videotestsrc num-buffers=1 ! videoconvert ! appsink name=sink emit-signals=true",
        nullptr);

    // Appsink elementini al
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    GstAppSink *appsink = GST_APP_SINK(sink);

    // new-preroll sinyalini bağla
    g_signal_connect(appsink, "new-preroll", G_CALLBACK(on_new_preroll), nullptr);

    // Pipeline’ı PAUSED yap → preroll tetiklenecek
    gst_element_set_state(pipeline, GST_STATE_PAUSED);

    // Main loop
    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);

    // Temizlik
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
