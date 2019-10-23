open FC;
open Base;

let cdf = ExampleCdfs.Example1.cdf;

let futureTime = 1559005200;

let llink =
  Link.Jsx2.make(
    ~isDisabled=false,
    ~className=
      Css.(
        style([
          textDecoration(`underline),
          color(`hex("384e67")),
          hover([color(Colors.link)]),
        ])
      ),
  );

let row =
  <Table.Row.Jsx2 onClick={_ => Js.log("Row Clicked")}>
    <Table.Cell.Jsx2 flex={`num(4.)}>
      <span className=Table.Styles.Elements.primaryText>
        {"What will be the " |> ReasonReact.string}
        {llink(~href="d", [|"GDP" |> ReasonReact.string|])
         |> ReasonReact.element}
        {" of " |> ReasonReact.string}
        {llink(~href="China", [|"China" |> ReasonReact.string|])
         |> ReasonReact.element}
        {" in " |> ReasonReact.string}
        {llink(~href="2018", [|"2018" |> ReasonReact.string|])
         |> ReasonReact.element}
      </span>
      {FC__StateStatus.make(
         ~state=OPEN(MomentRe.momentWithUnix(futureTime)),
         ~fontSize=`em(0.85),
         (),
       )}
    </Table.Cell.Jsx2>
    <Table.Cell.Jsx2 flex={`num(2.)}>
      <FC__CdfChart__Small.Jsx2
        cdf
        minX={Some(2.0)}
        color={`hex("#d9dcdf")}
        maxX={Some(12.0)}
      />
    </Table.Cell.Jsx2>
    <Table.Cell.Jsx2 flex={`num(1.)} properties=Css.[paddingTop(`em(0.3))]>
      <Div.Jsx2>
        <Link.Jsx2
          className={Table.Styles.Elements.link(~isUnderlined=false, ())}>
          {"Series A" |> ReasonReact.string}
        </Link.Jsx2>
        <Link.Jsx2
          className={Table.Styles.Elements.link(~isUnderlined=false, ())}>
          {"19" |> ReasonReact.string}
        </Link.Jsx2>
      </Div.Jsx2>
      <Div.Jsx2>
        <Link.Jsx2
          className={Table.Styles.Elements.link(~isUnderlined=true, ())}>
          {"Edit" |> ReasonReact.string}
        </Link.Jsx2>
        <Link.Jsx2
          className={Table.Styles.Elements.link(~isUnderlined=true, ())}>
          {"Archive" |> ReasonReact.string}
        </Link.Jsx2>
      </Div.Jsx2>
    </Table.Cell.Jsx2>
  </Table.Row.Jsx2>;

let make =
  <PageCard.Jsx2>
    <PageCard.HeaderRow.Jsx2>
      <Div.Jsx2
        float=`left
        className={Css.style([
          PageCard.HeaderRow.Styles.itemTopPadding,
          PageCard.HeaderRow.Styles.itemBottomPadding,
        ])}>
        <Tab2.Jsx2 isActive=true number=12>
          {"Open" |> ReasonReact.string}
        </Tab2.Jsx2>
        <Tab2.Jsx2 isActive=false number=18>
          {"Pending Resolution" |> ReasonReact.string}
        </Tab2.Jsx2>
        <Tab2.Jsx2 isActive=false number=831>
          {"Closed" |> ReasonReact.string}
        </Tab2.Jsx2>
      </Div.Jsx2>
      <Div.Jsx2
        float=`right
        styles=[Css.style([PageCard.HeaderRow.Styles.itemTopPadding])]>
        {PaginationButtons.make({
           currentValue: Range(3, 10),
           max: 100,
           pageLeft: {
             isDisabled: false,
             onClick: _ => (),
           },
           pageRight: {
             isDisabled: true,
             onClick: _ => (),
           },
         })}
      </Div.Jsx2>
    </PageCard.HeaderRow.Jsx2>
    <Table.Jsx2>
      <Table.HeaderRow.Jsx2>
        <Table.Cell.Jsx2 flex={`num(4.)}>
          {"Name & Status" |> ReasonReact.string}
        </Table.Cell.Jsx2>
        <Table.Cell.Jsx2 flex={`num(2.)}>
          {"Aggregate Prediction" |> ReasonReact.string}
        </Table.Cell.Jsx2>
        <Table.Cell.Jsx2 flex={`num(1.)}>
          {"Details" |> ReasonReact.string}
        </Table.Cell.Jsx2>
      </Table.HeaderRow.Jsx2>
      row
      row
      row
      row
      row
      row
      row
      row
      row
    </Table.Jsx2>
  </PageCard.Jsx2>;
