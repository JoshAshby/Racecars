void frontLeftWheelSpeed(void) {
  pulsewidthfl = pulseIn(speedsensorfl, HIGH, wheelTimeout);
  if (pulsewidthfl == 0) {
    wheelSpeeds.frontLeft = 0;
  } else {
    wheelSpeeds.frontLeft = wheelDivisor / pulsewidthfl;
  }
}

void frontRightWheelSpeed(void) {
  pulsewidthfr = pulseIn(speedsensorfr, HIGH, wheelTimeout);
  if (pulsewidthfr == 0) {
    wheelSpeeds.frontRight = 0;
  } else {
    wheelSpeeds.frontRight = wheelDivisor / pulsewidthfr;
  }
}

void rearLeftWheelSpeed(void) {
  pulsewidthrl = pulseIn(speedsensorrl, HIGH, wheelTimeout);
  if (pulsewidthrl == 0) {
    wheelSpeeds.rearLeft = 0;
  } else {
    wheelSpeeds.rearLeft = wheelDivisor / pulsewidthrl;
  }
}

void rearRightWheelSpeed(void) {
  pulsewidthrr = pulseIn(speedsensorrr, HIGH, wheelTimeout);
  if (pulsewidthrr == 0) {
    wheelSpeeds.rearRight = 0;
  } else {
    wheelSpeeds.rearRight = wheelDivisor / pulsewidthrr;
  } 
}
