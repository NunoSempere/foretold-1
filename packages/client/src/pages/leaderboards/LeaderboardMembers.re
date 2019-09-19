module ReducerConfig = {
  type itemType = Types.agentChannel;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    AgentChannelsGet.component(~channelId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardChannels");
let make =
    (
      ~channelId=None,
      ~agentId=None,
      ~head=Leaderboard.head(~subTab=ByMember),
      ~columns=LeaderboardTable.Columns.members,
      _children,
    ) => {
  ...component,
  render: _ => {
    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
        | _ => [||]
        };

      let items = items |> E.A.fmap(Primary.LeaderboardItem.fromAgentChannel);

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2 items columns />
              </FC.PageCard.Body>
            : <NothingToShow />
        | _ => <Spin />
        };

      let head =
        head(
          ~channelId,
          ~paginationPage=Reducer.Components.paginationPage(reducerParams),
          (),
        );

      <SLayout head> body </SLayout>;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};