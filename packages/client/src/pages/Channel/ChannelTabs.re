open Utils;
open Routing;

let tab = (isActive, interalUrl, str) =>
  <FC.Tab isActive onClick={LinkType.onClick(Internal(interalUrl))}>
    {str |> ste}
  </FC.Tab>;

let tabToInternalUrl = (channelId, tabSelected: ChannelPage.tab): Url.t => {
  let channelPage: ChannelPage.t = {
    channelId,
    subPage: ChannelPage.SubPage.fromTab(tabSelected),
  };
  channelPage |> Url.fromChannelPage;
};

let make = (tabSelected: Routing.ChannelPage.tab, channel: Primary.Channel.t) =>
  <>
    {tab(
       tabSelected == Measurables,
       tabToInternalUrl(channel.id, Measurables),
       "Questions",
     )}
    {E.React.showIf(
       channel.id != "home",
       tab(
         tabSelected == Members,
         tabToInternalUrl(channel.id, Members),
         (
           channel.membershipCount
           |> E.O.fmap(string_of_int)
           |> E.O.fmap(e => e ++ " ")
           |> E.O.default("")
         )
         ++ "Members",
       ),
     )}
    {tab(
       tabSelected == Updates,
       tabToInternalUrl(channel.id, Updates),
       "Updates",
     )}
    {E.React.showIf(
       channel.myRole === Some(`ADMIN),
       tab(
         tabSelected == Options,
         tabToInternalUrl(channel.id, Options),
         "Settings",
       ),
     )}
  </>;