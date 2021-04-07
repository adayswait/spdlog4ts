const { Logger } = require('bindings')('spdlog4ts');
Logger.critical('critical');
Logger.error('error');
Logger.warn('warning');
Logger.info('info');
Logger.debug('debug');
Logger.trace('trace');

let logger = new Logger(Logger.TYPE.DAILY, './logs/test222.log')
logger.info("testing")