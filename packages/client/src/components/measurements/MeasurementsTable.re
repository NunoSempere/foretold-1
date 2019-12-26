module Styles = {
  open Css;

  let descriptionStyle =
    style([
      marginTop(`px(5)),
      selector(
        " p",
        [color(`rgba((80, 80, 80, 0.78))), marginBottom(`px(10))],
      ),
    ]);

  let date = style([color(`rgba((35, 35, 35, 0.34)))]);

  let agentStyle =
    style([color(`rgb((102, 121, 134))), fontSize(`em(1.1))]);

  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Settings.accentBlue)]);

  let percentage = style([fontSize(`em(1.15))]);

  let result =
    style([
      fontSize(`em(1.15)),
      fontWeight(`num(800)),
      color(`hex("7d7ea2")),
    ]);
};

module Helpers = {
  module SmallDistribution = {
    [@react.component]
    let make =
        (
          ~measurement: Types.measurement,
          ~bounds: (float, float),
          ~width=230,
          ~height=38,
        ) =>
      switch (measurement.value) {
      | Ok(`FloatCdf(r)) =>
        let (minX, maxX) = bounds;
        r
        |> MeasurementValue.toPdf
        |> MeasurementValue.FloatCdf.toJs
        |> (
          data =>
            <SmallCdfChart
              minX={Some(minX)}
              maxX={Some(maxX)}
              data
              width
              height
              color={
                measurement.competitorType == `AGGREGATION
                  ? `hex("b1b9c6") : `hex("487192")
              }
            />
        );
      | Ok(`FloatPoint(r)) =>
        <div className=Styles.result>
          {r |> E.Float.with3DigitsPrecision |> Utils.ste}
        </div>
      | _ => <Null />
      };
  };

  module StatSummary = {
    [@react.component]
    let make = (~measurement: Types.measurement) =>
      switch (measurement.value) {
      | Ok(`FloatCdf(r)) =>
        r
        |> MeasurementValue.FloatCdf.toJs
        |> FC.Base.Types.Dist.fromJson
        |> (cdf => <FC__CdfChart__StatSummary cdf showMean=false />)
      | Ok(`FloatPoint(r)) => <FC__NumberShower precision=8 number=r />
      | Ok(`Percentage(r)) =>
        <span className=Styles.percentage>
          <FC__PercentageShower precision=8 percentage=r />
        </span>
      | Ok(`Binary(r)) =>
        <span className=Styles.result>
          {(r ? "Yes" : "No") |> Utils.ste}
        </span>
      | Ok(`UnresolvableResolution(r)) =>
        <span className=Styles.result>
          {r
           |> MeasurementValue.UnresolvableResolution.toPublicString
           |> Utils.ste}
        </span>
      | _ => <Null />
      };
  };

  module ValueText = {
    [@react.component]
    let make = (~measurement: Types.measurement) =>
      <div className=Styles.secondaryText>
        {measurement.valueText
         |> E.O.bind(_, r => r == "" ? None : Some(r))
         |> E.O.fmap(r => {|"|} ++ r ++ {|"|})
         |> E.O.default("")
         |> Utils.ste}
      </div>;
  };

  module Description = {
    [@react.component]
    let make = (~m: Types.measurement) => {
      switch (m.description) {
      | None
      | Some("") => <Null />
      | Some(description) =>
        <div className=Styles.descriptionStyle>
          <Markdown source=description />
        </div>
      };
    };
  };

  module RelevantAt = {
    [@react.component]
    let make = (~m: Types.measurement) =>
      m.relevantAt
      |> E.O.fmap(d =>
           <div className=Styles.date>
             {d |> E.M.goFormat_standard |> Utils.ste}
           </div>
         )
      |> E.O.default(<Null />);
  };

  let getFloatCdf = (e: Belt.Result.t(MeasurementValue.t, string)) =>
    switch (e) {
    | Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let bounds = (measurements: Js_array.t(Types.measurement)) => {
    let itemBounds =
      measurements
      |> E.A.keepMap(_, r => getFloatCdf(r.value))
      |> E.A.fmap(r =>
           (
             E.FloatCdf.firstAboveValue(0.02, r),
             E.FloatCdf.firstAboveValue(0.98, r),
           )
         );

    let min =
      itemBounds
      |> E.A.keepMap(_, ((min, _)) => min)
      |> E.A.fold_left((a, b) => a < b ? a : b, max_float);

    let max =
      itemBounds
      |> E.A.keepMap(_, ((_, max)) => max)
      |> E.A.fold_left((a, b) => a > b ? a : b, min_float);

    (min, max);
  };

  let toChartMeasurement = (measurement: Types.measurement) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      switch (
        E.FloatCdf.firstAboveValue(0.02, r),
        E.FloatCdf.firstAboveValue(0.98, r),
      ) {
      | (Some(low), Some(high)) => Some((low, high))
      | _ => None
      }
    | _ => None
    };

  module MeasurerLink = {
    [@react.component]
    let make = (~measurement: Types.measurement) => {
      let aLink = measurement.agent |> E.O.fmap(agent => <AgentLink agent />);

      let judgementStyle =
        Css.(
          style([
            selector(
              " h3",
              [
                color(`rgba((55, 47, 68, 0.85))),
                marginBottom(`px(0)),
                fontSize(`em(1.15)),
                fontWeight(`num(800)),
              ],
            ),
            selector(" a", [fontSize(`em(0.9))]),
          ])
        );

      let isJudge = Primary.Measurement.isJudgement(measurement);

      if (isJudge) {
        <div className=judgementStyle>
          {"Resolution" |> Utils.ste |> E.React2.inH3}
          {switch (aLink) {
           | Some(name) => <> name </>
           | None => E.React2.null
           }}
        </div>;
      } else {
        aLink |> E.O.React.defaultNull;
      };
    };
  };
};

module MeasurableLink = {
  [@react.component]
  let make = (~measurement: Types.measurement) => {
    switch (measurement.measurable) {
    | None => "" |> Utils.ste
    | Some(measurable) => <MeasurableItems.LinkMeasurable measurable />
    };
  };
};

let predictionValue =
  Table.Column.make(
    ~name="Prediction" |> Utils.ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) =>
        <div> <Helpers.StatSummary measurement /> </div>,
    (),
  );

let predictionText =
  Table.Column.make(
    ~name={
      "Input Text" |> Utils.ste;
    },
    ~flex=3,
    ~render=
      (measurement: Types.measurement) =>
        <div> <Helpers.ValueText measurement /> </div>,
    (),
  );

let agent =
  Table.Column.make(
    ~name="Member" |> Utils.ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) =>
        <Helpers.MeasurerLink measurement />,
    (),
  );

let time =
  Table.Column.make(
    ~name="Time" |> Utils.ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) =>
        <Helpers.RelevantAt m=measurement />,
    (),
  );

let measurable =
  Table.Column.make(
    ~name="Measurable" |> Utils.ste,
    ~render=
      (measurement: Types.measurement) => <MeasurableLink measurement />,
    ~flex=5,
    (),
  );

let score = _ =>
  Table.Column.make(
    ~name={
      "Relative Score" |> Utils.ste;
    },
    ~help=
      Some({
        headerContent: "Relative Score" |> Utils.ste,
        bodyContent:
          "The difference between the log error for this predictions vs. the log score for the most recent aggregate prediction."
          |> Utils.ste,
      }),
    ~render=
      (measurement: Types.measurement) =>
        measurement.measurementScoreSet
        |> E.O.ffmap((o: Types.measurementScoreSet) =>
             o.primaryPointScore |> E.O.fmap(E.Float.with3DigitsPrecision)
           )
        |> E.O.default("")
        |> Utils.ste,
    ~flex=5,
    (),
  );

let logScore = _ =>
  Table.Column.make(
    ~name="Log Score" |> Utils.ste,
    ~help=
      Some({
        headerContent: "Log Score" |> Utils.ste,
        bodyContent:
          "The absolute log error of the forecast vs. the most recent resolution."
          |> Utils.ste,
      }),
    ~render=
      (measurement: Types.measurement) =>
        measurement.measurementScoreSet
        |> E.O.bind(_, (o: Types.measurementScoreSet) =>
             o.nonMarketLogScore |> E.O.fmap(E.Float.with3DigitsPrecision)
           )
        |> E.O.default("")
        |> Utils.ste,
    ~flex=5,
    (),
  );

let getPredictionDistribution = (~bounds, ~width=230, ()) =>
  Table.Column.make(
    ~name="Distribution" |> Utils.ste,
    ~flex=7,
    ~render=
      (measurement: Types.measurement) =>
        <Helpers.SmallDistribution measurement bounds width />,
    ~show=
      (measurement: Types.measurement) =>
        switch (measurement.measurable) {
        | Some(measurable) => measurable.valueType !== `PERCENTAGE
        | _ => true
        },
    (),
  );

// ?
let bottomSubRowFn =
  Some(
    (measurement: Types.measurement) =>
      switch (measurement.description) {
      | Some(d) =>
        Some([|
          FC.Table.Row.textSection(<Helpers.Description m=measurement />),
        |])
      | _ => None
      },
  );

type columns = [ | `none | `extended];
type block = [ | `none | `inside];

[@react.component]
let make =
    (
      ~measurementsList: list(Types.measurement),
      ~measurableValueType: Types.valueType,
      ~colums: columns=`none,
      ~block: block=`none,
    ) => {
  let bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  let all =
    switch (colums, block, measurableValueType) {
    | (`none, `none, `FLOAT) => [|
        agent,
        getPredictionDistribution(~bounds, ()),
        predictionValue,
        predictionText,
        time,
      |]
    | (`none, `none, `PERCENTAGE) => [|
        agent,
        getPredictionDistribution(~bounds, ()),
        predictionValue,
        time,
      |]
    | (`extended, `none, `FLOAT) => [|
        agent,
        getPredictionDistribution(~bounds, ()),
        predictionValue,
        predictionText,
        logScore(),
        score(),
        time,
      |]
    | (`extended, `none, `PERCENTAGE) => [|
        agent,
        getPredictionDistribution(~bounds, ()),
        predictionValue,
        logScore(),
        score(),
        time,
      |]
    | (`none, `inside, `FLOAT) => [|
        agent,
        getPredictionDistribution(~bounds, ~width=150, ()),
        predictionText,
        time,
      |]
    | (`none, `inside, `PERCENTAGE) => [|
        agent,
        getPredictionDistribution(~bounds, ~width=150, ()),
        time,
      |]
    | (`extended, `inside, `FLOAT) => [|
        agent,
        getPredictionDistribution(~bounds, ~width=150, ()),
        predictionText,
        logScore(),
        score(),
        time,
      |]
    | (`extended, `inside, `PERCENTAGE) => [|
        agent,
        getPredictionDistribution(~bounds, ~width=150, ()),
        logScore(),
        score(),
        time,
      |]
    | _ => Js.Exn.raiseError("Date not supported")
    };

  let measurementsList' = measurementsList;

  measurementsList' |> E.L.length > 0
    ? <FC.PageCard.Body>
        <Table
          columns=all
          rows={measurementsList' |> Array.of_list}
          bottomSubRowFn
        />
      </FC.PageCard.Body>
    : <NothingToShow />;
};