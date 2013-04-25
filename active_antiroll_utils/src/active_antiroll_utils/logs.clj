(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]
            [incanter.core :as icore]))

(def full false)

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))

;There has got to a better way of doing this...
(def names (if full [:timems
                     :xAccel
                     :yAccel
                     :zAccel
                     :xGyro
                     :yGyro
                     :zGyro
                     :frontRightSpeed
                     :frontLeftSpeed
                     :rearRightSpeed
                     :rearLeftSpeed
                     :frontRightServo
                     :frontLeftServo
                     :rearServo
                     :setting
                     :message
                     :times
                     :xAccelCorrected
                     :yAccelCorrected
                     :zAccelCorrected
                     :xGyroCorrected
                     :yGyroCorrected
                     :zGyroCorrected
                     :frontRightSpeedCorrected
                     :frontLeftSpeedCorrected
                     :rearRightSpeedCorrected
                     :rearLeftSpeedCorrected]
                    [:timems
                     :xAccel
                     :yAccel
                     :zGyro
                     :frontRightSpeed
                     :frontLeftSpeed
                     :rearRightSpeed
                     :rearLeftSpeed
                     :frontRightServo
                     :frontLeftServo
                     :rearServo
                     :setting
                     :times
                     :xAccelCorrected
                     :yAccelCorrected
                     :zGyroCorrected
                     :frontRightSpeedCorrected
                     :frontLeftSpeedCorrected
                     :rearRightSpeedCorrected
                     :rearLeftSpeedCorrected]))

(defn processData[dataset accelDiv wheelDiv]
  (let [newTime (icore/$map (fn [x] (/ x 1000)) :timems dataset)
        newXAccel (icore/$map (fn [x] (/ x accelDiv)) :xAccel dataset)
        newYAccel (icore/$map (fn [x] (/ x accelDiv)) :yAccel dataset)
        newZAccel (if full (icore/$map (fn [x] (/ x accelDiv)) :zAccel dataset))
        newXGyro (if full (icore/$map (fn [x] (/ x accelDiv)) :xGyro dataset))
        newYGyro (if full (icore/$map (fn [x] (/ x accelDiv)) :yGyro dataset))
        newZGyro (icore/$map (fn [x] (/ x accelDiv)) :zGyro dataset)
        newFrontRS (icore/$map (fn [x] (/ x wheelDiv)) :frontRightSpeed dataset)
        newFrontLS (icore/$map (fn [x] (/ x wheelDiv)) :frontLeftSpeed dataset)
        newRearRS (icore/$map (fn [x] (/ x wheelDiv)) :rearRightSpeed dataset)
        newRearLS (icore/$map (fn [x] (/ x wheelDiv)) :rearLeftSpeed dataset)]
    ;Theres got to be a better way for this too.
    (if full
      (def data (icore/conj-cols dataset
                               newTime
                               newXAccel
                               newYAccel
                               newZAccel
                               newXGyro
                               newYGyro
                               newZGyro
                               newFrontRS
                               newFrontLS
                               newRearRS
                               newRearLS))
      (def data (icore/conj-cols dataset
                               newTime
                               newXAccel
                               newYAccel
                               newZGyro
                               newFrontRS
                               newFrontLS
                               newRearRS
                               newRearLS))))

  (icore/col-names data names))
