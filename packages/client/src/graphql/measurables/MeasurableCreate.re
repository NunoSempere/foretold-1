[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
             mutation measurableCreate($input: MeasurableCreateInput!) {
                 measurableCreate(input: $input) {
                   id
                   channelId
                 }
             }
     |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);