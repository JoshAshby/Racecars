(ns active-antiroll-utils.gui
  (:gen-class)
  (:use active-antiroll-utils.logs)
  (:require [seesaw.core :as score]
            [seesaw.keystroke :as skeystroke]
            [seesaw.chooser :as schooser]
            [incanter.core :as icore]
            [incanter.charts :as icharts])
  (:import [org.jfree.chart ChartPanel]))

(defn exit[e] (System/exit 0))

(defn makePlotPanel[plotData]
  (def accelPlot 
    (icharts/xy-plot :time :y_accel
                     :x-label "Time (s)"
                     :y-label "G's"
                     :title "Active Antiroll Data"
                     :legend true
                     :series-label "y"
                     :data plotData))
  (icharts/add-lines accelPlot :time :x_accel
                     :data plotData
                     :series-label "x")
  (def accelPlot (ChartPanel. accelPlot))
  accelPlot)

(defn openLogDialog[]
  (def logFile
    (schooser/choose-file :type :open
                          :selection-mode :files-only
                          :filters [["CSV" ["csv"]]]
                          :success-fn (fn [fc file] (.getAbsolutePath file))))
  (readCSV logFile))

(defn guiInit[]
  (score/native!)

  (def mainFrame
    (score/frame 
      :title "Active Antiroll Utils"))

  (defn display [content]
    (score/config! mainFrame :content content)
    content)

  ;Components...
  (def logButton (score/button :text "Choose log file"))

  (def introArea (score/flow-panel
                     :align :left
                     :hgap 20
                     :items [logButton]))

  (defn reloadPanel[e]
    (def plotArea (makePlotPanel (openLogDialog)))
    (def sidebarArea (score/flow-panel
                       :align :left
                       :hgap 20
                       :items [logButton]))

    (def split (score/left-right-split
                 (score/scrollable sidebarArea)
                 (score/scrollable plotArea)
                 :divider-location 1/4))

    (display split))

  ;Event bindings...
  (score/listen logButton :action reloadPanel)

  ;Menu items...
  (def openItem (score/action :name "Open log" :tip "Open a .csv log file"
                              :mnemonic \o :key (skeystroke/keystroke "O")
                              :handler reloadPanel))

  (def exitItem (score/action :name "Exit" :tip "Exit the application"
                        :mnemonic \q :key (skeystroke/keystroke "menu Q")
                        :handler exit))

  (def fileMenu (score/menu :text "File"
                      :mnemonic \f
                      :items [openItem exitItem]))

  (def topMenubar (score/menubar :items [fileMenu]))

  (score/config! mainFrame :menubar topMenubar)

  (display introArea)

  (-> mainFrame score/pack! score/show!))
