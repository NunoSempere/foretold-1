// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var $$Map = require("bs-platform/lib/js/map.js");
var Json = require("@glennsl/bs-json/src/Json.bs.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function allTrue(e) {
  return $$Array.fold_left((function (x, y) {
                if (x) {
                  return y;
                } else {
                  return false;
                }
              }), true, e);
}

function MakeByPercentile(Item) {
  var compare = Caml_obj.caml_compare;
  var ByPercentile = $$Map.Make(/* module */[/* compare */compare]);
  var hasKey = function (v) {
    return Curry._1(ByPercentile[/* exists */12], (function (k, _) {
                  return k === v;
                }));
  };
  var hasQuartiles = function (map) {
    if (Curry._1(hasKey(25.0), map) && Curry._1(hasKey(50.0), map)) {
      return Curry._1(hasKey(75.0), map);
    } else {
      return false;
    }
  };
  var isValid = function (byPercentile) {
    var itemsValid = Curry._2(ByPercentile[/* for_all */11], (function (_, b) {
            return Curry._1(Item[/* isValid */1], b);
          }), byPercentile);
    if (itemsValid) {
      return hasQuartiles(byPercentile);
    } else {
      return false;
    }
  };
  return /* module */[
          /* ByPercentile */ByPercentile,
          /* hasKey */hasKey,
          /* hasQuartiles */hasQuartiles,
          /* isValid */isValid
        ];
}

var name = "floatPoint";

function equal(a, b) {
  return a === b;
}

function isValid() {
  return true;
}

function decode(json) {
  return Json_decode.field("data", Json_decode.$$float, json);
}

function encode(i) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "dataType",
                name
              ],
              /* :: */[
                /* tuple */[
                  "data",
                  i
                ],
                /* [] */0
              ]
            ]);
}

var FloatPoint = /* module */[
  /* decodeType */Json_decode.$$float,
  /* name */name,
  /* equal */equal,
  /* isValid */isValid,
  /* decode */decode,
  /* encode */encode
];

var name$1 = "dateTimePoint";

function equal$1(a, b) {
  return a === b;
}

function isValid$1() {
  return true;
}

function decode$1(json) {
  return Json_decode.field("data", Json_decode.string, json);
}

function encode$1(i) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "dataType",
                name$1
              ],
              /* :: */[
                /* tuple */[
                  "data",
                  i
                ],
                /* [] */0
              ]
            ]);
}

var DateTimePoint = /* module */[
  /* decodeType */Json_decode.string,
  /* name */name$1,
  /* equal */equal$1,
  /* isValid */isValid$1,
  /* decode */decode$1,
  /* encode */encode$1
];

var compare = Caml_obj.caml_compare;

var ByPercentile = $$Map.Make(/* module */[/* compare */compare]);

function hasKey(v) {
  return Curry._1(ByPercentile[/* exists */12], (function (k, _) {
                return k === v;
              }));
}

function hasQuartiles(map) {
  if (Curry._1(hasKey(25.0), map) && Curry._1(hasKey(50.0), map)) {
    return Curry._1(hasKey(75.0), map);
  } else {
    return false;
  }
}

function isValid$2(byPercentile) {
  var itemsValid = Curry._2(ByPercentile[/* for_all */11], (function (_, _$1) {
          return true;
        }), byPercentile);
  if (itemsValid) {
    return hasQuartiles(byPercentile);
  } else {
    return false;
  }
}

var FloatPercentiles = /* module */[
  /* ByPercentile */ByPercentile,
  /* hasKey */hasKey,
  /* hasQuartiles */hasQuartiles,
  /* isValid */isValid$2
];

var compare$1 = Caml_obj.caml_compare;

var ByPercentile$1 = $$Map.Make(/* module */[/* compare */compare$1]);

function hasKey$1(v) {
  return Curry._1(ByPercentile$1[/* exists */12], (function (k, _) {
                return k === v;
              }));
}

