module type KenModule = {
  type t;
  let graph: Graph_Dirs.t;
  let itemUrl: string => string;
  let findName: string => option(string);
  let names: string => list(Graph_T.T.fact);
  let findInstanceOfName: string => option(Graph_T.T.thingIdString);
  let things: array(Graph_T.T.thing);
  let getName: Graph_T.T.thing => string;
  let getInstanceOfName: Graph_T.T.thing => string;
  let withNames: Js.Array.t(Graph_T.T.thing) => Js.Array.t(Graph_T.T.thing);
};

module Functor =
       (Config: {let globalSetting: option(Types.globalSetting);})
       : KenModule => {
  type t = string;

  let graph =
    Config.globalSetting
    |> E.O.fmap((globalSetting: Types.globalSetting) =>
         globalSetting.entityGraph |> E.O.default(Js.Json.null)
       )
    |> E.O.default(Js.Json.null)
    |> (e => Ken_Interface.Graph.fromJson(e));

  let itemUrl = id => Routing.Url.toString(EntityShow(id));

  let findName = propertyId =>
    graph
    |> Graph_T.F.factList
    |> Graph_Fact_Filters.withSubject(propertyId)
    |> Graph_Fact_Filters.withProperty("@base/properties/p-name")
    |> E.L.head
    |> E.O.bind(_, (k: Graph_T.T.fact) =>
         switch (k.value.valueType) {
         | String(s) => Some(s)
         | ThingId(s) => Some(s)
         | _ => None
         }
       );

  let findInstanceOfName = propertyId =>
    graph
    |> Graph_T.F.factList
    |> Graph_Fact_Filters.withSubject(propertyId)
    |> Graph_Fact_Filters.withProperty("@base/properties/p-instance-of")
    |> E.L.head
    |> E.O.bind(_, (k: Graph_T.T.fact) =>
         switch (k.value.valueType) {
         | String(s) => Some(s)
         | ThingId(s) => findName(s)
         | _ => None
         }
       );

  let names = subjectId =>
    graph
    |> Graph_T.F.factList
    |> Graph_Fact_Filters.withSubject(subjectId)
    |> E.L.fmap((f: Graph_T.T.fact) => f);

  let things = graph |> Graph_T.F.thingArray;

  let getName = (t: Graph_T.T.thing) =>
    t |> Graph_T.Thing.id |> findName |> E.O.default("");

  let getInstanceOfName = (t: Graph_T.T.thing) =>
    t |> Graph_T.Thing.id |> findInstanceOfName |> E.O.default("");

  let hasName = (t: Graph_T.T.thing) => getName(t) != "";

  let withNames = E.A.filter(hasName);
};