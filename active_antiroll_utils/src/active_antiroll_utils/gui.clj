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

;Utils
(defn exit[e] (System/exit 0))

(defn addTab [panel tab]
  (score/config! panel :tabs (conj (:tabs panel) tab)))

(defn display [frame content]
  (score/config! frame :content content)
  content)


;GraphTabPanel
(defn makePlotPanel[plotData]

  (def tabbedGraphs (score/tabbed-panel
                      :placement :top
                      :overflow :wrap))
  (def accel
    (icharts/xy-plot :x-label "Time (s)"
                     :y-label "Acceleration (g)"
                     :title "Active Antiroll Acceleration"
                     :legend true))
  (icharts/add-lines accel :time :y_accel
                     :data plotData
                     :series-label "y")
  (icharts/add-lines accel :time :x_accel
                     :data plotData
                     :series-label "x")

  (def gyro
    (icharts/xy-plot :x-label "Time (s)"
                     :y-label "Roll (Degrees)"
                     :title "Active Antiroll Roll"
                     :legend true))
  (icharts/add-lines gyro :time :z_gyro
                     :data plotData
                     :series-label "z")

  (def speed
    (icharts/xy-plot :x-label "Time (s)"
                     :y-label "Speed (stuffz)"
                     :title "Active Antiroll Wheel Speeds"
                     :legend true))
  (icharts/add-lines speed :time :fl_speed
                     :data plotData
                     :series-label "Front Left")
  (icharts/add-lines speed :time :fr_speed
                     :data plotData
                     :series-label "Front Right")
  (icharts/add-lines speed :time :rl_speed
                     :data plotData
                     :series-label "Rear Left")
  (icharts/add-lines speed :time :rr_speed
                     :data plotData
                     :series-label "Rear Right")

  ;(def plot (.getPlot accel))
  ;(.setTickUnit
    ;(.getDomainAxis plot)
    ;(org.jfree.chart.axis.NumberTickUnit. 100.0))
  ;(.setTickUnit
    ;(.getRangeAxis plot) 
    ;(org.jfree.chart.axis.NumberTickUnit. 1.0))

  (def accelPlot (ChartPanel. accel))
  (def gyroPlot (ChartPanel. gyro))
  (def speedPlot (ChartPanel. speed))

  (addTab tabbedGraphs {:title "Acceleration" :content accelPlot})
  (addTab tabbedGraphs {:title "Gyro" :content gyroPlot})
  (addTab tabbedGraphs {:title "Wheel Speed" :content speedPlot})

  tabbedGraphs)


;Dialogs
(defn openLogDialog[]
  (def logFile
    (schooser/choose-file :type :open
                          :selection-mode :files-only
                          :filters [["CSV" ["csv"]]]
                          :success-fn (fn [fc file] (.getAbsolutePath file))))
  (readCSV logFile))


;Components...
(def logButton (score/button :text "Choose log file"))

(def introArea (score/flow-panel
                   :align :center
                   :hgap 20
                   :items [logButton]))


;Containers...
(defn graphSidebarSplit[]
  (def plotArea (makePlotPanel (openLogDialog)))
  (def sidebarArea (score/flow-panel
                     :align :left
                     :hgap 20
                     :items [logButton]))

  (def split (score/left-right-split
               (score/scrollable sidebarArea)
               (score/scrollable plotArea)
               :divider-location 2/5))
  split)


;Menu items...
(def openItem (score/action :name "Open log" :tip "Open a .csv log file"
                            :mnemonic \o :key (skeystroke/keystroke "O")))

(def exitItem (score/action :name "Exit" :tip "Exit the application"
                      :mnemonic \q :key (skeystroke/keystroke "menu Q")
                      :handler exit))

(def fileMenu (score/menu :text "File"
                    :mnemonic \f
                    :items [openItem exitItem]))
;Menubar
(def menuBar (score/menubar :items [fileMenu]))


(defn guiInit[]
  ;GUI Initialization stuff, make it work on all platforms and look nice
  (score/native!)

;  (javax.swing.UIManager/setLookAndFeel "org.pushingpixels.substance.api.skin.SubstanceTwilightLookAndFeel")

  ;Main frame which is used for the application
  (def mainFrame
    (score/frame 
      :title "Active Antiroll Utils"))

  ;component actions
  (defn loadPanelAction[e]
    (display mainFrame (graphSidebarSplit)))

  ;Event bindings...
  (score/listen logButton :action loadPanelAction)
  (score/config! openItem :handler loadPanelAction)

  ;components
  (score/config! mainFrame :menubar menuBar)

  (display mainFrame introArea)

  (-> mainFrame score/pack! score/show!))
