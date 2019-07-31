const models = require('../models');
const { ChannelMembershipsData } = require('./channel-memberships-data');

describe('Data Layer - Channel Memberships Data', () => {

  it('class should be a constructor', () => {
    expect(ChannelMembershipsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelMembershipsData();
  const channelId = 'channelId1';
  const agentId = 'agentId1';
  const inviterAgentId = 'inviterAgentId1';
  const input = { channelId, agentId };
  const role = 'VIEWER';

  describe('createOne2() when there is user', () => {
    beforeEach(() => {
      jest.spyOn(models.ChannelMemberships, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds user without creating new one', () => {
      return instance.createOne2(channelId, agentId, inviterAgentId, role).then((result) => {
        expect(models.ChannelMemberships.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.ChannelMemberships.create).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('createOne2() when there is no user', () => {
    beforeEach(() => {
      jest.spyOn(models.ChannelMemberships, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('creates new user', () => {
      return instance.createOne2(channelId, agentId, inviterAgentId, role).then((result) => {
        expect(models.ChannelMemberships.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.ChannelMemberships.create).toHaveBeenCalledWith({
          ...input, inviterAgentId, role
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('deleteOne()', () => {
    beforeEach(() => {
      jest.spyOn(instance.ChannelMembershipModel, 'deleteOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds and destroys user', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(instance.ChannelMembershipModel.deleteOne).toHaveBeenCalledWith(
          channelId,
          agentId,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('_validate() if all input is present', () => {
    it('finds channel and agent', () => {
      return instance._validate(input).then((result) => {
        expect(models.Channel.findByPk).toHaveBeenCalledWith(channelId);
        expect(models.Agent.findByPk).toHaveBeenCalledWith(agentId);
        expect(result).toBe(true);
      });
    });
  });

  describe('_validate() if there is channel', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findByPk').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('throws an error ', () => {
      return instance._validate(input).catch((err) => {
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('_validate() if there are channel and agent', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findByPk').mockReturnValue(
        Promise.resolve(false),
      );
      jest.spyOn(models.Agent, 'findByPk').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('does not find agent', () => {
      return instance._validate({ agentId }).catch((err) => {
        expect(models.Channel.findByPk).toHaveBeenCalledTimes(1);
        expect(models.Agent.findByPk).toHaveBeenCalledTimes(0);
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('getOne()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.ChannelMemberships, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds agent-channel note ', () => {
      return instance.getOne(options).then((result) => {
        expect(models.ChannelMemberships.findOne).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAll()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.ChannelMemberships, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('find agents-channels notes', () => {
      return instance.getAll(options).then((result) => {
        expect(models.ChannelMemberships.findAll).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAllChannelIds()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getAll').mockReturnValue(Promise.resolve([
        { channelId: 'channelId1' }
      ]));
    });
    it('calls getAll()', () => {
      return instance.getAllChannelIds(options).then((result) => {
        expect(instance.getAll).toHaveBeenCalledWith(options);
        expect(result).toEqual(['channelId1']);
      });
    });
  });

  describe('updateOne2()', () => {
    beforeEach(() => {
      jest.spyOn(instance.ChannelMembershipModel, 'updateOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, '_validate').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates user', () => {
      return instance.updateOne2(channelId, agentId, role).then((result) => {
        expect(instance.ChannelMembershipModel.updateOne).toHaveBeenCalledWith(
          channelId,
          agentId,
          role,
        );
        expect(result).toBe(true);
      });
    });
  });

});

