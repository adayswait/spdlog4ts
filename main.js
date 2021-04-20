const { Logger } = require('bindings')('spdlog4ts');
Logger.critical('critical');
Logger.error('error');
Logger.warn('warning');
Logger.info('info');
Logger.debug('debug');
Logger.trace('trace');

let logger = new Logger("match",Logger.EType.HOURLY, './logs/test222.log')
logger.info("test1")
logger.pattern=">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<"
logger.info("test2")
Logger.info(`level:${logger.level}`)