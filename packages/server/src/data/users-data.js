const _ = require('lodash');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');

const { UserModel } = require('../models-abstract');

const { ForetoldAuthId } = require('../models/classes/foretold-auth-id');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {UserModel} UserModel
 */
class UsersData extends DataBase {

  constructor() {
    super();
    this.UserModel = new UserModel();
    this.model = this.UserModel;

    this.measurements = new MeasurementsData();
  }

  /**
   * @public
   * @param {string} auth0Id
   * @return {Promise<Models.User>}
   */
  async getUserByAuth0Id(auth0Id) {
    const params = { auth0Id };
    const query = {};
    const data = { auth0Id };
    return this.upsertOne(params, query, data);
  }


  /**
   * @todo: fix interface
   * @todo: move to permissions
   * @public
   * @deprecated: use updateOne
   * @param {Models.ObjectID} id
   * @param {object} data
   * @param {object} _user
   * @return {Promise<Models.User>}
   */
  async updateOne(id, data, _user) {
    const user = await this.models.User.findByPk(id);
    if (user && user.auth0Id === _user.auth0Id) {
      await user.update(data);
    }
    return user;
  }

  /**
   * @public
   * @param {string} id
   * @param {Auth0UserInfoResponse} userInfo
   * @return {Promise<Models.User>}
   */
  async updateUserInfoFromAuth0(id, userInfo) {
    const user = await this.models.User.findByPk(id);

    const emailIn = _.get(userInfo, 'email');
    const isEmailVerifiedIn = !!_.get(userInfo, 'email_verified');
    const nicknameIn = _.get(userInfo, 'nickname');
    const pictureIn = _.get(userInfo, 'picture');
    const auth0IdIn = _.get(userInfo, 'sub');

    const email = _.toString(emailIn).substr(0, 64);
    const nickname = _.toString(nicknameIn).substr(0, 30);
    const picture = _.toString(pictureIn).substr(0, 255);
    const auth0Id = _.toString(auth0IdIn).substr(0, 255);

    const emailValid = email !== '' && isEmailVerifiedIn === true;
    const foretoldAuthId = new ForetoldAuthId(email).toString();

    if (user.email === null && emailValid) {
      user.set('email', email);
      user.set('isEmailVerified', isEmailVerifiedIn);
    }
    if (user.name === '' && nickname !== '') {
      user.set('name', nickname);
    }
    if (user.picture === null && picture !== '') {
      user.set('picture', picture);
    }
    if (user.auth0Id === foretoldAuthId && auth0Id !== '') {
      user.set('auth0Id', auth0Id);
    }

    await user.save();
    return user;
  }

  /**
   * @public
   * @param {Models.ObjectID} agentId
   * @return {Promise<number>}
   */
  async getScore(agentId) {
    return this.measurements.getBrierScore(agentId);
  }
}

module.exports = {
  UsersData,
};
