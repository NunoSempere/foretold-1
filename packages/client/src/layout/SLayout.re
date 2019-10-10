open Utils;

module Styles = {
  open Css;

  let width100 = style([width(`percent(100.))]);
  let headerText = style([color(`hex("486474"))]);
  let descriptionText = style([color(`hex("aaa"))]);

  let seriesText =
    style([
      color(`hex("3d547b")),
      fontSize(`em(2.0)),
      float(`left),
      fontWeight(`medium),
    ]);

  let channelText =
    style([
      color(`hex("2c436e")),
      fontSize(`em(1.15)),
      fontWeight(`num(600)),
      marginTop(`px(3)),
      float(`left),
    ]);

  let container = isFluid =>
    isFluid
      ? style([paddingLeft(`em(2.0)), paddingRight(`em(2.0))])
      : style([maxWidth(`px(1170)), margin(`auto)]);

  let backHover = style([fontSize(`em(1.3))]);

  let dash =
    style([
      float(`left),
      marginTop(`em(0.2)),
      marginRight(`em(0.3)),
      marginLeft(`em(0.3)),
      fontSize(`em(1.5)),
      color(`hex("ccc")),
    ]);
};

module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let textDiv = text =>
    <FC__PageCard.HeaderRow.Title>
      {text |> ste}
    </FC__PageCard.HeaderRow.Title>;

  let make = children => {
    ...component,
    render: _ => <FC.PageCard.HeaderRow> ...children </FC.PageCard.HeaderRow>,
  };
};

module LayoutConfig = {
  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
    isFluid: bool,
  };
  let make = (~head, ~body, ~isFluid=false, ()) => {head, body, isFluid};
};

module FullPage = {
  let component = ReasonReact.statelessComponent("FullPage");
  let make = ({head, body, isFluid}: LayoutConfig.t) => {
    ...component,
    render: _ =>
      <FC.Base.Div.Jsx2
        className=Css.(
          style(
            [
              marginTop(`em(1.)),
              width(`percent(100.)),
              boxSizing(`borderBox),
            ]
            @ FC.Base.BaseStyles.fullWidthFloatLeft,
          )
        )>
        <div className={Styles.container(isFluid)}>
          <FC.PageCard>
            <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
            <FC.PageCard.Body> body </FC.PageCard.Body>
          </FC.PageCard>
        </div>
      </FC.Base.Div.Jsx2>,
  };
  let makeWithEl = (t: LayoutConfig.t) => t |> make |> E.React.el;
};

let channelBack = (~onClick, ()) =>
  <FC__Button onClick size=Medium>
    {"< Back" |> ReasonReact.string}
  </FC__Button>;

let channelLink = (c: Types.channel) =>
  <Link.Jsx2
    linkType={Internal(ChannelShow(c.id))} className=Styles.channelText>
    {switch (c.id) {
     | "home" =>
       Primary.Channel.presentGlobal(
         ~symbolClassName=Primary.Channel.Styles.globe,
         (),
       )
     | _ => c |> Primary.Channel.present
     }}
  </Link.Jsx2>;

let channelEditLink = (c: Types.channel) =>
  <Link.Jsx2
    linkType={Internal(ChannelEdit(c.id))} className=Styles.headerText>
    {"edit" |> ste}
  </Link.Jsx2>;

let seriesHead = (channel: Types.channel, seriesName) =>
  <>
    <div className=Styles.seriesText>
      <Icon.Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>
  </>;

let component = ReasonReact.statelessComponent("SLayout");
let make = (~head=ReasonReact.null, ~isFluid=false, children) => {
  ...component,
  render: _ =>
    <FC.Base.Div.Jsx2
      className=Css.(
        style(
          [
            marginTop(`em(1.)),
            width(`percent(100.)),
            boxSizing(`borderBox),
          ]
          @ FC.Base.BaseStyles.fullWidthFloatLeft,
        )
      )>
      <div className={Styles.container(isFluid)}>
        <FC.PageCard>
          <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
          <FC.PageCard.Body>
            {children |> ReasonReact.array}
          </FC.PageCard.Body>
        </FC.PageCard>
      </div>
    </FC.Base.Div.Jsx2>,
};