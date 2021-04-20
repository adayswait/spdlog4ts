const { Logger } = require('bindings')('spdlog4ts');
class BasicLogger extends Logger {
    constructor(name, type, path) {
        super(name, type, path);
    }
    critical(...messages) {
        if (messages.length === 0) return;
        this.__critical(messages.join(' '));
    }
    error(...messages) {
        if (messages.length === 0) return;
        this.__error(messages.join(' '));
    }
    warn(...messages) {
        if (messages.length === 0) return;
        this.__warn(messages.join(' '));
    }
    info(...messages) {
        if (messages.length === 0) return;
        this.__info(messages.join(' '));
    }
    debug(...messages) {
        if (messages.length === 0) return;
        this.__debug(messages.join(' '));
    }
    trace(...messages) {
        if (messages.length === 0) return;
        this.__trace(messages.join(' '));
    }
    static critical(...messages) {
        if (messages.length === 0) return;
        Logger.critical(messages.join(' '));
    }
    static error(...messages) {
        if (messages.length === 0) return;
        Logger.error(messages.join(' '));
    }
    static warn(...messages) {
        if (messages.length === 0) return;
        Logger.warn(messages.join(' '));
    }
    static info(...messages) {
        if (messages.length === 0) return;
        Logger.info(messages.join(' '));
    }
    static debug(...messages) {
        if (messages.length === 0) return;
        Logger.debug(messages.join(' '));
    }
    static trace(...messages) {
        if (messages.length === 0) return;
        Logger.trace(messages.join(' '));
    }
};

class StdoutLogger extends BasicLogger {
    constructor(name) {
        super(name || `log${StdoutLogger._logID++}`, Logger.EType.STDOUT, "");
    }
    static _logID = 0;
};

class AsyncLogger extends BasicLogger {
    constructor(name, path) {
        super(name, Logger.EType.ASYNC, path);
    }
};

class RotatingLogger extends BasicLogger {
    constructor(name, path) {
        super(name, Logger.EType.ROTATING, path);
    }
};

class DailyLogger extends BasicLogger {
    constructor(name, path) {
        super(name, Logger.EType.DAILY, path);
    }
};

class HourlyLogger extends BasicLogger {
    constructor(name, path) {
        super(name, Logger.EType.HOURLY, path);
    }
};

exports.ELevel = Logger.ELevel;
exports.EType = Logger.EType;
exports.RawLogger = Logger;
exports.BasicLogger = BasicLogger;
exports.StdoutLogger = StdoutLogger;
exports.AsyncLogger = AsyncLogger;
exports.RotatingLogger = RotatingLogger;
exports.DailyLogger = DailyLogger;
exports.HourlyLogger = HourlyLogger;
