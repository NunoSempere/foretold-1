const _ = require('lodash');

const { PreferencesData } = require('../../data');

const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function getOne(root, _args, _context, _info) {
  const agentId = _.get(root, 'id', null);

  const params = new Params({ agentId });
  const query = new Query();
  const data = new Data({ agentId });

  return new PreferencesData().upsertOne(params, query, data);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.PreferenceID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function update(_root, args, _context, _info) {
  const id = _.get(args, 'id', null);

  const params = new Params({ id });
  const data = new Data(args.input);
  const options = new Options();

  return new PreferencesData().updateOne(params, data, options);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.AgentID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function subscribe(_root, args, _context, _info) {
  const agentId = _.get(args, 'agentId', null);
  return new PreferencesData().subscribe(agentId);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.AgentID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function unsubscribe(_root, args, _context, _info) {
  const agentId = _.get(args, 'agentId', null);
  return new PreferencesData().unsubscribe(agentId);
}

module.exports = {
  getOne,
  update,
  subscribe,
  unsubscribe,
};
