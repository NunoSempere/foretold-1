// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var RList$Rationale = require("rationale/src/RList.js");
var Graph_Fact_Query$BsKen = require("./Graph_Fact_Query.bs.js");

function query(q, t) {
  return List.filter((function (param) {
                  return Graph_Fact_Query$BsKen.Query.run(q, param);
                }))(t);
}

function find(id, t) {
  return RList$Rationale.find((function (e) {
                return e.thingIdString === id;
              }), t);
}

function withQuery(query) {
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(query, param);
              }));
}

function withEdge(edge, id) {
  var partial_arg = {
    edge: edge,
    id: id,
    q: /* IS */0
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withoutEdge(edge, id) {
  var partial_arg = {
    edge: edge,
    id: id,
    q: /* IS_NOT */1
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withSubject(id) {
  var partial_arg = {
    edge: /* SUBJECT */0,
    id: id,
    q: /* IS */0
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withoutSubject(id) {
  var partial_arg = {
    edge: /* SUBJECT */0,
    id: id,
    q: /* IS_NOT */1
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withProperty(id) {
  var partial_arg = {
    edge: /* PROPERTY */1,
    id: id,
    q: /* IS */0
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withoutProperty(id) {
  var partial_arg = {
    edge: /* PROPERTY */1,
    id: id,
    q: /* IS_NOT */1
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withValue(id) {
  var partial_arg = {
    edge: /* VALUE */2,
    id: id,
    q: /* IS */0
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withoutValue(id) {
  var partial_arg = {
    edge: /* VALUE */2,
    id: id,
    q: /* IS_NOT */1
  };
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.run(partial_arg, param);
              }));
}

function withIdAsAnyEdge(id) {
  var partial_arg_000 = {
    edge: /* SUBJECT */0,
    id: id,
    q: /* IS */0
  };
  var partial_arg_001 = /* :: */[
    {
      edge: /* PROPERTY */1,
      id: id,
      q: /* IS */0
    },
    /* :: */[
      {
        edge: /* VALUE */2,
        id: id,
        q: /* IS */0
      },
      /* [] */0
    ]
  ];
  var partial_arg = /* :: */[
    partial_arg_000,
    partial_arg_001
  ];
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.qOr(partial_arg, param);
              }));
}

function withIdAsNoEdge(id) {
  var partial_arg_000 = {
    edge: /* SUBJECT */0,
    id: id,
    q: /* IS_NOT */1
  };
  var partial_arg_001 = /* :: */[
    {
      edge: /* PROPERTY */1,
      id: id,
      q: /* IS_NOT */1
    },
    /* :: */[
      {
        edge: /* VALUE */2,
        id: id,
        q: /* IS_NOT */1
      },
      /* [] */0
    ]
  ];
  var partial_arg = /* :: */[
    partial_arg_000,
    partial_arg_001
  ];
  return List.filter((function (param) {
                return Graph_Fact_Query$BsKen.Query.qAnd(partial_arg, param);
              }));
}

var filter = List.filter;

exports.query = query;
exports.find = find;
exports.filter = filter;
exports.withQuery = withQuery;
exports.withEdge = withEdge;
exports.withoutEdge = withoutEdge;
exports.withSubject = withSubject;
exports.withoutSubject = withoutSubject;
exports.withProperty = withProperty;
exports.withoutProperty = withoutProperty;
exports.withValue = withValue;
exports.withoutValue = withoutValue;
exports.withIdAsAnyEdge = withIdAsAnyEdge;
exports.withIdAsNoEdge = withIdAsNoEdge;
/* RList-Rationale Not a pure module */
