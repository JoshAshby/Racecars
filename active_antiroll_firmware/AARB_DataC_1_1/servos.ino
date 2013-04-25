//Just a general function to help clean up code with
//servos

void servoWrite(const servos_struct& servos) {
  if(servos.oldFrontRight != servos.frontRight) {
    frontRightServo.write(servos.frontRight);
  }
  if(servos.oldFrontLeft != servos.frontLeft) {
    frontLeftServo.write(servos.frontLeft);
  }
  if(servos.oldRear != servos.rear) {
    rearServo.write(servos.rear);
  }
}
