open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
  module GraphQL = [%graphql
    {|
             mutation measurableCreate($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!, $descriptionDate: Date, $descriptionProperty: String, $channelId: String!) {
                 measurableCreate(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity, descriptionDate: $descriptionDate, descriptionProperty: $descriptionProperty, channelId: $channelId) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);
};

module SignUpForm = ReForm.Create(MeasurableForm.SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: SignUpForm.values,
      channelId: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE" ?
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~descriptionDate=values.descriptionDate |> Js.Json.string,
        ~valueType=`FLOAT,
        ~channelId,
        (),
      ) :
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~valueType=`FLOAT,
        ~channelId,
        (),
      );
  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};

let component = ReasonReact.statelessComponent("Measurables");

module CMutationForm =
  MutationForm.Make({
    type queryType = CreateMeasurableMutation.GraphQL.t;
  });

let make = (~channelId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      SignUpForm.make(
        ~onSubmit=({values}) => mutate(mutation, values, channelId),
        ~initialState={
          name: "",
          description: "",
          descriptionProperty: "",
          descriptionEntity: "",
          expectedResolutionDate:
            MomentRe.momentNow() |> MeasurableForm.formatDate,
          descriptionDate: MomentRe.momentNow() |> MeasurableForm.formatDate,
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("New Measurable"),
      ~body=
        CreateMeasurableMutation.Mutation.make(
          ~onCompleted=
            () => Context.Routing.Url.push(ChannelShow(channelId)),
          (mutation, data) =>
            formCreator(mutation, ({handleSubmit, handleChange, form, _}) =>
              CMutationForm.showWithLoading(
                ~result=data.result,
                ~form=
                  MeasurableForm.showForm(
                    ~form,
                    ~handleSubmit,
                    ~handleChange,
                  ),
                (),
              )
            )
            |> E.React.el,
        )
        |> E.React.el,
    )
    |> layout;
  },
};