(ns active-antiroll-utils.logs
  (:gen-class)
  (:require [incanter.io :as iio]))

(defn readCSV[datafileName]
  (iio/read-dataset datafileName :header true))
