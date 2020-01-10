const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { ChannelMembershipModel } = require('../models-abstract');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../enums');

const { Data } = require('../data/classes');
const { Filter } = require('../data/classes');
const { Params } = require('../data/classes');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {ChannelMembershipModel} model
 */
class ChannelMembershipsData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelMembershipModel();
  }

  /**
   * @public
   * @param {object} filter
   * @param {Models.AgentID} [filter.agentId]
   * @param {Models.ChannelID} [filter.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(filter) {
    return (await this.getAll(filter)).map((o) => o.channelId);
  }

  /**
   * @param {object} options
   * @param {Models.ChannelID} options.channelId
   * @param {Models.AgentID} options.agentId
   * @return {Promise<Models.ChannelMemberships>}
   */
  async join(options) {
    assert(!!_.get(options, 'agentId'),
      'join::agentId is required.');
    assert(!!_.get(options, 'channelId'),
      'join::channelId is required.');

    const data = new Data({
      channelId: options.channelId,
      agentId: options.agentId,
      methodCreatedBy: CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY,
    });
    return this.createOne(data);
  }

  /**
   * @public
   * @param {object} options
   * @param {Models.AgentID} options.agentId
   * @param {Models.ChannelID} options.channelId
   * @return {Promise<Models.ChannelMemberships|null>}
   */
  async leave(options) {
    assert(!!_.get(options, 'agentId'),
      'leave::agentId is required.');
    assert(!!_.get(options, 'channelId'),
      'leave::channelId is required.');

    return this.deleteOne(new Params({
      channelId: options.channelId,
      agentId: options.agentId,
    }));
  }

  /**
   * @public
   * @param {object} params
   * @param {Models.AgentID} params.agentId
   * @param {Models.ChannelID} params.channelId
   * @return {Promise<string>}
   */
  async getOneOnlyRole(params) {
    assert(!!_.get(params, 'channelId'),
      'getOneOnlyRole::channelId is required.');

    if (!_.get(params, 'agentId')) {
      return ChannelMembershipModel.ROLES.NONE;
    }

    const channelMembership = await this.getOne(params);
    return _.get(
      channelMembership,
      'role',
      ChannelMembershipModel.ROLES.NONE,
    );
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} filter
   * @return {Promise<Models.ChannelMemberships[]>}
   */
  async getAllOnlyAdmins(filter) {
    return this.getAll(new Filter({
      ...filter,
      role: ChannelMembershipModel.ROLES.ADMIN,
    }));
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  ChannelMembershipsData,
};
