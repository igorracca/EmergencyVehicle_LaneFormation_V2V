# EmergencyVehicle_LaneFormation_V2V

In this project, a traffic simulation was created using with Veins framework. The traffic scenario is a straight road with two lanes, passenger cars and one emergency vehicle.

in this simulation, a virtual lane is created for the emergency vehicle, when this vehicle approaches, all the passanger vehicles ahead change to the right lane, leaving the other lane free for the emergency vehicle. This communication was made using V2V messages, where the emergency vehicle sends WSM messages to the other vehicles, and they replicate the message after 20 seconds.

<br />

## Contribution

1. This project is based on Veins sample project. 

Veins - The open source vehicular network simulation framework. See the Veins website <http://veins.car2x.org/> for a tutorial, documentation,
and publications.

2. Two functions were implemented based on Plexe framework. (https://github.com/michele-segata/plexe)

Plexe is a cooperative driving framework extending SUMO and Veins permitting the realistic simulation of platooning (http://plexe.car2x.org/)


 <br />

## Requirements

1. Python
2. Sumo (https://www.eclipse.org/sumo/)
3. Omnet++ (https://omnetpp.org/)
4. Veins (http://veins.car2x.org/)

Installation tutorial can be found in: http://veins.car2x.org/tutorial/

### Recommended versions:

- sumo-1.6.0
- omnetpp-5.5.1
- veins-veins-5.1

 <br />

## Running

The Project structure follows the Veins sample project structure.
* Install Veins and its sample project
* Copy the content of `examples/veins` from this repository to same directory on sample project
* Do the same for:
  * `src/veins/modules/application/traci/`
  * `src/veins/modules/mobility/traci/`
* Open omnet++ with the command `omnetpp`
* Run the script to proxy TCP connections between OMNeT++ and SUMO (`sumo-launchd.py -vv -c sumo')
* Run the project on omnet++

Running part can also be found on the tutorial: http://veins.car2x.org/tutorial/
