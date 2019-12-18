[@react.component]
let make = (~measurableId: string) => {
  MeasurableGet.component(~id=measurableId)
  |> E.F.apply((measurable: Types.measurable) =>
       <>
         <SLayout isFluid=true> <MeasurablePage id=measurableId /> </SLayout>
         <MeasurableBottomSection measurable />
       </>
     );
};