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


;Util functions
(defn exit[e] (System/exit 0))

(defn addTab [panel tab]
  (score/config! panel :tabs (conj (:tabs panel) tab)))

(defn display [frame content]
  (score/config! frame :content content)
  content)


;GraphTabPanel
(defn makePlotPanel[plotData]
  (def tabbedGraphsPanel (score/tabbed-panel :placement :top
                                             :overflow :wrap))

  (def accelPlot (icharts/xy-plot :x-label "Time (s)"
                                  :y-label "Acceleration (g)"
                                  :title "Active Antiroll Acceleration"
                                  :legend true
                                  :times :xAccelCorrected
                                  :data plotData
                                  :series-label "x"))

  (icharts/add-lines accelPlot    :times :yAccelCorrected
                                  :data plotData
                                  :series-label "y")

  (def gyroPlot (icharts/xy-plot  :x-label "Time (s)"
                                  :y-label "Roll (Degrees)"
                                  :title "Active Antiroll Roll"
                                  :legend true
                                  :times :zGyro
                                  :data plotData
                                  :series-label "z"))

  (def speedPlot (icharts/xy-plot :x-label "Time (s)"
                                  :y-label "Speed (rad/sec)"
                                  :title "Active Antiroll Wheel Speeds"
                                  :legend true
                                  :times :frontLeftSpeedCorrected
                                  :data plotData
                                  :series-label "Front Left"))

  (icharts/add-lines speedPlot    :times :frontRightSpeedCorrected
                                  :data plotData
                                  :series-label "Front Right")
  (icharts/add-lines speedPlot    :times :rearLeftSpeedCorrected
                                  :data plotData
                                  :series-label "Rear Left")
  (icharts/add-lines speedPlot    :times :rearRightSpeedCorrected
                                  :data plotData
                                  :series-label "Rear Right")

  ;(let [plot (.getPlot accelPlot)]
    ;(fn []
      ;(.setTickUnit
        ;(.getDomainAxis plot)
        ;(org.jfree.chart.axis.NumberTickUnit. 10.0))
      ;(.setTickUnit
        ;(.getRangeAxis plot)
        ;(org.jfree.chart.axis.NumberTickUnit. 1.0))))

  (let [accelPlotPanel (ChartPanel. accelPlot)
        gyroPlotPanel (ChartPanel. gyroPlot)
        speedPlotPanel (ChartPanel. speedPlot)]
    (fn [x] 
      (addTab tabbedGraphsPanel {:title "Acceleration"
                                 :content accelPlotPanel})
      (addTab tabbedGraphsPanel {:title "Gyro"
                                 :content gyroPlotPanel})
      (addTab tabbedGraphsPanel {:title "Wheel Speed"
                                 :content speedPlotPanel})))

  tabbedGraphsPanel)


;Dialogs
(defn openLogDialog[]
  (let [logFile (schooser/choose-file :type :open
                                     :selection-mode :files-only
                                     :filters [["CSV" ["csv"]]]
                                     :success-fn (fn [fc file]
                                                   (.getAbsolutePath file)))]
    (readCSV logFile)))


;Components...
(def logButton (score/button :text "Choose log file"))

(def introArea (score/flow-panel :align :center
                                 :hgap 20
                                 :items [logButton]))

(def sidebarArea (score/flow-panel :align :left
                                   :hgap 20
                                   :items [logButton]))


;Containers...
(defn graphSidebarSplit[]
  (def plotArea (makePlotPanel (processData (openLogDialog))))

  (def split (score/left-right-split
               (score/scrollable sidebarArea)
               (score/scrollable plotArea)
               :divider-location 3/10))
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


;Main frame which is used for the application
(def mainFrame
  (score/frame :title "Active Antiroll Utils"))


;GUI Init code
(defn guiInit[]
  ;GUI Initialization stuff, make it work on all platforms and look nice
  (score/native!)

  ;component actions
  (defn loadPanelAction[e]
    (display mainFrame (graphSidebarSplit)))

  ;Event bindings...
  (score/listen logButton :action loadPanelAction)
  (score/config! openItem :handler loadPanelAction)

  ;components
  (score/config! mainFrame :menubar menuBar)

  (display mainFrame introArea)

  ;set the style of the gui to something a little better than default java
  ;swing uglyness
  (javax.swing.UIManager/setLookAndFeel
    "org.pushingpixels.substance.api.skin.SubstanceTwilightLookAndFeel")

  (-> mainFrame score/pack! score/show!))
