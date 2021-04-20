### spdlog4ts is a spdlog binding for Node.js whether using TypeScript or JavaScript

* ### install
    c++ environment is required
```
    npm install spdlog4ts
```
* ### example
```
const {
    RawLogger,
    StdoutLogger,
    AsyncLogger,
    RotatingLogger,
    DailyLogger,
    HourlyLogger,
    LogLevel
} = require('./main.js');

// use the default logger (stdout, multi-threaded, colored)
RawLogger.level = LogLevel.TRACE;
RawLogger.critical("critical log");
RawLogger.error("error log");
RawLogger.warn("warning log");
RawLogger.info("info log");
RawLogger.debug("debug log");
RawLogger.trace("trace log");

// force all loggers flush to disk every 3 seconds
RawLogger.flushEvery(3);

// create a stdout logger 
const stdlogger = new StdoutLogger('stdout');

// set log pattern
stdlogger.pattern = ">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<";

// set log level
stdlogger.level = LogLevel.TRACE;
stdlogger.trace("stdout logger message");

// create an async logger 
const asynclogger = new AsyncLogger('asynclogger', "./logs/async.log");
asynclogger.info("async logger message");

// create a rotating file logger 
const rotatinglogger = new RotatingLogger('rotatinglogger', "./logs/rotating.log");
rotatinglogger.info("rotating logger message");

// create a daily file logger
const dailylogger = new DailyLogger('dailylogger', './logs/daily.log');
dailylogger.info("daily logger message");

// create an hourly file logger
const hourlylogger = new HourlyLogger('hourlyLogger', './logs/hourly.log');
hourlylogger.info("hourly logger message")

```