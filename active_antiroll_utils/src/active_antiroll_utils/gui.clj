(ns active-antiroll-utils.gui
  (:gen-class)
  (:use active-antiroll-utils.logs)
  (:require [seesaw.core :as score]
            [seesaw.keystroke :as skeystroke]
            [seesaw.chooser :as schooser]
            [incanter.core :as icore]
            [incanter.charts :as icharts])
  (:import [org.jfree.chart ChartPanel StandardChartTheme]
           org.pushingpixels.substance.api.SubstanceLookAndFeel
           [java.awt Color]))


;Make everything pretty
(score/native!)
(javax.swing.UIManager/setLookAndFeel
  "org.pushingpixels.substance.api.skin.SubstanceTwilightLookAndFeel")


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

  (def accelPlot (icharts/scatter-plot   :times :xAccelCorrected
                                         :title "Active Antiroll Acceleration"
                                         :y-label "Acceleration (g)"
                                         :x-label "Time (s)"
                                         :legend true
                                         :data plotData
                                         :series-label "x"))

  (icharts/add-points accelPlot          :times :yAccelCorrected
                                         :data plotData
                                         :series-label "y")

  (if full (icharts/add-points accelPlot :times :zAccelCorrected
                                         :data plotData
                                         :series-label "z"))

  (def gyroPlot (icharts/scatter-plot    :times :zGyroCorrected
                                         :title "Active Antiroll Roll"
                                         :y-label "Roll (Degrees)"
                                         :x-label "Time (s)"
                                         :legend true
                                         :data plotData
                                         :series-label "z"))

  (if full (icharts/add-points gyroPlot  :times :xGyroCorrected
                                         :data plotData
                                         :series-label "x"))

  (if full (icharts/add-points gyroPlot  :times :yGyroCorrected
                                         :data plotData
                                         :series-label "y"))

  (def speedPlot (icharts/scatter-plot   :times :frontLeftSpeedCorrected
                                         :title "Active Antiroll Wheel Speeds"
                                         :y-label "Speed (rad/sec)"
                                         :x-label "Time (s)"
                                         :legend true
                                         :data plotData
                                         :series-label "Front Left"))

  (icharts/add-points speedPlot          :times :frontRightSpeedCorrected
                                         :data plotData
                                         :series-label "Front Right")
  (icharts/add-points speedPlot          :times :rearLeftSpeedCorrected
                                         :data plotData
                                         :series-label "Rear Left")
  (icharts/add-points speedPlot          :times :rearRightSpeedCorrected
                                         :data plotData
                                         :series-label "Rear Right")

  (def servoPlot (icharts/scatter-plot   :times :frontLeftServo
                                         :title "Active Antiroll Servo Position"
                                         :y-label "Servo Position degrees"
                                         :x-label "Time (s)"
                                         :legend true
                                         :data plotData
                                         :series-label "Front Left"))

  (icharts/add-points servoPlot          :times :frontRightServo
                                         :data plotData
                                         :series-label "Front Right")
  (icharts/add-points servoPlot          :times :rearServo
                                         :data plotData
                                         :series-label "Rear")


  ;(let [plot (.getPlot accelPlot)]
    ;(fn []
      ;(.setTickUnit
        ;(.getDomainAxis plot)
        ;(org.jfree.chart.axis.NumberTickUnit. 10.0))
      ;(.setTickUnit
        ;(.getRangeAxis plot)
        ;(org.jfree.chart.axis.NumberTickUnit. 1.0))))

  (def custom-dark-theme
    (doto
      (StandardChartTheme/createJFreeTheme)
      (.setPlotBackgroundPaint Color/DARK_GRAY)
      (.setChartBackgroundPaint Color/DARK_GRAY)
      (.setLegendBackgroundPaint Color/DARK_GRAY)
      (.setAxisLabelPaint Color/WHITE)
      (.setTickLabelPaint Color/WHITE)
      (.setLegendItemPaint Color/WHITE)))

  (icharts/set-theme accelPlot custom-dark-theme)
  (icharts/set-theme gyroPlot custom-dark-theme)
  (icharts/set-theme speedPlot custom-dark-theme)
  (icharts/set-theme servoPlot custom-dark-theme)

  (addTab tabbedGraphsPanel {:title "Acceleration"
                             :content (ChartPanel. accelPlot)})
  (addTab tabbedGraphsPanel {:title "Gyro"
                             :content (ChartPanel. gyroPlot)})
  (addTab tabbedGraphsPanel {:title "Wheel Speed"
                             :content (ChartPanel. speedPlot)})
  (addTab tabbedGraphsPanel {:title "Servo Position"
                             :content (ChartPanel. servoPlot)})

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


;Containers...
(defn graphSidebarSplit[]
  (def plotArea (makePlotPanel (processData (openLogDialog) 30000 1)))

  (def sidebarArea (score/flow-panel :align :left
                                     :hgap 20
                                     :items [logButton]))

  (def split (score/left-right-split
               (score/scrollable sidebarArea)
               (score/scrollable plotArea)
               :divider-location 2/10))
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
(def mainFrame (score/frame :title "Active Antiroll Utils"))


;GUI Init code
(defn guiInit[]
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
