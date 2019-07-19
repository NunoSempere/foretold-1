/* TODO: Allow for cases where user is not logged in */

let makeWithPage =
    (channelPage: Routing.ChannelPage.t, loggedInUser)
    : ReasonReact.reactElement => {
  let channelId = channelPage.channelId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let loadChannel = ChannelGet.component2(~id=channelId);

    let toEl = fn => fn |> E.React.makeToEl(~key=channelId);

    let successFn = (channel: Primary.Channel.t) => {
      let layout =
        Channel_Layout_C.makeWithEl(
          channelPage,
          loggedInUser,
          Some(channel),
        );

      switch (channelPage.subPage) {
      | Measurables(searchParams) =>
        MeasurableIndex.make(
          ~channelId,
          ~searchParams,
          ~loggedInUser,
          ~itemsPerPage=20,
          ~layout,
        )
        |> toEl
      | Measurable(measurableId) =>
        ChannelMeasurable.make(~measurableId, ~loggedInUser, ~layout) |> toEl
      | Series(id) =>
        SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout) |> toEl
      | NewMeasurable => MeasurableNew.make(~channelId, ~layout) |> toEl
      | Members => ChannelMembers.make(~channelId, ~layout, ~channel) |> toEl
      | InviteNewMember =>
        ChannelInvite.make(~channelId, ~loggedInUser, ~layout) |> toEl
      | Settings => ChannelEdit.make(~channelId, ~layout) |> toEl
      | NewSeries =>
        SeriesNew.make(~channelId, ~loggedInUser, ~layout) |> toEl
      };
    };

    let errorFn = _ => {
      let layout =
        Channel_Layout_C.makeWithEl(channelPage, loggedInUser, None);

      SLayout.LayoutConfig.make(
        ~head=<div />,
        ~body=<div> {"No channel." |> ReasonReact.string} </div>,
      )
      |> layout;
    };

    let loadingFn = () => {
      let layout =
        Channel_Layout_C.makeWithEl(channelPage, loggedInUser, None);

      SLayout.LayoutConfig.make(~head=<div />, ~body=<SLayout.Spin />)
      |> layout;
    };

    loadChannel(result =>
      result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
    );
  | None => <Home />
  };
};