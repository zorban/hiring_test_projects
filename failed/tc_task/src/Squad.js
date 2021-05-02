const Utils = require('./utils/Utils');
const Soldier = require('./units/Soldier');
const Vehicle = require('./units/Vehicle');

const VEHICLE_PERCENT = 0.2;

class Squad {
  id = -1;
  armyId = -1;
  units = {};
  rechargeTime = -1;
  killedUnits = {};

  constructor(id, numberOfUnits, armyId) {
    this.id = id;
    this.armyId = armyId;
    let i = 0;
    let numberOfVehicles = Math.floor(VEHICLE_PERCENT * numberOfUnits);
    let numberOfSoldiers = numberOfUnits - numberOfVehicles;
    while (numberOfVehicles--) {
      this.units[i] = new Vehicle(Utils.getRandomFromSegment(1, 3), i); // numberOfOperators [1-3]
      ++i;
    }

    while (numberOfSoldiers--) {
      this.units[i] = new Soldier(i);
      ++i;
    }

    this.findRechargeTime();
  }

  getId() {
    return this.id;
  }

  getArmyId() {
    return this.armyId;
  }

  getUnits() {
    return this.units;
  }

  getKilledUnits() {
    return this.killedUnits;
  }

  findRechargeTime() {
    this.rechargeTime = Object.values(this.units)
      .map(unit => unit.getRechargeTime())
      .reduce((a, b) => Math.max(a, b), -1);
  }

  getRechargeTime() {
    return this.rechargeTime;
  }

  getAttackSuccessProbability() {
    let unitsArr = Object.values(this.units);
    return Math.pow(unitsArr.map(unit => unit.getAttackSuccessProbability())
      .reduce((a, b) => a * b, 1), 1 / unitsArr.length);
  }

  getInflictDamage() {
    return Object.values(this.units).map(unit => unit.getInflictDamage())
      .reduce((a, b) => a + b, 0);
  }

  hurt(damage) {
    let killedUnitKeys = [];
    let unitKeysArray = Object.keys(this.units);
    let damagePerUnit = damage / unitKeysArray.length;
    unitKeysArray.forEach(unitKey => {
      let unitActive = this.units[unitKey].hurt(damagePerUnit);
      if (!unitActive) {
        killedUnitKeys.push(unitKey);
      }
    });
    killedUnitKeys.forEach(unitKey => {
      this.killedUnits[unitKey] = this.units[unitKey];
      delete this.units[unitKey];
    });

    return this.isActive();
  }

  addExperience() {
    Object.values(this.units).forEach(unit => unit.addExperience());
  }

  isActive() {
    return Object.keys(this.units).length > 0;
  }

  getTotalHealth() {
    return Object.values(this.units).map(unit => unit.getHealth())
      .reduce((a, b) => a + b, 0);
  }

  getExperiencePerUnit() {
    let unitsArray = Object.values(this.units);
    return unitsArray
      .map(unit => unit.getExperience())
      .reduce((a, b) => a + b, 0)
        / unitsArray.length;
  }

  getNumberOfUnits() {
    return Object.keys(this.units).length;
  }

  getTotalDamage() {
    return Object.values(this.units)
      .map(unit => unit.getInflictDamage())
      .reduce((a, b) => a + b, 0);
  }

  rechargeStartTimestamp = -1;
  getRechargeStartTimestamp() {
    return this.rechargeStartTimestamp;
  }
  
  onRechargeCallback = null;
  setOnRechargeCallback(onRechargeCallback) {
    this.onRechargeCallback = onRechargeCallback;
  }

  onRecharge() {
    if (this.onRechargeCallback) {
      this.onRechargeCallback(this);
    }
  }
  recharge() {
    if (!this.isActive()) {
      return;
    }
    this.rechargeStartTimestamp = Date.now();
    setTimeout(this.onRecharge.bind(this), this.rechargeTime);
  }
}

module.exports = Squad;
