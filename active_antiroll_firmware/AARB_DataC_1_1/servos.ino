//Just a general function to help clean up code with
//servos

void servoWrite(const servos_struct& pulseBuffer) {
  frontRightServo.write(pulseBuffer.frontRight);
  frontLeftServo.write(pulseBuffer.frontLeft);
  rearServo.write(pulseBuffer.rear);
}
