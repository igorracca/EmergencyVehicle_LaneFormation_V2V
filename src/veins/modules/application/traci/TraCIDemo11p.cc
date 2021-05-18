//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/TraCIDemo11p.h"

#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::TraCIDemo11p);

void TraCIDemo11p::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
    }
}

void TraCIDemo11p::onWSA(DemoServiceAdvertisment* wsa)
{
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
        currentSubscribedServiceId = wsa->getPsid();
        if (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService(static_cast<Channel>(wsa->getTargetChannel()), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}

void TraCIDemo11p::onWSM(BaseFrame1609_4* frame)
{
    TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);

    senderID = wsm->getSenderID();  // get senderID
    EV_WARN << "node[" << getParentModule()->getIndex() << "]: WSM message received from node[" << senderID << "] /n";

    // if you are not the emergency vehicle but received a message from it
    if(senderID == EMERGENCY_VEHICLE_NODE && EMERGENCY_VEHICLE_NODE != getParentModule()->getIndex()) {
        findHost()->getDisplayString().setTagArg("i", 1, "yellow"); // change tag color to yellow

        traciVehicle->changeLane(0, 2); // vehicle change to left lane
        traciVehicle->setLaneChangeMode(0b0000001000); // vehicle stays in the left lane //b1=00 b2=00 b3=00 b4=10 b5=10 b6=00
        EV_WARN << "node[" << getParentModule()->getIndex() << "]: LaneChangeMode set";

        // broadcast message
        if (!sentMessage) {
             sentMessage = true;
             // repeat the received traffic update once in 20 seconds plus some random delay
             wsm->setSenderAddress(myId);
             wsm->setSerial(3);
             scheduleAt(simTime() + 20 + uniform(0.01, 0.2), wsm->dup());
        }
    }
}

void TraCIDemo11p::handleSelfMsg(cMessage* msg)
{
    if (TraCIDemo11pMessage* wsm = dynamic_cast<TraCIDemo11pMessage*>(msg)) {
        // send this message on the service channel until the counter is 3 or higher.
        // this code only runs when channel switching is enabled
        sendDown(wsm->dup());
        wsm->setSerial(wsm->getSerial() + 1);
        if (wsm->getSerial() >= 3) {
            // stop service advertisements
            stopService();
            delete (wsm);
        }
        else {
            scheduleAt(simTime() + 1, wsm);
        }
    }
    else {
        DemoBaseApplLayer::handleSelfMsg(msg);
    }
}

void TraCIDemo11p::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    nodeID = getParentModule()->getIndex();

    if (nodeID == EMERGENCY_VEHICLE_NODE) {
        EV_INFO << "Emergency Vehicle found (NodeId = " << EMERGENCY_VEHICLE_NODE << ") \n";

        // schedule message
        if (!sentMessage) {
            sentMessage = true;

            TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();
            populateWSM(wsm);
            wsm->setSenderID(nodeID);

            wsm->setSerial(3);
            scheduleAt(simTime() + 1, wsm->dup());
        }
    }
    else {
        lastDroveAt = simTime();
    }
}
