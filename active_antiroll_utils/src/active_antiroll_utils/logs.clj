(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]
            [incanter.core :as icore]))

(def full false)

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))

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
  (def newTime (icore/to-dataset (icore/$map (fn [x] (/ x 1000)) :timems dataset)))
  (def newXAccel (icore/to-dataset (icore/$map (fn [x] (/ x 4096)) :xAccel dataset)))
  (def newYAccel (icore/to-dataset (icore/$map (fn [x] (/ x 4096)) :yAccel dataset)))
  (def newZAccel (if full (icore/to-dataset (icore/$map (fn [x] (/ x 4096)) :zAccel dataset))))
  (def newFrontRS (icore/to-dataset (icore/$map (fn [x] (/ x 1)) :frontRightSpeed dataset)))
  (def newFrontLS (icore/to-dataset (icore/$map (fn [x] (/ x 1)) :frontLeftSpeed dataset)))
  (def newRearRS (icore/to-dataset (icore/$map (fn [x] (/ x 1)) :rearRightSpeed dataset)))
  (def newRearLS (icore/to-dataset (icore/$map (fn [x] (/ x 1)) :rearLeftSpeed dataset)))
    (def data (icore/conj-cols dataset
                               newTime
                               newXAccel
                               newYAccel
                               (if full newZAccel)
                               newFrontRS
                               newFrontLS
                               newRearRS
                               newRearLS))

  (println data)
  (def newData (icore/col-names data names))
  newData)
