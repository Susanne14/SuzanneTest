#include <iostream>
#include "car_controller.hpp"
using namespace std;

	


	
    cluon::OD4Session motorChannel(MOTOR_CHANNEL,[](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            std::cout << "Sent a message for ground steering to " << receivedMsg.steeringAngle() << "." << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            std::cout << "Sent a message for pedal position to " << receivedMsg.percent() << "." << std::endl;
        }
    });


	opendlv::proxy::GroundSteeringReading msgSteering;
   	opendlv::proxy::PedalPositionReading msgPedal;

int main() {
		
	std::shared_ptr<car_controller> remoteSending = std::make_shared<car_controller>();
	
	unsigned char direction;

	std::cout << "         Group 4 Car Controller             " << std::endl;
	std::cout << "                            " << std::endl;
	std::cout << "Control the car with following commands" << std::endl;
	std::cout << "           w          " << std::endl;
	std::cout << "    a             d   " << std::endl;
	std::cout << "           s          " << std::endl;
	std::cout << "Press c to emergency stop and quit" << std::endl;
	std::cout << "                                  " << std::endl;
   
   while (true) {

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	
        std::cin >> direction;

                switch (direction) {

                    // move forward 
		    
                    case 'w': {
                        
			remoteSending->move_forward();
                        motorChannel.send(msgSteering);
                        motorChannel.send(msgPedal);
                         
                         break;
           		          }

                    // move backward 
                     case 's': {
                         
                        remoteSending->move_backward();
                        motorChannel.send(msgSteering);
                        motorChannel.send(msgPedal);

                         break;
           		          }
                    // Turn left
                     case 'a': { 

			remoteSending->turn_left();
                        motorChannel.send(msgSteering);
                        motorChannel.send(msgPedal);
                         
                         break;
            		         }
                    // Turn right 
                     case 'd': { 

			remoteSending->turn_right();
                        motorChannel.send(msgSteering);
                        motorChannel.send(msgPedal);

                         break;
            		         }
                    // Stops
                     case 'c': {

			remoteSending->Stop();
                        motorChannel.send(msgSteering);
                        motorChannel.send(msgPedal);
			return -1;


                		     }
                     default: 
			{
			std::cout << "Command doesn't exist, type another command "<< std::endl;

			break;
			}                 
		}
	}
	return 0;
}



	// Changing the speed pedal 
	float pedalPercent;
				

void car_controller::move_forward() {

                        msgSteering.steeringAngle(0.0);
                        //the car should move forward  as long as the speed is below 40%.
                	if (pedalPercent <= 0.4){
                            pedalPercent += 0.06;
                        }else{std::cout << "Speed is on maximum " << std::endl;
			}

			std::cout << "Move forward to  " << pedalPercent * 100 << "%." << std::endl;
                        msgPedal.percent(pedalPercent);


}

void car_controller::move_backward() {    // the speed is above 10%.

                        msgSteering.steeringAngle(0.0);
 
                        if (pedalPercent >= 0.20){
                            pedalPercent -= 0.05;
                        }

                        std::cout << "Move backward to " << pedalPercent * 100 << "%." << std::endl;
                        msgPedal.percent(pedalPercent);

}

void car_controller::turn_left() {        // the speed is over 25%.

                        
                        if (pedalPercent >= 0.25){
                        pedalPercent = 0.25;}
			std::cout << "turned left  " << pedalPercent * 100 << "%." << std::endl;
                        msgSteering.steeringAngle(-15.0);

}

void car_controller::turn_right() {      // the speed is over 25%,slow down.

                        
                        if (pedalPercent >= 0.25){
                        pedalPercent = 0.25;}
			std::cout << "turned right" << pedalPercent * 100 << "%." << std::endl;
                        msgSteering.steeringAngle(15.0);

}

void car_controller::Stop() {

                        pedalPercent = 0;
			msgPedal.percent(pedalPercent);
                        msgSteering.steeringAngle(0.0);
                        std::cout << "Car stops" << std::endl;
}
