const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

const { MEASURABLE_STATE } = require('../models/measurable-state');

class MeasurableModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurable,
      sequelize: models.sequelize,
    });
    this.Op = this.sequelize.Op;
    this.fn = this.sequelize.fn;
  }

  /**
   * @return {Sequelize.literal|*}
   */
  getStateOrderField() {
    return this.sequelize.literal(
      `(CASE WHEN "state"='${MeasurableModel.MEASURABLE_STATE.OPEN}' THEN 1 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGEMENT_PENDING}' THEN 2 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGED}' THEN 3 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.ARCHIVED}' THEN 4 ` +
      `ELSE 5 END) AS "stateOrder"`,
    );
  }

  /**
   * @return {Promise<boolean>}
   */
  setJudgementPending() {
    return this.model.update({
      state: MeasurableModel.MEASURABLE_STATE.JUDGEMENT_PENDING,
    }, {
      where: {
        state: MeasurableModel.MEASURABLE_STATE.OPEN,
        [this.Op.or]: [
          {
            expectedResolutionDate: {
              [this.Op.lt]: this.fn('now'),
            }
          },
          { expectedResolutionDate: null },
        ],
      },
    }).then(() => true);
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
