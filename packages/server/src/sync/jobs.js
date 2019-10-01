const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

// m h d month weekDay
const EVERY_HOUR = '0 * * * *';
const EVERY_TEN_MINUTES = '*/10 * * * *';
const EVERY_MINUTE = '* * * * *';

function runJobs() {
  console.log('Jobs are in a queue.');

  const jobs = [];

  const a = cron.scheduleJob(EVERY_HOUR, () => {
    emitter.emit(events.EVERY_HOUR);
  });

  const b = cron.scheduleJob(EVERY_TEN_MINUTES, () => {
    emitter.emit(events.EVERY_TEN_MINUTES);
  });

  const c = cron.scheduleJob(EVERY_MINUTE, () => {
    emitter.emit(events.EVERY_MINUTE);
  });

  jobs.push(a);
  jobs.push(b);
  jobs.push(c);

  return jobs;
}

module.exports = {
  runJobs,
};