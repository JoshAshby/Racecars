(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]
            [incanter.core :as icore]))

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))

(defn processData[dataset]
  (let [names [:time
               :xAccel
               :yAccel
;               :zAccel
;               :xGyro
;               :yGyro
               :zGyro
               :frontRightSpeed
               :frontLeftSpeed
               :rearRightSpeed
               :rearLeftSpeed]
        leftOutCols [:time
                     :xaccel
                     :yaccel]
 ;                    :zaccel]
        newTime (icore/$map (fn [x] (/ x 10000)) :time dataset)
        newXAccel (icore/$map (fn [x] (/ x 4096)) :xaccel dataset)
        newYAccel (icore/$map (fn [x] (/ x 4096)) :yaccel dataset)
 ;       newZAccel (if (icore/sel dataset :cols :z_accel :rows 0)
 ;                   (icore/$map (fn [x] (/ x 4096)) :z_accel dataset))
        data (icore/conj-cols newTime
                              newXAccel
                              newYAccel
;                              newZAccel
                              (icore/sel dataset
                                         :except leftOutCols))]
    (icore/col-names data names)))
