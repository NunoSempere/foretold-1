open Style.Grid;

module StatusDisplay = Foretold__Component__StatusDisplay;
module Items = Foretold__Components__Measurable__Items;

let component = ReasonReact.statelessComponent("MeasurableFullPresentation");

module Styles = {
  open Css;

  let header =
    style(
      [padding2(~v=`em(1.5), ~h=`em(1.5))]
      @ FC.Base.BaseStyles.fullWidthFloatLeft,
    );

  let link = style([marginTop(em(1.))]);

  let description = style([paddingTop(`em(1.5))]);
};

let make = (~id: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _self => {
    MeasurableWithMeasurementsGet.component(~id)
    |> E.F.apply((m: Types.measurable) => {
         let userAgentId =
           loggedInUser.agent |> E.O.fmap((r: Primary.Agent.t) => r.id);

         let creatorId = m.creator |> E.O.fmap((r: Primary.Agent.t) => r.id);

         <>
           <Div styles=[Styles.header]>
             <Div flexDirection=`row>
               <Div flex={`num(3.)}>
                 <FC.PageCard.H1> {Items.link(~m)} </FC.PageCard.H1>
                 <StatusDisplay measurable=m />
               </Div>
               <Div flex={`num(1.)}>
                 {Items.series(~m, ()) |> E.O.React.defaultNull}
                 {Items.creatorLink(~m) |> E.O.React.defaultNull}
                 {Items.editLink(~m)}
                 {Items.resolutionEndpoint(~m) |> E.O.React.defaultNull}
                 {Items.endpointResponse(~m) |> E.O.React.defaultNull}
                 {Items.questionLink(~m)}
               </Div>
             </Div>
             {Items.description(~m)
              |> E.O.React.fmapOrNull(d =>
                   <Div styles=[Styles.description]> d </Div>
                 )}
           </Div>
           <>
             {userAgentId == creatorId
              || Primary.Measurable.toStatus(m) !== `JUDGED
                ? <Foretold__Components__Measurement__Form
                    measurable=m
                    measurableId=id
                    isCreator={userAgentId == creatorId}
                    loggedInUser
                  />
                : E.React.null}
             {MeasurementsGet.component(
                ~measurableId=m.id,
                ~pageLimit=20,
                ~direction=None,
                ~innerComponentFn=(
                                    m:
                                      option(
                                        Primary.Connection.t(
                                          Primary.Measurement.t,
                                        ),
                                      ),
                                  ) =>
                m
                |> E.O.React.fmapOrNull(
                     (b: Primary.Connection.t(Primary.Measurement.t)) =>
                     b.edges
                     |> E.A.to_list
                     |> Foretold__Components__Measurements__Table.make
                   )
              )}
           </>
         </>;
       });
  },
};