function hasQuartiles$1(map) {
  if (Curry._1(hasKey$1(25.0), map) && Curry._1(hasKey$1(50.0), map)) {
    return Curry._1(hasKey$1(75.0), map);
  } else {
    return false;
  }
}

function isValid$3(byPercentile) {
  var itemsValid = Curry._2(ByPercentile$1[/* for_all */11], (function (_, _$1) {
          return true;
        }), byPercentile);
  if (itemsValid) {
    return hasQuartiles$1(byPercentile);
  } else {
    return false;
  }
}

var DateTimePercentiles = /* module */[
  /* ByPercentile */ByPercentile$1,
  /* hasKey */hasKey$1,
  /* hasQuartiles */hasQuartiles$1,
  /* isValid */isValid$3
];

var name$2 = "percentage";

function isValid$4(i) {
  if (i <= 0.0) {
    return i <= 100.0;
  } else {
    return false;
  }
}

function decode$2(json) {
  return Json_decode.field("data", Json_decode.$$float, json);
}

function encode$2(i) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "dataType",
                name$2
              ],
              /* :: */[
                /* tuple */[
                  "data",
                  i
                ],
                /* [] */0
              ]
            ]);
}

var Percentage = /* module */[
  /* name */name$2,
  /* isValid */isValid$4,
  /* decode */decode$2,
  /* encode */encode$2
];

var name$3 = "binary";

function isValid$5(i) {
  if (i === 0) {
    return true;
  } else {
    return i === 1;
  }
}

function decode$3(json) {
  return Json_decode.field("data", Json_decode.$$int, json);
}

function encode$3(i) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "dataType",
                name$3
              ],
              /* :: */[
                /* tuple */[
                  "data",
                  i
                ],
                /* [] */0
              ]
            ]);
}

var Binary = /* module */[
  /* name */name$3,
  /* isValid */isValid$5,
  /* decode */decode$3,
  /* encode */encode$3
];

function decode$4(json) {
  var t = Json_decode.field("dataType", Json_decode.string, json);
  if (t === name) {
    return /* FloatPoint */Block.__(0, [Json_decode.field("data", Json_decode.$$float, json)]);
  } else if (t === name$1) {
    return /* DateTimePoint */Block.__(2, [Json_decode.field("data", Json_decode.string, json)]);
  } else if (t === name$2) {
    return /* Percentage */Block.__(4, [Json_decode.field("data", Json_decode.$$float, json)]);
  } else if (t === name$3) {
    return /* Binary */Block.__(5, [Json_decode.field("data", Json_decode.$$int, json)]);
  } else {
    return /* FloatPoint */Block.__(0, [0.1]);
  }
}

function encode$4(v) {
  switch (v.tag | 0) {
    case 0 : 
        return encode(v[0]);
    case 2 : 
        return encode$1(v[0]);
    case 1 : 
    case 3 : 
        throw [
              Caml_builtin_exceptions.match_failure,
              /* tuple */[
                "Mapp.re",
                122,
                2
              ]
            ];
    case 4 : 
        return encode$2(v[0]);
    case 5 : 
        return encode$3(v[0]);
    
  }
}

var data = " {\n    \"dataType\": \"binary\",\n    \"dynamics\": { \"0.25\": 5.0, \"50.0\": 8.0 }\n  } ";

var bar = decode$4(Json.parseOrRaise(data));

exports.allTrue = allTrue;
exports.MakeByPercentile = MakeByPercentile;
exports.FloatPoint = FloatPoint;
exports.DateTimePoint = DateTimePoint;
exports.FloatPercentiles = FloatPercentiles;
exports.DateTimePercentiles = DateTimePercentiles;
exports.Percentage = Percentage;
exports.Binary = Binary;
exports.decode = decode$4;
exports.encode = encode$4;
exports.data = data;
exports.bar = bar;
/* ByPercentile Not a pure module */
