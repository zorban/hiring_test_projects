const Utils = require('../utils/Utils');
const Unit = require('./Unit');
const Soldier = require('./Soldier');

const VEHICLE_DAMAGE_FACTOR = 0.3;
const UNLUCKY_OPERATOR_DAMAGE_FACTOR = 0.5;
const OTHER_OPERATORS_DAMAGE_FACTOR = 0.2;

class Vehicle extends Unit {
  operators = {};
  killedOperators = {};

  constructor(numberOfOperators, id) { // numberOfOperators [1-3]
    super(id);
    this.setRechargeTime(Utils.getRandomFromSegment(1001, 2000));
    let i = 0;
    while (numberOfOperators--) {
      this.operators[i] = new Soldier(i);
      ++i;
    }
  }

  getOperators() {
    return this.operators;
  }

  getKilledOperators() {
    return this.killedOperators;
  }

  getHealth() {
    let operatorsArray = Object.values(this.operators);
    return (super.getHealth() + operatorsArray.map(soldier => soldier.getHealth())
      .reduce((a, b) => a + b, 0)) / (operatorsArray.length + 1);
  }

  getExperience() {
    let operatorsArray = Object.values(this.operators);
    return operatorsArray
      .map(soldier => soldier.getExperience())
      .reduce((a, b) => a + b, 0)
        / operatorsArray.length;
  }

  isActive() {
    return super.isActive() && Object.values(this.operators)
      .map(soldier => soldier.getHealth())
      .reduce((a, b) => a + b, 0) > 0;
  }

  getAttackSuccessProbability() {
    let operatorsArr = Object.values(this.operators);
    let operatorsCount = operatorsArr.length;
    let operatorsSuccess = 0;
    if (operatorsCount > 0) {
      operatorsSuccess = Math.pow(operatorsArr
        .map(soldier => soldier.getAttackSuccessProbability())
        .reduce((a, b) => a * b, 1), 
      1 / operatorsCount);
    }
    return 0.5 * (1 + this.health / 100) * operatorsSuccess;
  }

  getInflictDamage() {
    return 0.1 + Object.values(this.operators)
      .map(soldier => soldier.getExperience())
      .reduce((a, b) => a + b, 0) / 100;
  }

  hurt(damage) {
    super.hurt(VEHICLE_DAMAGE_FACTOR * damage);
    let operatorKeys = Object.keys(this.operators);
    let unluckyOperatorIndex = Utils.getRandomArrayIndex(operatorKeys);
    let unluckyOperatorKey = operatorKeys[unluckyOperatorIndex];
    let unluckyOperatorActive = this.operators[unluckyOperatorKey].hurt(UNLUCKY_OPERATOR_DAMAGE_FACTOR * damage);
    let killedOperatorKeys = [];
    if (!unluckyOperatorActive) {
      killedOperatorKeys.push(unluckyOperatorKey);
    }
    let numberOfOtherOperators = operatorKeys.length - 1;
    if (numberOfOtherOperators > 0) {
      let damagePerOperator = OTHER_OPERATORS_DAMAGE_FACTOR / numberOfOtherOperators * damage;
      operatorKeys.forEach((operatorKey, operatorIndex) => {
        if (operatorIndex !== unluckyOperatorIndex) {
          let operatorActive = this.operators[operatorKey].hurt(damagePerOperator);
          if (!operatorActive) {
            killedOperatorKeys.push(operatorKey);
          }
        }
      });
    } else {
      super.hurt(OTHER_OPERATORS_DAMAGE_FACTOR * damage);
    }
    killedOperatorKeys.forEach(killedOperatorKey => {
      this.killedOperators[killedOperatorKey] = this.operators[killedOperatorKey];
      delete this.operators[killedOperatorKey];
    });
    return this.isActive();
  }

  addExperience() {
    Object.values(this.operators).forEach(soldier => soldier.addExperience());
  }
}

module.exports = Vehicle;
