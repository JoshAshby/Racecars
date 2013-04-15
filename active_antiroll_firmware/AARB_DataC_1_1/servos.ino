void servoWrite(int *pulseBuffer) {
  frontRightServo.write(pulseBuffer[0]);
  frontLeftServo.write(pulseBuffer[1]);
  rearServo.write(pulseBuffer[2]);
}
