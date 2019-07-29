[@bs.module]
external reactClass: ReasonReact.reactClass = "antd/lib/input-number";

[%bs.raw {|require("antd/lib/input-number/style")|}];

[@bs.obj]
external makeProps:
  (
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    ~disabled: bool,
    ~defaultValue: float=?,
    ~parser: string => string=?,
    ~formatter: string => string=?,
    ~value: float=?,
    ~min: float=?,
    ~max: float=?,
    ~step: float=?,
    ~onChange: float => unit=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~className=?,
      ~style=?,
      ~onChange=?,
      ~defaultValue=?,
      ~parser=?,
      ~formatter=?,
      ~value=?,
      ~min=?,
      ~max=?,
      ~step=?,
      ~disabled=false,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~className?,
        ~style?,
        ~parser?,
        ~formatter?,
        ~onChange?,
        ~defaultValue?,
        ~value?,
        ~min?,
        ~max?,
        ~step?,
        ~disabled,
        (),
      ),
    children,
  );