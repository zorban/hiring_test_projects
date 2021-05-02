const { createLogger, format, transports } = require('winston');
const { combine, timestamp, printf } = format;

const myFormat = printf(({ timestamp, level, message }) => {
  return `${timestamp} ${level}: ${message}`;
});

const logger = createLogger({
  level: 'info',
  // format: format.splat(),
  // defaultMeta: { service: 'user-service' },
  transports: [
    new transports.File({ filename: 'logs/error.log', level: 'error' }),
    new transports.File({ filename: 'logs/combined.log' }),
    new transports.Console({
      format: combine(
        // timestamp({ format: 'YYYY-MM-DD HH:mm:ss' }),
        myFormat,
        format.colorize(),
        format.splat(),
        format.simple()
      )
    })
  ]
});


module.exports = logger;
