module CMutationForm =
  MutationForm.Make({
    type queryType = BotUpdate.Query.t;
  });

let component = ReasonReact.statelessComponent("BotEdit");
let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let getForm = (bot: option(Types.bot)) =>
      <BotUpdate.Mutation>
        ...{(mutation, data) => {
          let onSubmit = (values: BotForm.Form.onSubmitAPI): unit => {
            BotUpdate.mutate(
              mutation,
              pageParams.id,
              values.state.values.name,
              values.state.values.description,
              values.state.values.competitorType,
              values.state.values.picture,
            );
            ();
          };

          BotForm.withForm(
            onSubmit,
            bot,
            ({send, state}) => {
              let form =
                BotForm.formFields(state, send, () =>
                  send(BotForm.Form.Submit)
                );

              CMutationForm.showWithLoading2(
                ~result=data.result,
                ~form,
                ~onSuccess=_ => BotForm.onSuccess(loggedInUser, ()),
                (),
              );
            },
          );
        }}
      </BotUpdate.Mutation>;

    let botId = pageParams.id;
    let body = BotGet.component(~id=botId, getForm);

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit a Bot"),
      ~body,
      (),
    )
    |> layout;
  },
};