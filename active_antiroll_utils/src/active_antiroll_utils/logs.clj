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
                        :times
                        :xAccelCorrected
                        :yAccelCorrected
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
                        :times
                        :xAccelCorrected
                        :yAccelCorrected
                        :frontRightSpeedCorrected
                        :frontLeftSpeedCorrected
                        :rearRightSpeedCorrected
                        :rearLeftSpeedCorrected]))

(defn processData[dataset]
  (let [newTime (icore/$map (fn [x] (/ x 1000)) :timems dataset)
        newXAccel (icore/$map (fn [x] (/ x 4096)) :xAccel dataset)
        newYAccel (icore/$map (fn [x] (/ x 4096)) :yAccel dataset)
        newZAccel (if full (icore/$map (fn [x] (/ x 4096)) :zAccel dataset))
        newFrontRS (icore/$map (fn [x] (/ x 1)) :frontRightSpeed dataset)
        newFrontLS (icore/$map (fn [x] (/ x 1)) :frontLeftSpeed dataset)
        newRearRS (icore/$map (fn [x] (/ x 1)) :rearRightSpeed dataset)
        newRearLS (icore/$map (fn [x] (/ x 1)) :rearLeftSpeed dataset)]
    ;Theres got to be a better way for this too.
    (if full
      (def data (icore/conj-cols dataset
                               newTime
                               newXAccel
                               newYAccel
                               newZAccel
                               newFrontRS
                               newFrontLS
                               newRearRS
                               newRearLS))
      (def data (icore/conj-cols dataset
                               newTime
                               newXAccel
                               newYAccel
                               newFrontRS
                               newFrontLS
                               newRearRS
                               newRearLS))))

  (icore/col-names data names))
