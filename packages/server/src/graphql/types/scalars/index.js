const graphql = require('graphql');

const { LimitedLengthType } = require('./limited-length-type');
const jwt = require('./jwt');
const objectId = require('./object-id');

const string30 = new LimitedLengthType(graphql.GraphQLString, 30);
const string64 = new LimitedLengthType(graphql.GraphQLString, 64);
const string256 = new LimitedLengthType(graphql.GraphQLString, 256);
const string512 = new LimitedLengthType(graphql.GraphQLString, 512);
const string8K = new LimitedLengthType(graphql.GraphQLString, 1024*8);

module.exports = {
  jwt,
  objectId,
  string30,
  string64,
  string256,
  string512,

  string8K,

  LimitedLengthType,
};
