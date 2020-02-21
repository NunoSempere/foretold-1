const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
 */
class PreferenceModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Preference,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  PreferenceModel,
};
