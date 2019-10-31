[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.leaderboardItem;
  type column = Table.column(record);

  let getName = (r: record): ReasonReact.reactElement =>
    switch (r.measurable) {
    | Some(measurable) =>
      <Link
        linkType={
          Internal(MeasurableShow(measurable.channelId, measurable.id))
        }>
        {measurable.name |> E.S.default("Question") |> Utils.ste}
      </Link>
    | _ => "Question" |> Utils.ste
    };

  let measurable =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> getName,
      ~flex=4,
      (),
    );

  let agent =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=
        (r: record) =>
          r.agent
          |> E.O.fmap((agent: Types.agent) => <AgentLink agent />)
          |> E.O.default("Member" |> Utils.ste),
      ~flex=1,
      (),
    );

  let channel =
    Table.Column.make(
      ~name="Community" |> Utils.ste,
      ~render=
        (r: record) =>
          r.channel
          |> E.O.fmap((channel: Types.channel) =>
               <Link linkType={Internal(ChannelShow(channel.id))}>
                 {channel.name |> Utils.ste}
               </Link>
             )
          |> E.O.default("Community" |> Utils.ste),
      ~flex=1,
      (),
    );

  let score =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let timeAveragedScore =
    Table.Column.make(
      ~name="Time-Average Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Time-Average Score" |> Utils.ste,
          bodyContent:
            "The average difference in log error between this member's predictions and the aggregate prediction, over the course of the life of the question."
            |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let timeAveragedScore2 =
    Table.Column.make(
      ~name="Time-Average Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Time-Average Score" |> Utils.ste,
          bodyContent:
            "The average difference in log error between this member's predictions and the aggregate prediction, over the course of the life of the question."
            |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.pointScore2
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let totalScore =
    Table.Column.make(
      ~name="Total Net Log Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Total Time-Averaged Log Score" |> Utils.ste,
          bodyContent:
            "The sum of all time-averaged scores by this member." |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Predictions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.predictionCountTotal
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let predictedMeasurablesCount =
    Table.Column.make(
      ~name="Predicted Questions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.numberOfQuestionsScored
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let time =
    Table.Column.make(
      ~name="Time" |> Utils.ste,
      ~render=
        (r: record) =>
          r.createdAt
          |> E.O.fmap(MomentRe.Moment.format("LLL"))
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let getMeasurement = measurement => {
    let bounds = MeasurementsTable.Helpers.bounds([|measurement|]);
    MeasurementsTable.Helpers.smallDistribution(
      ~measurement,
      ~bounds,
      ~width=75,
      ~height=30,
      (),
    )
    |> E.O.default("" |> Utils.ste);
  };

  let competitiveMeasurement =
    Table.Column.make(
      ~name="Prediction" |> Utils.ste,
      ~render=
        (r: record) =>
          r.competitiveMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      ~flex=1,
      (),
    );

  let aggregationMeasurement =
    Table.Column.make(
      ~name="Recent Aggregate" |> Utils.ste,
      ~render=
        (r: record) =>
          r.aggregationMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      ~flex=1,
      (),
    );

  let measurables = [|
    agent,
    measurable,
    timeAveragedScore,
    timeAveragedScore2,
    predictionCount,
    time,
  |];

  let measurables' = [|
    agent,
    timeAveragedScore,
    timeAveragedScore2,
    predictionCount,
    time,
  |];

  let members = [|
    agent,
    totalScore,
    predictedMeasurablesCount,
    predictionCount,
  |];

  let members' = [|
    channel,
    totalScore,
    predictedMeasurablesCount,
    predictionCount,
  |];
};

[@react.component]
let make = (~items, ~columns=Columns.measurables) =>
  Table.fromColumns(columns, items, ());

module Jsx2 = {
  let make = (~items, ~columns=Columns.measurables, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~items, ~columns, ()),
      children,
    );
};