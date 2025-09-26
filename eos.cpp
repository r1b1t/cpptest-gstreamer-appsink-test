#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <iostream>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Pipeline: 10 frame üretsin ve dursun
    GstElement *pipeline = gst_parse_launch(
        "videotestsrc num-buffers=10 ! videoconvert ! appsink name=sink emit-signals=false",
        nullptr);

    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    GstAppSink *appsink = GST_APP_SINK(sink);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Burada main loop yerine döngüye giriyoruz
    while (true)
    {
        if (gst_app_sink_is_eos(appsink))
        {
            std::cout << "EOS alındı! Akış bitti." << std::endl;
            break;
        }

        GstSample *sample = gst_app_sink_pull_sample(appsink);
        if (sample)
        {
            std::cout << "Frame geldi." << std::endl;
            gst_sample_unref(sample);
        }
    }

    // Temizlik
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
