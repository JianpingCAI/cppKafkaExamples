#include "../include/MyEventCb.h"

void MyEventCb::event_cb(RdKafka::Event &event)
{
    switch (event.type())
    {
    case RdKafka::Event::EVENT_STATS:
        std::cout << "Stats: " << event.str() << std::endl;
        break;
    case RdKafka::Event::EVENT_ERROR:
        std::cerr << "Error: " << event.str() << std::endl;
        break;
    default:
        std::cout << "Other event: " << event.type() << std::endl;
    }
}
