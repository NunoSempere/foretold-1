const _ = require('lodash');
const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Params } = require('../data/classes/params');
const { Filter } = require('../data/classes/filter');
const { Options } = require('../data/classes/options');
const { Query } = require('../data/classes/query');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: currentAgentId,
  });

  return data.series.getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.channelId
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const channelId = _.get(args, 'channelId');
  const currentAgentId = _.get(context, 'agent.id');

  const filter = new Filter({ channelId });
  const pagination = new Pagination();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: currentAgentId,
  });

  return data.series.getAll(filter, pagination, options);
}

/**
 * @todo:
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const datas = {
    ...args.input,
    creatorId: agentId,
  };
  return data.series.createOne(datas);
}

module.exports = {
  create,
  one,
  all,
};
