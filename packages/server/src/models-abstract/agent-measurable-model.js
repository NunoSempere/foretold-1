const _ = require('lodash');
const assert = require('assert');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentMeasurableModel extends ModelPostgres {

  constructor() {
    super({
      model: models.AgentMeasurable,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @param {string} [name]
   * @return {string}
   */
  async getAgentMeasurableScoring(agentId, measurableId, name = '') {
    const query = this._agentMeasurableScoring(agentId, measurableId);
    const response = await this.sequelize.query(query);
    return response;
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @param {string} [name]
   * @return {string}
   */
  _agentMeasurableScoringLiteral(agentId, measurableId, name = '') {
    return this.literal(this._agentMeasurableScoring(
      agentId,
      measurableId,
      name,
    ));
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @param {string} [name]
   * @return {string}
   */
  _agentMeasurableScoring(agentId, measurableId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurableId, 'Measurable ID is required.');
    return `(
      /* Agent-Measurable Scoring (${ name }) */
      SELECT
        /* all of the aggregates made after that */
        (
          SELECT ARRAY(
            SELECT (
              "Measurements2"."id", 
              "Measurements2"."value" ->> 'data',
              "Measurements2"."createdAt"
            )
            FROM "Measurements" AS "Measurements2"
            WHERE "Measurements2"."competitorType" = 'AGGREGATION'
              AND "Measurements2"."measurableId" = "Measurements"."measurableId"
              AND "Measurements2"."createdAt" > "Measurements"."createdAt"
          )
        ) AS "aggregations",
    
        /* and the aggregate made immediately before that, with timestamps */
        (
          SELECT (
            "Measurements3"."id", "Measurements3"."createdAt",
            "Measurements3"."value" ->> 'data'
          )
          FROM "Measurements" AS "Measurements3"
          WHERE "Measurements3"."competitorType" = 'AGGREGATION'
            AND "Measurements3"."measurableId" = "Measurements"."measurableId"
            AND "Measurements3"."createdAt" < "Measurements"."createdAt"
          ORDER BY "Measurements3"."createdAt" DESC
          LIMIT 1
         ) AS "previousAggregation",
    
        /* the most recent result, with a timestamp */
        (
          SELECT (
            "Measurements4"."id", "Measurements4"."createdAt",
            "Measurements4"."value" ->> 'data'
          )
          FROM "Measurements" AS "Measurements4"
          WHERE "Measurements4"."competitorType" = 'OBJECTIVE'
            AND "Measurements4"."measurableId" = "Measurements"."measurableId"
          ORDER BY "Measurements4"."createdAt" DESC
          LIMIT 1
        ) AS "recentResult",
        
        "Measurements"."id",
        "Measurements"."agentId",
        "Measurements"."createdAt",
    
        /* the timestamp of when the measurable was created */
        "Measurables"."createdAt" AS "measurableCreatedAt"
      
      FROM "Measurements"
               LEFT JOIN "Measurables"
                         ON "Measurables"."id" = "Measurements"."measurableId"
      
      WHERE "Measurements"."competitorType" = 'COMPETITIVE'
        AND "Measurements"."agentId" = '${ agentId }'
        AND "Measurements"."measurableId" = '${ measurableId }'
      ORDER BY "Measurements"."createdAt" DESC
    )`;
  }
}

module.exports = {
  AgentMeasurableModel,
};
