const _ = require('lodash');
const path = require('path');
const cors = require('cors');
const express = require('express');
const bodyParser = require('body-parser-graphql');

const config = require('./config');
const { runJobs } = require('./async');
const { runListeners } = require('./async/listeners');
const events = require('./async/events');
const emitter = require('./async/emitter');
const { apolloServer } = require('./apollo-server');

{
  runJobs();
  runListeners();
}

const app = express();
app.use(cors());

{
  const fallbackFile = path.resolve(__dirname, '../../client/dist/index.html');
  const distDir = path.resolve(__dirname, '../../client/dist');

  console.log('Fallback file', fallbackFile);
  console.log('Dist dir', distDir);

  // Returns all routes excluding "/graphql" as static files
  // or returns fallback page.
  app.get(/^((?!(graphql|hooks|env\.js)).)*$/,
    express.static(distDir),
    (req, res) => res.sendFile(fallbackFile),
  );
}

{
  const url = process.env.API_URL
    || `https://${process.env.HEROKU_APP_NAME}.herokuapp.com/graphql`;
  app.get('/env.js', (_req, res) => res.send(
    `window.ENV = { ` +
    `API_URL: "${url}", ` +
    `AUTH0_DOMAIN: "${process.env.AUTH0_DOMAIN}", ` +
    `AUTH0_CLIENT_ID: "${process.env.AUTH0_CLIENT_ID}", ` +
    `}`
  ));
}

{
  const { app: subApp } = require('./github/app');
  app.use('/hooks', subApp);
}

{
  app.use(bodyParser.graphql());
  apolloServer.applyMiddleware({ app });
}

app.listen({ port: config.PORT }, () => {
  console.log(`Server ready at http://localhost:${config.PORT}`);
  emitter.emit(events.SERVER_IS_READY, app);
});
