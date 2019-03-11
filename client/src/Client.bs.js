// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var E$Client = require("./types/E.bs.js");
var Env$Client = require("./utils/Env.bs.js");
var ApolloLinks = require("reason-apollo/src/ApolloLinks.bs.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var ApolloLink = require("apollo-link");
var Auth0$Client = require("./utils/Auth0.bs.js");
var ReasonApollo = require("reason-apollo/src/ReasonApollo.bs.js");
var ApolloLinkError = require("apollo-link-error");
var ApolloInMemoryCache = require("reason-apollo/src/ApolloInMemoryCache.bs.js");

var inMemoryCache = ApolloInMemoryCache.createInMemoryCache(undefined, undefined, /* () */0);

function headers(param) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "authorization",
                "Bearer " + Curry._2(E$Client.O[/* default */3], "", Auth0$Client.authToken(/* () */0))
              ],
              /* [] */0
            ]);
}

var httpLink = ApolloLinks.createHttpLink(Env$Client.serverUrl, undefined, undefined, undefined, undefined, undefined, /* () */0);

var contextLink = ApolloLinks.createContextLink((function (param) {
        return {
                headers: headers(/* () */0)
              };
      }));

var errorLink = ApolloLinkError.onError((function (error) {
        console.log("GraphQL Error!", error);
        return /* () */0;
      }));

var link = ApolloLink.from(/* array */[
      contextLink,
      errorLink,
      httpLink
    ]);

var instance = ReasonApollo.createApolloClient(link, inMemoryCache, undefined, undefined, undefined, undefined, /* () */0);

exports.inMemoryCache = inMemoryCache;
exports.headers = headers;
exports.httpLink = httpLink;
exports.contextLink = contextLink;
exports.errorLink = errorLink;
exports.link = link;
exports.instance = instance;
/* inMemoryCache Not a pure module */
