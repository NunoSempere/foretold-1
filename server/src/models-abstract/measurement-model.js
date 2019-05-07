const _ = require('lodash');

const models = require('../models');
const { splitBy } = require('../lib/functions');

const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MeasurementModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {string} [filter.measurableId]
   * @param {string} [filter.agentId]
   * @param {string[]} [filter.competitorType]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {Promise<{data: Models.Measurement[], total: number}>}
   */
  async getAll(filter, pagination, restrictions) {
    const startDate = _.get(filter, 'findInDateRange.startDate');
    const endDate = _.get(filter, 'findInDateRange.endDate');
    const spacedLimit = _.get(filter, 'findInDateRange.spacedLimit');

    const where = {};

    this.applyRestrictions(where, restrictions);

    if (filter.measurableId) where.measurableId = filter.measurableId;
    if (filter.agentId) where.agentId = filter.agentId;
    if (filter.competitorType) where.competitorType = {
      [this.in]: filter.competitorType,
    };
    if (startDate) _.set(where, ['createdAt', this.gte], startDate);
    if (endDate) _.set(where, ['createdAt', this.lte], endDate);

    /** @type {number} */
    const total = await this.model.count({ where });
    const edgePagination = this.getPagination(pagination, total);

    const cond = {
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [['createdAt', 'DESC']],
      where,
    };

    /** @type {Models.Measurement[]} */
    let data = await this.model.findAll(cond);
    data = this.setIndexes(data, edgePagination);
    data.total = total;

    // tricky
    if (spacedLimit) data = splitBy(data, spacedLimit);

    return { data, total };
  }
}

module.exports = {
  MeasurementModel,
};
