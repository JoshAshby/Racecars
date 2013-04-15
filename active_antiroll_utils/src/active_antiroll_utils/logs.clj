(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]
            [incanter.core :as icore]))

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))

(defn processData[dataset]
  (let [names [:timems
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
               :rearLeftSpeedCorrected]
        newTime (icore/$map (fn [x] (/ x 10000)) :timems dataset)
        newXAccel (icore/$map (fn [x] (/ x 4096)) :xAccel dataset)
        newYAccel (icore/$map (fn [x] (/ x 4096)) :yAccel dataset)
        newFrontRS (icore/$map (fn [x] (/ x 1)) :frontRightSpeed dataset)
        newFrontLS (icore/$map (fn [x] (/ x 1)) :frontLeftSpeed dataset)
        newRearRS (icore/$map (fn [x] (/ x 1)) :rearRightSpeed dataset)
        newRearLS (icore/$map (fn [x] (/ x 1)) :rearLeftSpeed dataset)
        data (icore/conj-cols dataset
                              newTime
                              newXAccel
                              newYAccel
                              newFrontRS
                              newFrontLS
                              newRearRS
                              newRearLS)]
    (icore/col-names data names)))
