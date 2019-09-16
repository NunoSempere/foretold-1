type predictionGroupErrorType = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

let predictionGroupError:
  (~scoringCombination: predictionGroupErrorType, ~sampleCount: int, unit) =>
  Belt.Result.t(float, 'a);

let differentialEntropy:
  (int, PredictionAnalysis.MeasurementValue.t) => option(float);