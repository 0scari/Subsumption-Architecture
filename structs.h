//
// Created by Oskars Pozdnakovs on 2019-03-22.
//

#ifndef SUBSUMPTION_ARCHITECTURE_STRUCTS_H
#define SUBSUMPTION_ARCHITECTURE_STRUCTS_H

struct Sensor {
    bool is_activated = false;
} sensor1, sensor2, sensor3;

struct Wheel {
    int id;
    bool is_coordinator_signal_received;
    string movement_direction = "forward";
    Sensor* sensor;
} wheel1, wheel2, wheel3;

struct ResolutionInstrSet {
    string wheel1;
    string wheel2;
    string wheel3;
};

struct Coordinator {
    Wheel *wheels[3];
    vector<ResolutionInstrSet> instuction_queue;
} coordinator;

#endif //SUBSUMPTION_ARCHITECTURE_STRUCTS_H
