const readline = require('readline');

const logger = require('./logger');
const Utils = require('./utils/Utils');
const Army = require('./Army');

const ARMY_PREFIX = 'Army';

class GodOfWar {
  numberOfArmies = -1;
  armies = {};
  defeatedArmies = {};

  inputNumberOfArmies() {
    let rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout
    });

    rl.question('Enter number of armies (>=2): ', numberOfArmies => {
      numberOfArmies = parseInt(numberOfArmies);
      if (isNaN(numberOfArmies) || numberOfArmies < 2) {
        logger.warn(`Wrong number of armies entered: "${numberOfArmies}".`);
        numberOfArmies = Utils.getRandomFromSegment(2, 7);
        logger.info(`Randomly selected number of armies: "${numberOfArmies}".`);
      }
      this.numberOfArmies = numberOfArmies;
      rl.close();
      this.inputArmyProperties();
    });
  }

  inputArmyProperties() {
    let properties = {};

    let rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout
    });

    logger.info(`Enter Army${this.armyIndex} properties:`);
    rl.question('Strategy? (random|weakest|strongest): ', strategy => {
      const strategies = ['random', 'weakest', 'strongest'];
      if (strategies.indexOf(strategy) === -1) {
        logger.warn(`Wrong strategy entered: "${strategy}".`);
        strategy = strategies[Utils.getRandomArrayIndex(strategies)];
        logger.info(`Randomly selected strategy: "${strategy}".`);
      }
      properties['strategy'] = strategy;
      rl.question('Number of squads? (>=2): ', numberOfSquads => {
        numberOfSquads = parseInt(numberOfSquads);
        if (isNaN(numberOfSquads) || numberOfSquads < 2) {
          logger.warn(`Wrong number of squads: ${numberOfSquads}.`);
          numberOfSquads = Utils.getRandomFromSegment(2, 7);
          logger.info(`Randomly selected number of squads: ${numberOfSquads}.`);
        }
        properties['numberOfSquads'] = numberOfSquads;
        rl.question('Number of units per squad? (>=5, <=10): ', numberOfUnitsPerSquad => {
          numberOfUnitsPerSquad = parseInt(numberOfUnitsPerSquad);
          if (isNaN(numberOfUnitsPerSquad) || numberOfUnitsPerSquad < 5 || numberOfUnitsPerSquad > 10) {
            logger.warn(`Wrong number of units per squad: ${numberOfUnitsPerSquad}.`);
            numberOfUnitsPerSquad = Utils.getRandomFromSegment(5, 10);
            logger.info(`Randomly selected number of units per squad: ${numberOfUnitsPerSquad}.`);
          }
          properties['numberOfUnitsPerSquad'] = numberOfUnitsPerSquad;
          rl.close();
          this.createArmy(properties);
        });
      });
    });
  }

  armyIndex = 0;
  onArmiesCreated = null;
  createArmy(properties) {
    this.armies[this.armyIndex] = new Army(this.armyIndex, ARMY_PREFIX + this.armyIndex, 
      properties.strategy, properties.numberOfSquads, properties.numberOfUnitsPerSquad, this);
    
    if (++this.armyIndex < this.numberOfArmies) {
      this.inputArmyProperties();
    } else {
      if (this.onArmiesCreated) {
        this.onArmiesCreated();
      }
      // Let's get ready to rumble
      logger.info('Let\'s get ready to rumble!');
      this.rechargeArmies();
    }
  }

  startChaos(onArmiesCreated) {
    this.onArmiesCreated = onArmiesCreated;
    this.inputNumberOfArmies();
  }

  rechargeArmies() {
    Object.values(this.armies).forEach(army => army.recharge());
  }

  selectRandomSquad(attackingArmyId) {
    let randomSquads = [];
    Object.values(this.armies).forEach(army => {
      if (army.getId() !== attackingArmyId) {
        randomSquads.push(army.getRandomSquad());
      }
    });
    let randomSquad;
    if (randomSquads.length > 0) {
      randomSquad = Utils.getRandomArrayValue(randomSquads);
    }
    return randomSquad;
  }

  selectWeakestSquad(attackingArmyId) {
    let weakSquads = [];
    Object.values(this.armies).forEach(army => {
      if (army.getId() !== attackingArmyId) {
        weakSquads.push(army.getWeakestSquad());
      }
    });
    let weakestSquad;
    if (weakSquads.length > 0) {
      weakSquads.sort(Army.squadsSortingPredicate);
      weakestSquad = weakSquads[0];
    }
    return weakestSquad;
  }

  selectStrongestSquad(attackingArmyId) {
    let strongSquads = [];
    Object.values(this.armies).forEach(army => {
      if (army.getId() !== attackingArmyId) {
        strongSquads.push(army.getStrongestSquad());
      }
    });
    let strongestSquad;
    let length = strongSquads.length;
    if (length > 0) {
      strongSquads.sort(Army.squadsSortingPredicate);
      strongestSquad = strongSquads[length - 1];
    }
    return strongestSquad;
  }

  selectOpponentSquad(attackStrategy, attackingArmyId) {
    let opponentSquad;
    switch(attackStrategy) {
      case 'random':
        opponentSquad = this.selectRandomSquad(attackingArmyId);
        break;
      case 'weakest':
        opponentSquad = this.selectWeakestSquad(attackingArmyId);
        break;
      case 'strongest':
        opponentSquad = this.selectStrongestSquad(attackingArmyId);
        break;
      default:
        logger.error('C\'mon!?');
        break;
    }
    return opponentSquad;
  }

  /*
    Once the target is determined both the attacking and defending squads calculate their attack
    probability success and the squad with the highest probability wins. If the attacking squad wins,
    damage is dealt to the defending side. If the attacking squad loses, no damage is dealt to either
    side.
  */
  attack(attackingSquad, attackingArmy) {
    let attackStrategy = attackingArmy.getAttackStrategy()
    let attackingArmyId = attackingArmy.getId();
    let opponentSquad = this.selectOpponentSquad(attackStrategy, attackingArmyId);
    let opponentArmyId = opponentSquad.getArmyId();
    let opponentArmy = this.armies[opponentArmyId];
    let attackerSuccessProbability = attackingSquad.getAttackSuccessProbability();
    let opponentSuccessProbability = opponentSquad.getAttackSuccessProbability();

    if (attackerSuccessProbability > opponentSuccessProbability) {
      let damage = attackingSquad.getInflictDamage();
      logger.info(`Squad "${opponentSquad.getId()}-(${opponentArmy.getName()})" attacked by squad "${attackingSquad.getId()}-(${attackingArmy.getName()})" damage(${damage}).`);
      let opponentSquadActive = opponentSquad.hurt(damage);
      attackingSquad.addExperience();
      if (!opponentSquadActive) {
        opponentArmy.defeatSquad(opponentSquad.getId());

        if (!opponentArmy.isActive()) {
          logger.info(`Army "${opponentArmy.getName()}" defeated.`);
          this.defeatArmy(opponentArmyId);
        }
      }
    }
    if (this.isWarEnded()) {
      logger.info(`War ended. ${attackingArmy.getName()} won.`);
    } else {
      attackingSquad.recharge();
    }
  }

  defeatArmy(armyId) {
    this.defeatedArmies[armyId] = this.armies[armyId];
    delete this.armies[armyId];
  }

  isWarEnded() {
    return Object.keys(this.armies).length === 1;
  }

}

module.exports = GodOfWar;
