(ns active-antiroll-utils.gui
  (:gen-class)
  (:use active-antiroll-utils.logs)
  (:require [seesaw.core :as score]
            [seesaw.keystroke :as skeystroke]
            [seesaw.chooser :as schooser]
            [incanter.core :as icore]
            [incanter.charts :as icharts])
  (:import [org.jfree.chart ChartPanel]
           org.pushingpixels.substance.api.SubstanceLookAndFeel))

(defn exit[e] (System/exit 0))

(defn addTab [panel tab]
  (score/config! panel :tabs (conj (:tabs panel) tab)))

(defn display [frame content]
  (score/config! frame :content content)
  content)

(defn makePlotPanel[plotData]

  (def tabbedGraphs (score/tabbed-panel
                      :placement :top
                      :overflow :wrap))
  (def accel
    (icharts/xy-plot :time :y_accel
                     :x-label "Time (s)"
                     :y-label "Acceleration (g)"
                     :title "Active Antiroll Acceleration"
                     :legend true
                     :series-label "y"
                     :data plotData))
  (icharts/add-lines accel :time :x_accel
                     :data plotData
                     :series-label "x")
  (def gyro
    (icharts/xy-plot :time :z_gyro
                     :x-label "Time (s)"
                     :y-label "Roll? (Degrees?)"
                     :title "Active Antiroll Roll"
                     :legend true
                     :series-label "z"
                     :data plotData))
  (def speed
    (icharts/xy-plot :time :fl_speed
                     :x-label "Time (s)"
                     :y-label "Speed (stuffz)"
                     :title "Active Antiroll Wheel Speeds"
                     :legend true
                     :series-label "Front Left"
                     :data plotData))
  (icharts/add-lines speed :time :fr_speed
                     :data plotData
                     :series-label "Front Right")
  (icharts/add-lines speed :time :rl_speed
                     :data plotData
                     :series-label "Rear Left")
  (icharts/add-lines speed :time :rr_speed
                     :data plotData
                     :series-label "Rear Right")
  (def accelPlot (ChartPanel. accel))
  (def gyroPlot (ChartPanel. gyro))
  (def speedPlot (ChartPanel. speed))

  (addTab tabbedGraphs {:title "Acceleration" :content accelPlot})
  (addTab tabbedGraphs {:title "Gyro" :content gyroPlot})
  (addTab tabbedGraphs {:title "Wheel Speed" :content speedPlot})

  tabbedGraphs)

(defn openLogDialog[]
  (def logFile
    (schooser/choose-file :type :open
                          :selection-mode :files-only
                          :filters [["CSV" ["csv"]]]
                          :success-fn (fn [fc file] (.getAbsolutePath file))))
  (readCSV logFile))

(defn guiInit[]
  (score/native!)

  (javax.swing.UIManager/setLookAndFeel "org.pushingpixels.substance.api.skin.SubstanceTwilightLookAndFeel")

  (def mainFrame
    (score/frame 
      :title "Active Antiroll Utils"))


  ;Components...
  (def logButton (score/button :text "Choose log file"))

  (def introArea (score/flow-panel
                     :align :center
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

    (display mainFrame split))

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

  (display mainFrame introArea)

  (-> mainFrame score/pack! score/show!))
