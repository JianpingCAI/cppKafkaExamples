#ifndef MY_EVENT_CB_H
#define MY_EVENT_CB_H

#include <iostream>
#include <librdkafka/rdkafkacpp.h>

class MyEventCb : public RdKafka::EventCb
{
public:
    void event_cb(RdKafka::Event &event) override;
};

#endif // MY_EVENT_CB_H
