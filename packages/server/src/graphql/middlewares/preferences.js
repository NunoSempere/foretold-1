const _ = require('lodash');

const data = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/preferences');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextPreference(root, args, context, info) {
  const id = _.get(args, 'id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextPreference)', { id });

  context.preference = !!id
    ? await data.preferences.getOne({ id })
    : null;
}

module.exports = {
  setContextPreference,
};
