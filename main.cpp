#include <stdio.h>
#include <pthread.h>
#include "VideoCapture.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <mutex>


using boost::asio::ip::udp;
using namespace std::chrono;


typedef struct control_packet {
    uint8_t last_received_id; //(num & 0xFF) last packet id obtained by server
    uint8_t prev_received_id; //(num & 0xFF)
    int32_t last_receive_time; // timestamp
} ctl_pkt;

void control_stream(int port, bool & bitrate_switch, uint32_t & bitrate, std::mutex &m) {
    boost::asio::io_service io_service;
    udp::socket socket(io_service, udp::endpoint(udp::v4(), port));
    uint8_t prev_reseived = 0;
    uint8_t success_sequence = 0;

    while (true) {
        ctl_pkt *res = nullptr;
        udp::endpoint remote_endpoint;
        boost::system::error_code error;
        boost::array<char, sizeof(ctl_pkt)> recv_buf = {0};

        // receive from server struct with id of received packet and timestamp
        socket.receive_from(boost::asio::buffer(recv_buf),
                            remote_endpoint, 0, error);
        if (error && error != boost::asio::error::message_size)
            throw boost::system::system_error(error);

        // cast char * to ctl_pkt
        res = (ctl_pkt *) recv_buf.data();
        m.lock();
        bitrate_switch = true;
        bitrate = res->last_receive_time;
        m.unlock();

        // calculate latency between client-server packet exchange
        // diff = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() & 0xffffffff) - res->last_receive_time;

        // if we had dropped 5+ frames of latency more than 1000 milliseconds reduce bitrate
        //if (res->last_receive_time - prev_reseived > 5 || diff > 1000) {
        //  *change_bitrate = -1;
        //success_sequence = 0;
        //}
        // if we had success(received frames without dropping or latency) increase bitrate
        //if (success_sequence > 10 /* add delimiter to bitrate upper boundary */) {
        //  *change_bitrate = 1;
        //}
        // save  successfully obtained packet id  to prev
        //prev_reseived = res->last_received_id;
        // increase success sequence
        //success_sequence++;}

    }
}

int main() {
    std::mutex mutex;               // mutex to control bitrate (only for udp)

    std::string dev = "/dev/video0"; // cam name
    /* Resolution */
    uint16_t width = 1280;          // frame width
    uint16_t height = 720;          // frame height
    uint16_t fps = 30;              // stream fps
    uint32_t bitrate = 30000000;    // stream bitrate

    // Video cam object
    VideoCapture a(std::string("/dev/video0"), width, height, fps, bitrate);
    // initialize bitrate control switch to off state
    a.get_switch() = false;
    // launch control udp control thread
    std::thread udp_control(control_stream, 9013, std::ref(a.get_switch()), std::ref(a.get_bitrete()), std::ref(mutex));

    // initialize camera
    a.video_init();

    // main frame capture loop
    a.capture_video_stream(mutex);

    // turnoff cam
    a.video_free();
    return 0;
}