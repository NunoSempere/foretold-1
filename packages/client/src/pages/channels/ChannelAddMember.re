[@bs.config {jsx: 3}];

type column = Table.column(Types.agent);

[@react.component]
let make = (~channelId: string) => {
  let addToChannelLink = (agentId: string, channelId: string) =>
    ChannelMembershipCreate.Mutation.make((mutation, _) =>
      <Link.Jsx2
        linkType={
          Action(
            _ => ChannelMembershipCreate.mutate(mutation, agentId, channelId),
          )
        }>
        {"Add to Community" |> ReasonReact.string}
      </Link.Jsx2>
    )
    |> ReasonReact.element;

  let agentColumn =
    Table.Column.make(
      ~name="Member" |> ReasonReact.string,
      ~render=(agent: Types.agent) => <AgentLink.Jsx2 agent />,
      (),
    );

  let inviteColumn =
    Table.Column.make(
      ~name="Invite" |> ReasonReact.string,
      ~render=(agent: Types.agent) => addToChannelLink(agent.id, channelId),
      (),
    );

  let all: array(column) = [|agentColumn, inviteColumn|];

  let title =
    <FC.Base.Div.Jsx2 float=`left>
      <FC.PageCard.HeaderRow.Title>
        {"Add Agents" |> ReasonReact.string}
      </FC.PageCard.HeaderRow.Title>
    </FC.Base.Div.Jsx2>;

  let onSuccess = agents => {
    let dataSource =
      agents
      |> Js.Array.filter((agent: Types.agent) =>
           switch (agent.name) {
           | Some(name) when name != "" => true
           | _ => false
           }
         );

    Table.fromColumns(all, dataSource, ());
  };

  let onError = e => <Error e />;

  let loadingFn = () => <Spin />;

  let table =
    AgentsGet.componentUsers(~excludeChannelId=channelId, agents =>
      agents |> HttpResponse.flatten(onSuccess, onError, loadingFn)
    );

  <SLayout head={<div> title </div>}>
    <FC.PageCard.Body> table </FC.PageCard.Body>
  </SLayout>;
};
