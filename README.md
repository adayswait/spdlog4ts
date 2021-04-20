### spdlog4ts is a spdlog binding for Node.js whether using TypeScript or JavaScript

* ### install
    c++ environment is required
```
    npm install spdlog4ts
```
* ### example
```
const {
    BasicLogger,
    StdoutLogger,
    AsyncLogger,
    RotatingLogger,
    DailyLogger,
    HourlyLogger,
    EType,
    ELevel
} = require('./index.js');

// use the default logger (stdout, multi-threaded, colored)
BasicLogger.level = ELevel.TRACE;
BasicLogger.critical("critical log");
BasicLogger.error("error log");
BasicLogger.warn("warning log");
BasicLogger.info("info log");
BasicLogger.pattern = "*** [%H:%M:%S %z] [thread %t] %v ***";
BasicLogger.debug("debug log");
BasicLogger.trace("trace log");

BasicLogger.info(EType.DAILY, EType.HOURLY);

// force all loggers flush to disk every 3 seconds
BasicLogger.flushEvery(3);

// create a stdout logger 
const stdlogger = new StdoutLogger('stdout');

// set log pattern
stdlogger.pattern = ">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<";

// set log level
stdlogger.level = ELevel.TRACE;
stdlogger.trace("stdout logger message");

// create an async logger 
const asynclogger = new AsyncLogger('asynclogger', "./logs/async.log");
asynclogger.info("async logger message");

// create a rotating file logger 
const rotatinglogger = new RotatingLogger('rotatinglogger',
    "./logs/rotating.log");
rotatinglogger.info("rotating logger message");

// create a daily file logger
const dailylogger = new DailyLogger('dailylogger', './logs/daily.log');
dailylogger.info("daily logger message");

// create an hourly file logger
const hourlylogger = new HourlyLogger('hourlyLogger', './logs/hourly.log');
hourlylogger.info("hourly logger message");

```