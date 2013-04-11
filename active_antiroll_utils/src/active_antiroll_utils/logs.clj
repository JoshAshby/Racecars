(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]
            [incanter.core :as icore]))

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))

(defn processData[dataset]
  (let [names [:timems
               :xaccel
               :yaccel
               :zaccel
               :xgyro
               :ygyro
               :zgyro
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
               :zAccelCorrected
               :frontRightSpeedCorrected
               :frontLeftSpeedCorrected
               :rearRightSpeedCorrected
               :rearLeftSpeedCorrected]
        newTime (icore/$map (fn [x] (/ x 10000)) :time dataset)
        newXAccel (icore/$map (fn [x] (/ x 4096)) :xaccel dataset)
        newYAccel (icore/$map (fn [x] (/ x 4096)) :yaccel dataset)
        newZAccel (icore/$map (fn [x] (/ x 4096)) :zaccel dataset)
        newFrontRS (icore/$map (fn [x] (/ x 1)) :frontRightSpeed dataset)
        newFrontLS (icore/$map (fn [x] (/ x 1)) :frontLeftSpeed dataset)
        newRearRS (icore/$map (fn [x] (/ x 1)) :rearRightSpeed dataset)
        newRearLS (icore/$map (fn [x] (/ x 1)) :rearLeftSpeed dataset)
        data (icore/conj-cols (icore/sel dataset)
                              newTime
                              newXAccel
                              newYAccel
                              newZAccel
                              newFrontRS
                              newFrontLS
                              newRearRS
                              newRearLS)]
    (icore/col-names data names)))
