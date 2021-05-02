const logger = require('./logger');
const GodOfWar = require('./GodOfWar');
const godOfWar = new GodOfWar();

const check = () => {
  console.log(godOfWar);
};

godOfWar.startChaos(check);
