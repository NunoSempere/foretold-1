const { INVITATION_STATUS } = require('../src/models/enums/invitation-status');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.createTable('Invitations', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        tokenId: {
          type: Sequelize.UUID,
          references: {
            model: 'Tokens',
            key: 'id',
          },
          allowNull: false,
        },
        inviterAgentId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: false,
        },
        channelId: {
          type: Sequelize.UUID,
          references: {
            model: 'Channels',
            key: 'id',
          },
          allowNull: false,
        },
        status: {
          type: Sequelize.ENUM([
            INVITATION_STATUS.AWAITING,
            INVITATION_STATUS.ACCEPTED,
          ]),
          defaultValue: INVITATION_STATUS.AWAITING,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.dropTable('Invitations');

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
