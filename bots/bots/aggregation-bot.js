const { API } = require('../api');

const config = require('../config');

class AggregationBot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const measurables = await this.api.measurables();

    for (const measurable of measurables) {
      const measurements = await this.api.measurementsCompetitive({
        measurableId: measurable.id,
      });
      const aggregated = await this.aggregate(measurements);
      await this.api.measurementCreateAggregation({
        measurableId: measurable.id,
        ...aggregated,
      });
    }

    return true;
  }

  /**
   * @param {object[]} measurements
   * @return {Promise<{floatPoint: number}>}
   */
  async aggregate(measurements) {
    return {
      floatPoint: 7.77,
    };
  }
}

module.exports = {
  AggregationBot,
};
