//
// Created by jin on 15.04.19.
//

#ifndef WB_VIDEOCAPTURE_H
#define WB_VIDEOCAPTURE_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <linux/videodev2.h>
#include <time.h>
#include <linux/uvcvideo.h>


extern "C" {
#include "libv4l2-hw.h"
#include "UvcH264.h"
}

#include <pthread.h>
#include <mutex>
#include <vector>

// uvc XU
#define SET_CUR 0x01
#define GET_CUR 0x81
#define GET_MIN 0x82
#define GET_MAX 0x83
#define GET_LEN 0x85
#define GET_INFO 0x86

typedef enum {
    IO_METHOD_READ,
    IO_METHOD_MMAP,
    IO_METHOD_USERPTR,
} io_method;

struct buffer {
    void	*start;
    size_t	length;
};

class VideoCapture {
public:
    VideoCapture();
    VideoCapture(std::string device, uint16_t width, uint16_t height, uint16_t fps, uint32_t bitrate);
    void        set_params(std::string &dev,uint16_t width, uint16_t height, uint16_t fps, uint32_t bitrate);
    int         video_init(void);
    int         video_free(void);
    void        capture_video_stream(std::mutex &);
    void        set_new_bitrate(uint32_t bitrate);
    bool        &get_switch(){ return  bitrate_switch_;}
    uint32_t    &get_bitrete(){ return bitrate_;}
    void        video_set_power_line_freq(int freq);

private:
    int _open_device();
    int _init_device();
    int _start_capturing();
    int _shot(u_int32_t tout);
    int _stop_capturing();
    int _uninit_device();
    int _close_device();
    int _process_image(int n, int used_bytes);
    int _init_userp(uint buffer_size);
    int _init_mmap();
    int _init_read(uint buffer_size);
    int _read_frame();
    int video_H264_set_bitrate(int bitrate);
    int video_H264_set_framerate(int framerate);
    int video_H264_get_key_frame(int type);

private:
    void _measure_bitrate(int used_bytes);



private:
    typedef  std::chrono::time_point<std::chrono::system_clock> time_stamp_t;

    uint16_t                height_;
    uint16_t                width_;
    uint16_t                fps_;
    uint32_t                bitrate_;
    uint32_t                current_cnt_;
    bool                    bitrate_switch_;
    std::string             device_name_;
    time_stamp_t            init_point_;
    int						fd;
    io_method				io;
    struct buffer			*buffers;
    int						buffer_last;
    u_int32_t				n_buffers;
    u_int32_t				bytesperline;
    u_int32_t				fmt_;
    u_int32_t				field;
    int						error;
    int						init_ok;
    int						start_ok;
    struct v4l2_capability	cap;
    int						needShot;
    struct buffer			frame;
    int						raw;
    struct v4l2_buffer		v4l2_buf;
    int8_t					H264_ctrl_unit_id;
    int8_t					need_key_frame;
    std::string				name_;
    std::string				uevent_;
    int						busnum;
    int						devnum;
    int						PL_FREQ;	// power line freq

};


#endif //WB_VIDEOCAPTURE_H
