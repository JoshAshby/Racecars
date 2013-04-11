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
  (let [tabbedGraphs (score/tabbed-panel
                       :placement :top
                       :overflow :wrap)
        accel (icharts/xy-plot :x-label "Time (s)"
                               :y-label "Acceleration (g)"
                               :title "Active Antiroll Acceleration"
                               :legend true)
        gyro (icharts/xy-plot  :x-label "Time (s)"
                               :y-label "Roll (Degrees)"
                               :title "Active Antiroll Roll"
                               :legend true)
        speed (icharts/xy-plot :x-label "Time (s)"
                               :y-label "Speed (rad/sec)"
                               :title "Active Antiroll Wheel Speeds"
                               :legend true)]

    (fn [] (icharts/add-lines accel :time :yAccel
                         :data plotData
                         :series-label "y")
            (icharts/add-lines accel :time :xAccel
                               :data plotData
                               :series-label "x")
            (icharts/add-lines gyro :time :zGyro
                               :data plotData
                               :series-label "z")
            (icharts/add-lines speed :time :frontLeftSpeed
                               :data plotData
                               :series-label "Front Left")
            (icharts/add-lines speed :time :frontRightSpeed
                               :data plotData
                               :series-label "Front Right")
            (icharts/add-lines speed :time :rearLeftSpeed
                               :data plotData
                               :series-label "Rear Left")
            (icharts/add-lines speed :time :rearRightpeed
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

            tabbedGraphs)))


;Dialogs
(defn openLogDialog[]
  (let [logFile (schooser/choose-file :type :open
                                      :selection-mode :files-only
                                      :filters [["CSV" ["csv"]]]
                                      :success-fn (fn [fc file]
                                                    (.getAbsolutePath file)))]
  (processData (readCSV logFile))))


;Components...
(def logButton (score/button :text "Choose log file"))

(def introArea (score/flow-panel
                   :align :center
                   :hgap 20
                   :items [logButton]))


;Containers...
(defn graphSidebarSplit[]
  (let [plotArea (makePlotPanel (openLogDialog))
        sidebarArea (score/flow-panel
                      :align :left
                      :hgap 20
                      :items [logButton])

        split (score/left-right-split 
                (score/scrollable sidebarArea)
                (score/scrollable plotArea)
                :divider-location 2/5)]
    split))


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
