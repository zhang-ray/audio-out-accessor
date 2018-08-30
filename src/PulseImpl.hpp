#pragma once

#include "aoa/ReturnType.hpp"

#include <fstream>
#include <memory>

#include <pulse/stream.h>
#include <pulse/context.h>
#include <pulse/introspect.h>
#include <pulse/thread-mainloop.h>

namespace {

pa_threaded_mainloop *mainLoop = nullptr;
std::shared_ptr<std::ofstream> pFileDumper = nullptr;


class PulseLocker{
public:
    PulseLocker(pa_threaded_mainloop *ml)
        :ml_(ml) {
        pa_threaded_mainloop_lock(ml_);
    }


    ~PulseLocker() {
        pa_threaded_mainloop_unlock(ml_);
    }


private:
    pa_threaded_mainloop *ml_;
};
}



class PulseImpl {
public:

    PulseImpl(){
        pa_threaded_mainloop_start(mainLoop = pa_threaded_mainloop_new());

        PulseLocker locker(mainLoop);
        context_ = pa_context_new(pa_threaded_mainloop_get_api(mainLoop), "AOA");
        pa_context_set_state_callback(context_,[&](pa_context *, void *)->void{pa_threaded_mainloop_signal(mainLoop, 0);}, nullptr);
        pa_context_connect(context_, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr);
    }


    ~PulseImpl(){
        if (stream_){
            stop();
        }

        {
            PulseLocker locker(mainLoop);
            pa_context_disconnect(context_);
            pa_context_unref(context_);
        }

        if (mainLoop){
            pa_threaded_mainloop_stop(mainLoop);
            pa_threaded_mainloop_free(mainLoop);
            mainLoop=nullptr;
        }
    }


    PulseImpl(const PulseImpl &) = delete;


    ReturnType dumpDevices(){
        if (!wait()){
            return "wait() failed";
        }

        PulseLocker locker(mainLoop);
        auto op = pa_context_get_sink_info_list(context_, [=](pa_context *, const pa_sink_info *info, int eol,void *userdata) -> void
        {
            if (eol == 0 && info->monitor_source != PA_INVALID_INDEX) {
                //printf("%s:%s\n", info->description, info->monitor_source_name);
                ((PulseImpl *)userdata)->monitor_source_name_ = info->monitor_source_name;
            }
            pa_threaded_mainloop_signal(mainLoop, 0);
        }
        , this);

        for (;PA_OPERATION_RUNNING==pa_operation_get_state(op);){
            pa_threaded_mainloop_wait(mainLoop);
        }

        return 0;
    }


    ReturnType start() {
        if (!wait()){
            return "wait() failed";
        }

        {
            PulseLocker locker(mainLoop);

            auto op =pa_context_get_server_info(context_, [=] (pa_context *, const pa_server_info *info, void *userdata) -> void{
                auto myData = (PulseImpl*) userdata;
                myData->server_name_ = info->server_name;
                myData->server_version_ = info->server_version;
                pa_threaded_mainloop_signal(mainLoop, 0);
            }
            , this);

            if (!op){
                return "!pa_context_get_server_info";
            }

            for (;PA_OPERATION_RUNNING==pa_operation_get_state(op);){
                pa_threaded_mainloop_wait(mainLoop);
            }
        }

        getSourceInfo();


        if (!pa_sample_spec_valid(&spec_)) {
            return "!pa_sample_spec_valid";
        }

        // auto bytes_per_frame = pa_frame_size(&spec);



        stream_ = pa_stream_new(context_, monitor_source_name_.c_str(), &spec_, nullptr);

        {
            PulseLocker locker(mainLoop);
            pa_stream_set_read_callback(stream_, [=](pa_stream *, size_t , void *userdata) {
                auto pulseImpl = (PulseImpl *)userdata;
                if (pulseImpl->stream_){
                    const void *data;
                    size_t nbytes;
                    pa_stream_peek(pulseImpl->stream_, &data, &nbytes);

                    // check if we got data
                    if (nbytes && data) {
                        //printf("%u\n",nbytes);
                        if (pFileDumper) {
                            auto myBuffer = (const char *)data;
                            pFileDumper->write(myBuffer, nbytes);
                        }
                    }
                    pa_stream_drop(pulseImpl->stream_);
                }
                pa_threaded_mainloop_signal(mainLoop, 0);
            }
            ,
            (void *)this);
        }

        pa_buffer_attr attr{(uint32_t)-1,(uint32_t)-1,(uint32_t)-1,(uint32_t)-1,(uint32_t)-1};
        attr.fragsize  = pa_usec_to_bytes(10000/*microsecond, 10 millisecond, 0.01s*/, &spec_);

        {
            PulseLocker locker(mainLoop);
            int ret = pa_stream_connect_record(stream_, monitor_source_name_.c_str(), &attr, PA_STREAM_ADJUST_LATENCY);

            if (ret < 0) {
                stop();
                return "Unable to connect to stream";
            }
        }

        // for debug
        static int counter = 0;
        pFileDumper.reset(new std::ofstream(std::string(std::to_string(counter++)+".pcm").c_str()));

        return 0;
    }


    ReturnType stop(){
        if (stream_) {
            PulseLocker locker(mainLoop);
            pa_stream_disconnect(stream_);
            pa_stream_unref(stream_);
            stream_ = nullptr;
        }

        return 0;
    }



private:
    ReturnType getSourceInfo(){
        if (!wait()) {
            return "wait() failed";
        }

        PulseLocker locker(mainLoop);

        auto op = pa_context_get_source_info_by_name(context_, monitor_source_name_.c_str(), [=](pa_context *, const pa_source_info *info, int eol, void *userdata) -> void {
            auto pulseImpl = (PulseImpl *)userdata;
            if (eol < 0) {
                pulseImpl->spec_.format = PA_SAMPLE_INVALID;
                pa_threaded_mainloop_signal(mainLoop, 0);
                return;
            }

            // more than one instance?
            if (eol > 0){
                pa_threaded_mainloop_signal(mainLoop, 0);
                return;
            }

            pulseImpl->spec_ = info->sample_spec;
            pa_threaded_mainloop_signal(mainLoop, 0);
        }
        , this);


        if (!op) {
            return "pa_context_get_source_info_by_name != 0";
        }

        for (;PA_OPERATION_RUNNING==pa_operation_get_state(op);){
            pa_threaded_mainloop_wait(mainLoop);
        }

        return 0;
    }


    ReturnType wait() {
        PulseLocker locker(mainLoop);

        if (!PA_CONTEXT_IS_GOOD(pa_context_get_state(context_))) {
            return "!PA_CONTEXT_IS_GOOD";
        }

        for ( ; PA_CONTEXT_READY != pa_context_get_state(context_) ; ){
            pa_threaded_mainloop_wait(mainLoop);
        }

        return 0;
    }


private:
    pa_context *context_ = nullptr;
    pa_stream *stream_ = nullptr;
    pa_sample_spec spec_;
    std::string monitor_source_name_;
    std::string server_name_;
    std::string server_version_;
};


