declare module "spdlog4ts" {
    export enum ELevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL,
        OFF
    }

    export enum EType {
        STDOUT,
        ASYNC,
        ROTATING,
        DAILY,
        HOURLY
    }

    export class RawLogger {
        constructor(logName: string, LogType: EType, LogPath: string);
        static get level(): ELevel;
        static set level(LogLevel: ELevel);
        static set pattern(logPattern: string);
        static ELevel: ELevel;
        static EType: EType;
        static critical(...messages: any): void;
        static error(...messages: any): void;
        static warn(...messages: any): void;
        static info(...messages: any): void;
        static debug(...messages: any): void;
        static trace(...messages: any): void;
        static flushEvery(seconds: number): void;
        get level(): ELevel;
        set level(LogLevel: ELevel);
        set pattern(logPattern: string);
        protected __critical(...messages: any): void;
        protected __error(...messages: any): void;
        protected __warn(...messages: any): void;
        protected __info(...messages: any): void;
        protected __debug(...messages: any): void;
        protected __trace(...messages: any): void;
        flush(): void;
        flushOn(level: ELevel): void;
    }

    export class BasicLogger extends RawLogger {
        constructor(logName: string, LogType: EType, LogPath: string);
        critical(...messages: any): void;
        error(...messages: any): void;
        warn(...messages: any): void;
        info(...messages: any): void;
        debug(...messages: any): void;
        trace(...messages: any): void;
        flush(): void;
        flushOn(logLevel: ELevel): void;
    }

    export class StdoutLogger extends BasicLogger {
        constructor(logName: string);
    }

    export class AsyncLogger extends BasicLogger {
        constructor(logName: string, logPath: string);
    }

    export class RotatingLogger extends BasicLogger {
        constructor(logName: string, logPath: string);
    }

    export class DailyLogger extends BasicLogger {
        constructor(logName: string, logPath: string);
    }

    export class HourlyLogger extends BasicLogger {
        constructor(logName: string, logPath: string);
    }
}
