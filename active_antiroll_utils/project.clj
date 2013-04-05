(defproject active_antiroll_utils "0.1.0-SNAPSHOT"
  :description "Basic utils for working with the logs returned from the
               active antiroll on the 2013 CSU FSAE racecar"
  :url "https://github.com/JoshAshby/Racecars"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.5.1"]
                 [incanter "1.4.1"]
                 [seesaw "1.4.3"]
                 [org.jfree/jfreechart "1.0.14"]]
  :plugins [[lein-bin "0.3.2"]]
  :main active-antiroll-utils.core
  :aot [active-antiroll-utils.core])
