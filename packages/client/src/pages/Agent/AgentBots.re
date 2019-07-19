open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let paddingRight = [paddingRight(`em(1.))] |> style;
};

let title =
  <FC.Base.Div float=`left>
    <FC.PageCard.HeaderRow.Title>
      {"Agent Bots" |> ReasonReact.string}
    </FC.PageCard.HeaderRow.Title>
  </FC.Base.Div>;

let agentSection = (agent: GetAgent.agent) =>
  switch (agent) {
  | {user: Some(_user)} =>
    <>
      {E.React.showIf(
         agent.isMe,
         <Div float=`right>
           <Antd.Button
             onClick={_ => Routing.Url.push(BotCreate)} _type=`primary>
             {"New Bot" |> ste}
           </Antd.Button>
         </Div>,
       )}
    </>
  | _ => E.React.null
  };

let component = ReasonReact.statelessComponent("AgentBots");

module Columns = {
  type column = Table.column(Primary.Bot.t);

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (bot: Primary.Bot.t) =>
          switch (bot.name, bot.agent) {
          | (Some(name), Some(agent)) =>
            <Foretold__Components__Link
              linkType={
                Internal(
                  Agent({agentId: agent.id, subPage: AgentMeasurements}),
                )
              }>
              {name |> ste}
            </Foretold__Components__Link>
          | _ => ReasonReact.null
          },
      (),
    );

  let descriptionColumn: column =
    Table.Column.make(
      ~name="Description" |> ste,
      ~render=
        (bot: Primary.Bot.t) =>
          bot.description |> Rationale.Option.default("") |> ste,
      (),
    );

  let tokenColumn: column =
    Table.Column.make(
      ~name="Token" |> ste,
      ~render=
        (bot: Primary.Bot.t) =>
          <div className=Styles.paddingRight>
            <Antd.Input value={bot.token |> Rationale.Option.default("")} />
          </div>,
      ~show=(bot: Primary.Bot.t) => bot.token != None,
      (),
    );

  let editColumn: column =
    Table.Column.make(
      ~name="Edit" |> ste,
      ~render=
        (bot: Primary.Bot.t) =>
          <Foretold__Components__Link linkType={Internal(BotEdit(bot.id))}>
            {"Edit Bot" |> ste}
          </Foretold__Components__Link>,
      ~show=
        (bot: Primary.Bot.t) =>
          Primary.Permissions.can(`BOT_UPDATE, bot.permissions),
      (),
    );

  let all: array(column) = [|
    nameColumn,
    descriptionColumn,
    tokenColumn,
    editColumn,
  |];
};

type pageParams = {id: string};

let getUserId = (agent: GetAgent.agent): string => {
  switch (agent.user) {
  | Some(user) => user.id
  | None => ""
  };
};

let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    GetAgent.component(
      ~id=pageParams.id,
      ({agent}) => {
        let showBots = bots =>
          Array.length(bots) > 0
            ? <FC.PageCard.Body>
                {Table.fromColumns(Columns.all, bots, ())}
              </FC.PageCard.Body>
            : <SLayout.NothingToShow />;

        let body =
          getUserId(agent) !== ""
            ? BotsGet.component(~ownerId=getUserId(agent), showBots)
            : <SLayout.NothingToShow />;

        let head =
          <div>
            title
            <FC.Base.Div
              float=`right
              className={Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
                FC.PageCard.HeaderRow.Styles.itemBottomPadding,
              ])}>
              {agentSection(agent)}
            </FC.Base.Div>
          </div>;

        SLayout.LayoutConfig.make(~head, ~body) |> layout;
      },
    ),
};